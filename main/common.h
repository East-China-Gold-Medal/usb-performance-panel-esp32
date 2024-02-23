/* @file common.h

    Several common headers & definitions required by this project.
    SPDX-License-Identifier: WTFPL

*/

#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <tinyusb.h>

#define VOLTEMETER_CHANNEL_COUNT ((uint8_t)16)

// Microsoft OS 2.0 Descriptor

#define MS_OS_20_DESC_LEN  0xB2
#define BOS_TOTAL_LEN      (TUD_BOS_DESC_LEN + TUD_BOS_MICROSOFT_OS_DESC_LEN)

// Microsoft OS 1.0 Descriptor
#define MS_OS_DESC_STRING_INDEX		0xEE
#define MS_OS_DESC_STRING_LENGTH	0x12
#define MS_OS_DESC_VENDOR_CODE_OFFSET	0x10

// USB VID for vendor "East China Gold Medal".
#define VID 0x0cbc

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

typedef const enum {
    VOLTMETER_CHANNEL_REPORT_BITMAP=0,
    VOLTMETER_CHANNEL_REPORT_RAW_VALUE=0x80
} voltmeter_channel_report_method_t;

// USB PID for currently supported products.
typedef const enum {
    PID_CBC = 0x0cbc, // CBC's USB performance panel
    PID_HZ  = 0xcbc0  // HZ's "Enhanced" USB performance panel
} panel_pid_t;

typedef enum {
    COMMAND_QUERY_CAP = 0xFF,
    COMMAND_SET_USAGE = 0xFE,
    VENDOR_REQUEST_MICROSOFT_2_0 = 0x2,
    VENDOR_REQUEST_MICROSOFT_1_0 = 0x1
} host_operation_command_t;

extern const tinyusb_config_t panel_usb_config;
extern const voltmeter_channel_t voltmeter_channels[VOLTEMETER_CHANNEL_COUNT];
extern const uint16_t *usb_performance_panel_string_desc[];
extern const int usb_performance_panel_string_desc_size;
extern const off_t usb_performance_panel_string_chinese_offset;
extern const uint8_t desc_ms_os_20[];
extern const uint8_t desc_ms_os_10_str[];
extern const uint8_t desc_bos[];
extern const uint8_t desc_ms_os_10_header[];
extern const uint8_t desc_ms_os_10_detail[];

#endif
