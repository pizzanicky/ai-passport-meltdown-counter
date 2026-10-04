#include "meltdown_app.h"

#include "meltdown_audio.h"
#include "meltdown_model.h"
#include "meltdown_network.h"
#include "meltdown_store.h"
#include "meltdown_ui.h"

#include "bsp_battery.h"
#include "bsp_button.h"
#include "bsp_display.h"
#include "bsp_i2c.h"
#include "bsp_pins.h"

#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include <string.h>
#include <time.h>

static const char *TAG = "meltdown";

#define INPUT_QUEUE_DEPTH 16
/* Pays for the NimBLE host stack growing to 8192 during DH negotiation. */
#define APP_TASK_STACK 8192
#define IDLE_BLANK_US (60LL * 1000 * 1000)
#define BATTERY_POLL_US (5LL * 1000 * 1000)
#define MUTE_FLASH_US (800LL * 1000)
#define PROVISION_WAIT_US (15LL * 1000 * 1000)
#define SYNC_FAIL_US (30LL * 1000 * 1000)
#define LOOP_DELAY_MS 20

typedef struct {
    bsp_btn_t button;
    bsp_btn_ev_t event;
} button_event_t;

static QueueHandle_t s_queue;
static meltdown_state_t s_state;
static bool s_record_dirty;
static bool s_save_failed;
static bool s_rollover_pending;
static bool s_ui_dirty = true;
static bool s_blanked;
static bool s_battery_ready;
static bool s_net_started;
static const char *s_sync_line = "正在校时";
static int64_t s_boot_us;
static int64_t s_idle_us;
static int64_t s_mute_until_us;
static int64_t s_save_log_us;
static int64_t s_provision_retry_us;
static int64_t s_battery_us;
static int s_battery_soc = -1;

static void on_button(bsp_btn_t button, bsp_btn_ev_t event, void *user)
{
    (void)user;
    const button_event_t item = {.button = button, .event = event};
    if (!s_queue || xQueueSend(s_queue, &item, 0) != pdTRUE) {
        ESP_LOGW(TAG, "input queue full");
    }
}

static int map_button(const button_event_t *event, meltdown_input_t *input)
{
    if (event->event == BSP_BTN_PRESS) return 0;
    if (event->event == BSP_BTN_LONG) {
        if (event->button == BSP_BTN_OK) {
            *input = MELTDOWN_IN_OK_LONG;
            return 1;
        }
        if (event->button == BSP_BTN_UP) {
            *input = MELTDOWN_IN_UP_LONG;
            return 1;
        }
        return 0;
    }
    if (event->button == BSP_BTN_UP) *input = MELTDOWN_IN_UP;
    else if (event->button == BSP_BTN_DOWN) *input = MELTDOWN_IN_DOWN;
    else if (event->button == BSP_BTN_OK) *input = MELTDOWN_IN_OK;
    else return 0;
    if (event->event == BSP_BTN_CLICK) return 1;
    /* A double that still arrives inside the short window is two records. */
    if (event->event == BSP_BTN_DOUBLE) return 2;
    return 0;
}

static void note_save(esp_err_t err)
{
    const int64_t now = esp_timer_get_time();
    if (err == ESP_OK) {
        s_record_dirty = false;
        /* The count is already on screen. Refresh again only to clear a previous error. */
        if (!s_rollover_pending && s_save_failed) {
            s_save_failed = false;
            s_ui_dirty = true;
        }
        return;
    }
    s_record_dirty = true;
    if (!s_save_failed) s_ui_dirty = true;
    s_save_failed = true;
    if (now - s_save_log_us > 1000000) {
        ESP_LOGE(TAG, "record save failed: %s", esp_err_to_name(err));
        s_save_log_us = now;
    }
}

static void save_record(void)
{
    note_save(meltdown_store_save(&s_state.record));
}

static bool commit_new_month(void)
{
    meltdown_record_t staged;
    meltdown_prepare_month(&s_state.record, &s_state.today, &staged);
    const esp_err_t err = meltdown_store_save(&staged);
    if (err != ESP_OK) {
        s_save_failed = true;
        s_ui_dirty = true;
        const int64_t now = esp_timer_get_time();
        if (now - s_save_log_us > 1000000) {
            ESP_LOGE(TAG, "month save failed: %s", esp_err_to_name(err));
            s_save_log_us = now;
        }
        return false;
    }
    meltdown_commit_month(&s_state, &staged);
    s_record_dirty = false;
    s_save_failed = false;
    s_rollover_pending = false;
    s_ui_dirty = true;
    return true;
}

static void clear_credentials(void)
{
    if (!s_net_started && meltdown_network_start() == ESP_OK) s_net_started = true;
    if (s_net_started) {
        const esp_err_t err = meltdown_network_clear_credentials();
        if (err != ESP_OK) ESP_LOGE(TAG, "clear credentials failed: %s", esp_err_to_name(err));
    }
    if (meltdown_network_provision_start() != ESP_OK) {
        s_sync_line = "校时失败";
    } else {
        s_sync_line = "请打开小程序";
    }
    s_ui_dirty = true;
}

static void hold_unfiled_press(void)
{
    meltdown_effect_t held;
    meltdown_note_unfiled(&s_state, &held);
    if (held.stop_audio) meltdown_audio_stop();
    if (held.play) meltdown_audio_play(held.sound);
    if (held.changed) s_record_dirty = true;
    s_ui_dirty = true;
}

static void handle_input(meltdown_input_t input, bool allow_rollover)
{
    meltdown_effect_t effect;
    meltdown_apply_input(&s_state, input, &effect);
    if (effect.stop_audio) meltdown_audio_stop();
    if (effect.clear_credentials) clear_credentials();
    if (effect.needs_rollover) {
        if (allow_rollover && commit_new_month()) {
            handle_input(input, false);
            return;
        }
        s_rollover_pending = true;
        /* Navigation can be pressed again. A counted press must not disappear. */
        if (input == MELTDOWN_IN_OK) hold_unfiled_press();
        s_ui_dirty = true;
        return;
    }
    if (effect.play) meltdown_audio_play(effect.sound);
    if (effect.changed) s_record_dirty = true;
    if (s_state.mute_flash) s_mute_until_us = esp_timer_get_time() + MUTE_FLASH_US;
    s_ui_dirty = true;
}

static void retry_rollover(void)
{
    if (!s_rollover_pending || !s_state.clock_trusted || !meltdown_date_valid(&s_state.today)) return;
    (void)commit_new_month();
}

static void service_network(void)
{
    const int64_t now = esp_timer_get_time();
    if (!s_state.clock_trusted) {
        if (!s_net_started && meltdown_network_start() == ESP_OK) s_net_started = true;
        if (s_net_started && !meltdown_network_provisioning() && now >= s_provision_retry_us) {
            const bool credentials = meltdown_network_has_credentials();
            const bool waited = now - s_boot_us > PROVISION_WAIT_US;
            if (!credentials || (!meltdown_network_has_ip() && waited)) {
                if (meltdown_network_provision_start() != ESP_OK) {
                    s_sync_line = "校时失败";
                    s_provision_retry_us = now + 5000000;
                }
            }
        }
        const char *line = "正在校时";
        if (!s_net_started) line = "校时失败";
        else if (meltdown_network_provisioning()) line = "蓝牙配网";
        else if (!meltdown_network_has_credentials()) line = "请打开小程序";
        else if (now - s_boot_us > SYNC_FAIL_US && !meltdown_network_clock_synced()) line = "校时失败";
        if (strcmp(s_sync_line, line) != 0) {
            s_sync_line = line;
            s_ui_dirty = true;
        }
        return;
    }

    if (s_state.page == MELTDOWN_PAGE_SYNC) {
        if (s_state.rollback && strcmp(s_sync_line, "") != 0) {
            s_sync_line = "";
            s_ui_dirty = true;
        }
        /* UP-long on this page may have started provisioning. Do not stop it. */
        if (!meltdown_network_provisioning()) meltdown_network_maintain();
        return;
    }
    if (meltdown_network_provisioning()) (void)meltdown_network_provision_stop();
    meltdown_network_maintain();
}

static void poll_clock(void)
{
    if (!s_state.clock_trusted && !meltdown_network_clock_synced()) return;
    time_t now = 0;
    time(&now);
    meltdown_date_t today;
    if (!meltdown_date_from_unix((int64_t)now, &today)) return;
    const bool same = s_state.clock_trusted && s_state.today.year == today.year &&
                      s_state.today.month == today.month && s_state.today.day == today.day;
    if (same) return;
    meltdown_effect_t effect;
    meltdown_on_clock(&s_state, &today, &effect);
    if (effect.needs_rollover) {
        if (!commit_new_month()) s_rollover_pending = true;
    }
    s_ui_dirty = true;
}

static void service_backlight(bool activity)
{
    const int64_t now = esp_timer_get_time();
    if (activity) {
        s_idle_us = now;
        if (s_blanked) {
            bsp_display_backlight(100);
            s_blanked = false;
        }
        return;
    }
    if (!s_blanked && now - s_idle_us > IDLE_BLANK_US) {
        bsp_display_backlight(0);
        s_blanked = true;
    }
}

static void poll_battery(void)
{
    const int64_t now = esp_timer_get_time();
    if (!s_battery_ready) {
        s_battery_ready = true;
        if (bsp_battery_init() != ESP_OK) {
            ESP_LOGW(TAG, "battery gauge unavailable");
            s_battery_soc = -1;
        } else {
            s_battery_soc = bsp_battery_soc();
        }
        s_battery_us = now;
        s_ui_dirty = true;
        return;
    }
    if (now - s_battery_us < BATTERY_POLL_US) return;
    s_battery_us = now;
    const int soc = bsp_battery_soc();
    if (soc == s_battery_soc) return;
    s_battery_soc = soc;
    s_ui_dirty = true;
}

static void refresh_ui(void)
{
    if (!s_ui_dirty) return;
    if (!bsp_lvgl_lock(200)) return;
    meltdown_ui_refresh(&s_state, s_sync_line, s_save_failed, s_battery_soc);
    bsp_lvgl_unlock();
    s_ui_dirty = false;
}

static void app_task(void *arg)
{
    (void)arg;
    for (;;) {
        button_event_t event;
        bool activity = false;
        while (xQueueReceive(s_queue, &event, 0) == pdTRUE) {
            activity = true;
            /* A press while the backlight is off only wakes the screen. */
            if (s_blanked) continue;
            meltdown_input_t input;
            const int count = map_button(&event, &input);
            for (int i = 0; i < count; i++) handle_input(input, true);
        }
        /* Paint the new count before the two NVS commits. The lock is not held across flash. */
        refresh_ui();
        service_network();
        poll_clock();
        if (s_record_dirty) save_record();
        retry_rollover();
        if (s_state.mute_flash && esp_timer_get_time() > s_mute_until_us) {
            s_state.mute_flash = false;
            s_ui_dirty = true;
        }
        service_backlight(activity);
        poll_battery();
        refresh_ui();
        vTaskDelay(pdMS_TO_TICKS(LOOP_DELAY_MS));
    }
}

esp_err_t meltdown_app_start(void)
{
    s_boot_us = esp_timer_get_time();
    s_idle_us = s_boot_us;
    meltdown_state_init(&s_state);

    esp_err_t err = meltdown_store_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS init failed: %s", esp_err_to_name(err));
        s_save_failed = true;
    } else {
        meltdown_record_t loaded;
        err = meltdown_store_load(&loaded);
        if (err == ESP_OK) s_state.record = loaded;
        else if (err != ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "NVS load failed: %s", esp_err_to_name(err));
            s_save_failed = true;
        }
    }
    /* A stored month is not a trusted clock. The sync page stays up until SNTP. */

    bsp_i2c_init();
    if (bsp_display_init() != ESP_OK || !bsp_lvgl_init()) {
        ESP_LOGE(TAG, "display init failed");
        return ESP_FAIL;
    }
    bsp_display_backlight(100);
    if (!bsp_lvgl_lock(1000)) return ESP_ERR_TIMEOUT;
    meltdown_ui_create();
    meltdown_ui_refresh(&s_state, s_sync_line, s_save_failed, s_battery_soc);
    bsp_lvgl_unlock();
    s_ui_dirty = false;
    ESP_LOGI(TAG, "heap after UI free=%u largest=%u",
             (unsigned)esp_get_free_heap_size(),
             (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));

    err = meltdown_audio_start();
    if (err != ESP_OK) ESP_LOGE(TAG, "audio init failed: %s", esp_err_to_name(err));

    s_queue = xQueueCreate(INPUT_QUEUE_DEPTH, sizeof(button_event_t));
    if (!s_queue) return ESP_ERR_NO_MEM;
    err = bsp_button_init(on_button, NULL);
    if (err != ESP_OK) ESP_LOGE(TAG, "button init failed: %s", esp_err_to_name(err));

    if (xTaskCreate(app_task, "melt_app", APP_TASK_STACK, NULL, 5, NULL) != pdPASS) {
        return ESP_ERR_NO_MEM;
    }
    return ESP_OK;
}
