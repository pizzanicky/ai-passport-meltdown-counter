#pragma once

#include "meltdown_model.h"

#include "esp_err.h"

/* NVS access stays on one task. Load may run once before that task starts. */

esp_err_t meltdown_store_init(void);
esp_err_t meltdown_store_load(meltdown_record_t *out);
esp_err_t meltdown_store_save(const meltdown_record_t *record);
