#pragma once

#include "meltdown_model.h"

#include "esp_err.h"

/* Playback runs on its own task. play() only queues a replacement clip. */

esp_err_t meltdown_audio_start(void);
void meltdown_audio_play(meltdown_sound_t sound);
void meltdown_audio_stop(void);
