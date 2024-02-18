/* @file monitor_main.c

    The driver's main entry.
    SPDX-License-Identifier: WTFPL

*/

#include "common.h"

void app_main(void)
{
    ESP_ERROR_CHECK(tinyusb_driver_install(&panel_usb_config));

    while (1) {
        if (tud_mounted()) {
            // Loop here.
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
