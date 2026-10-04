#pragma once

#include "meltdown_model.h"

#include <stdbool.h>

/* Call only while holding bsp_lvgl_lock(). */

void meltdown_ui_create(void);
void meltdown_ui_refresh(const meltdown_state_t *state, const char *sync_line, bool save_failed, int battery_soc);
