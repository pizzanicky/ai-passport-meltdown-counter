#pragma once

#include "esp_err.h"
#include <stdbool.h>

/* Wi-Fi is started only while the clock is untrusted. BLUFI is the official
 * provisioning path and advertises as BLUFI_FoloPassport. Passwords are never logged.
 */

esp_err_t meltdown_network_start(void);
esp_err_t meltdown_network_provision_start(void);
esp_err_t meltdown_network_provision_stop(void);
esp_err_t meltdown_network_clear_credentials(void);
bool meltdown_network_has_credentials(void);
bool meltdown_network_has_ip(void);
bool meltdown_network_provisioning(void);
bool meltdown_network_clock_synced(void);
void meltdown_network_maintain(void);
