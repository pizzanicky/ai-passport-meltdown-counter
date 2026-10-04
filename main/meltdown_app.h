#pragma once

#include "esp_err.h"

/* Starts the meltdown counter and does not return after the app task is created.
 * A display failure returns an error and leaves the demo menu unentered.
 */
esp_err_t meltdown_app_start(void);
