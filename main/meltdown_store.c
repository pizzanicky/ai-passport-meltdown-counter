#include "meltdown_store.h"

#include "nvs.h"
#include "nvs_flash.h"

#include <string.h>

static const char *NAMESPACE = "meltdown";

static esp_err_t open_namespace(nvs_open_mode_t mode, nvs_handle_t *handle)
{
    esp_err_t err = nvs_open(NAMESPACE, mode, handle);
    if (err == ESP_ERR_NVS_NOT_FOUND && mode == NVS_READONLY) return err;
    return err;
}

static void read_generation(nvs_handle_t handle, const char *key, uint32_t *value)
{
    if (nvs_get_u32(handle, key, value) != ESP_OK) *value = 0;
}

static esp_err_t read_slots(meltdown_slots_t *slots)
{
    memset(slots, 0, sizeof(*slots));
    nvs_handle_t handle;
    esp_err_t err = open_namespace(NVS_READONLY, &handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) return ESP_OK;
    if (err != ESP_OK) return err;

    uint8_t active = 0;
    if (nvs_get_u8(handle, "act", &active) == ESP_OK && active < 2) slots->active = active;
    for (int i = 0; i < 2; i++) {
        char blob_key[3] = {'b', (char)('0' + i), '\0'};
        char gen_key[3] = {'g', (char)('0' + i), '\0'};
        size_t length = MELTDOWN_BLOB_BYTES;
        if (nvs_get_blob(handle, blob_key, slots->blob[i], &length) == ESP_OK &&
            length == MELTDOWN_BLOB_BYTES) {
            slots->valid[i] = true;
        }
        read_generation(handle, gen_key, &slots->generation[i]);
    }
    nvs_close(handle);
    return ESP_OK;
}

esp_err_t meltdown_store_init(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        /* The partition itself is unreadable. Erasing it also drops Wi-Fi credentials. */
        err = nvs_flash_erase();
        if (err == ESP_OK) err = nvs_flash_init();
    }
    return err;
}

esp_err_t meltdown_store_load(meltdown_record_t *out)
{
    if (!out) return ESP_ERR_INVALID_ARG;
    meltdown_slots_t slots;
    esp_err_t err = read_slots(&slots);
    if (err != ESP_OK) return err;
    if (!meltdown_slots_load(&slots, out)) return ESP_ERR_NOT_FOUND;
    return ESP_OK;
}

esp_err_t meltdown_store_save(const meltdown_record_t *record)
{
    if (!record) return ESP_ERR_INVALID_ARG;
    meltdown_slots_t slots;
    esp_err_t err = read_slots(&slots);
    if (err != ESP_OK) return err;
    if (!meltdown_slots_stage(&slots, record)) return ESP_FAIL;

    nvs_handle_t handle;
    err = nvs_open(NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    /* stage() fills the inactive slot and leaves active pointing at the previous record. */
    const uint8_t inactive = (uint8_t)(slots.active ^ 1u);
    char blob_key[3] = {'b', (char)('0' + inactive), '\0'};
    char gen_key[3] = {'g', (char)('0' + inactive), '\0'};
    err = nvs_set_blob(handle, blob_key, slots.blob[inactive], MELTDOWN_BLOB_BYTES);
    if (err == ESP_OK) err = nvs_set_u32(handle, gen_key, slots.generation[inactive]);
    if (err == ESP_OK) err = nvs_commit(handle);
    if (err != ESP_OK) {
        nvs_close(handle);
        return err;
    }

    if (!meltdown_slots_publish(&slots)) {
        nvs_close(handle);
        return ESP_FAIL;
    }
    err = nvs_set_u8(handle, "act", slots.active);
    if (err == ESP_OK) err = nvs_commit(handle);
    nvs_close(handle);
    return err;
}
