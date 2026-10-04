#include "meltdown_audio.h"

#include "bsp_audio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stddef.h>
#include <stdint.h>

extern const uint8_t meltdown_c3_pcm[];
extern const size_t meltdown_c3_pcm_len;
extern const uint32_t meltdown_c3_pcm_rate;
extern const uint8_t meltdown_egg_pcm[];
extern const size_t meltdown_egg_pcm_len;
extern const uint32_t meltdown_egg_pcm_rate;

#define AUDIO_CHUNK_BYTES 512
#define AUDIO_VOLUME 70

static TaskHandle_t s_task;
static volatile uint32_t s_generation;
static volatile meltdown_sound_t s_requested;
static bool s_ready;

static void play_clip(const uint8_t *pcm, size_t length, uint32_t rate, uint32_t generation)
{
    if (!pcm || length < 2 || rate == 0) return;
    if (bsp_audio_set_format(rate, 16, 1) != ESP_OK) return;
    bsp_audio_set_volume(AUDIO_VOLUME);
    size_t offset = 0;
    while (offset + 1 < length && generation == s_generation) {
        size_t chunk = length - offset;
        if (chunk > AUDIO_CHUNK_BYTES) chunk = AUDIO_CHUNK_BYTES;
        chunk &= ~(size_t)1u;
        if (chunk == 0) break;
        if (bsp_audio_write(pcm + offset, chunk) != ESP_OK) break;
        offset += chunk;
    }
}

static void audio_task(void *arg)
{
    (void)arg;
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        for (;;) {
            const uint32_t generation = s_generation;
            const meltdown_sound_t sound = s_requested;
            if (sound == MELTDOWN_SOUND_C3) {
                play_clip(meltdown_c3_pcm, meltdown_c3_pcm_len, meltdown_c3_pcm_rate, generation);
            } else if (sound == MELTDOWN_SOUND_EGG) {
                play_clip(meltdown_egg_pcm, meltdown_egg_pcm_len, meltdown_egg_pcm_rate, generation);
            }
            if (generation == s_generation) break;
        }
    }
}

esp_err_t meltdown_audio_start(void)
{
    if (s_ready) return ESP_OK;
    esp_err_t err = bsp_audio_init();
    if (err != ESP_OK) return err;
    if (xTaskCreate(audio_task, "melt_audio", 4096, NULL, 4, &s_task) != pdPASS) {
        return ESP_ERR_NO_MEM;
    }
    s_ready = true;
    return ESP_OK;
}

void meltdown_audio_play(meltdown_sound_t sound)
{
    if (!s_ready || sound == MELTDOWN_SOUND_NONE) {
        meltdown_audio_stop();
        return;
    }
    s_requested = sound;
    s_generation++;
    xTaskNotifyGive(s_task);
}

void meltdown_audio_stop(void)
{
    s_requested = MELTDOWN_SOUND_NONE;
    s_generation++;
    if (s_task) xTaskNotifyGive(s_task);
}
