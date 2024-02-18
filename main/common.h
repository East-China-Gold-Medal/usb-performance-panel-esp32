/* @file common.h

    Several common headers & definitions required by this project.
    SPDX-License-Identifier: WTFPL

*/

#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <tinyusb.h>
#include <driver/gpio.h>

// Available GPIOs:
// 1 2 3 4 5 6 7 8 9 10 11 12 13 14 16 17 18 21 33 34 35 36 37 38 39 40
// Selects these GPIOs for now:
// 2 3 4 5 6 7 8 9 40 39 37 38 35 36 33 34
typedef const enum {
    VOLTMETER_CHANNEL_0=2,
    VOLTMETER_CHANNEL_1=3,
    VOLTMETER_CHANNEL_2=4,
    VOLTMETER_CHANNEL_3=5,
    VOLTMETER_CHANNEL_4=6,
    VOLTMETER_CHANNEL_5=7,
    VOLTMETER_CHANNEL_6=8,
    VOLTMETER_CHANNEL_7=9,
    VOLTMETER_CHANNEL_8=40,
    VOLTMETER_CHANNEL_9=39,
    VOLTMETER_CHANNEL_10=37,
    VOLTMETER_CHANNEL_11=38,
    VOLTMETER_CHANNEL_12=35,
    VOLTMETER_CHANNEL_13=36,
    VOLTMETER_CHANNEL_14=33,
    VOLTMETER_CHANNEL_15=34
} voltmeter_channel_t;

// USB PID for currently supported products.
typedef const enum {
    PID_CBC = 0x0cbc, // CBC's USB performance panel
    PID_HZ  = 0xcbc0  // HZ's "Enhanced" USB performance panel
} panel_pid_t;

// USB VID for vendor "East China Gold Medal".
#define VID 0x0cbc

extern const tinyusb_config_t panel_usb_config;

#endif