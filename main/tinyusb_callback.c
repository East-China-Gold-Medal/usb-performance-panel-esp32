/* @file tinyusb_driver.c

    USB port driver reported directly to TinyUSB.
    SPDX-License-Identifier: WTFPL

*/

#include "common.h"
#include <device/usbd_pvt.h>

extern void set_pwm (uint8_t channel,uint8_t value);

bool tud_vendor_control_xfer_cb (uint8_t rhport, uint8_t stage, tusb_control_request_t const * request)
{
    if (stage != CONTROL_STAGE_SETUP) return true;
    uint8_t channel = (request->wValue & 0xFF00)>>16;
    uint8_t value = request->wValue & 0xFF;
    host_operation_command_t command = (host_operation_command_t)request->bRequest;
    switch (command) {
        case COMMAND_QUERY_CAP: {
            const uint8_t channel_count = VOLTEMETER_CHANNEL_COUNT | VOLTMETER_CHANNEL_REPORT_RAW_VALUE;
            return tud_control_xfer(rhport, request, (void*)&channel_count, 1);
        }
        case COMMAND_SET_USAGE: {
            set_pwm(channel,value);
            return true;
        }
        case VENDOR_REQUEST_MICROSOFT_2_0: {
          if ( request->wIndex == 7 ) {
            // Get Microsoft OS 2.0 compatible descriptor
            uint16_t total_len;
            memcpy(&total_len, desc_ms_os_20+8, 2);
            return tud_control_xfer(rhport, request, (void*)(uintptr_t) desc_ms_os_20, total_len);
          }
          else {
            return false;
          }
        }
        case VENDOR_REQUEST_MICROSOFT_1_0: {
            if ( request->wIndex == 4 ) {
                // Get Microsoft OS 1.0 compatible descriptor header
                return tud_control_xfer(rhport, request, (void*)desc_ms_os_10_header, 40);
            }
            else if (request->wIndex == 5) {
                return tud_control_xfer(rhport, request, (void*)desc_ms_os_10_detail, 142);
            }
            else {
                return false;
            }
        }
    }
    return false;
}

void usb_performance_panel_init (void)
{
    // Do nothing.
}

void usb_performance_panel_reset (uint8_t port)
{
    // Clear all values.
    for(int i=0;i<VOLTEMETER_CHANNEL_COUNT;i++) {
        set_pwm(voltmeter_channels[i],0);
    }
}

uint16_t usb_performance_panel_open (uint8_t rhport, tusb_desc_interface_t const * desc_intf, uint16_t max_len)
{
    uint16_t drv_len = tu_desc_len(desc_intf);
    // We don't have special handoff procedures for our device.

    return drv_len;
}

bool usb_performance_panel_xfer_cb (uint8_t rhport, uint8_t ep_addr, xfer_result_t result, uint32_t xferred_bytes)
{
    // Nothing to do: we only have EP 0 Control.
    return false;
}

usbd_class_driver_t performance_panel_driver = {
#if CFG_TUSB_DEBUG >= 2
        .name= "PerformancePanel",
#endif
        .init = usb_performance_panel_init,
        .reset = usb_performance_panel_reset,
        .open = usb_performance_panel_open,
        .control_xfer_cb = tud_vendor_control_xfer_cb,
        .xfer_cb = usb_performance_panel_xfer_cb,
        .sof = NULL
};

usbd_class_driver_t const* usbd_app_driver_get_cb(uint8_t* driver_count)
{
    *driver_count = 1;
    return &performance_panel_driver;
}

uint8_t const * tud_descriptor_bos_cb(void)
{
    return desc_bos;
}

// Overriden function from managed_components/espressif__esp_tinyusb/descriptors_control.c

#define MAX_DESC_BUF_SIZE 32
#define USB_STRING_DESCRIPTOR_ARRAY_SIZE 8

uint16_t const *__wrap_tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void) langid; // Unused, this driver supports only one language in string descriptors
    uint8_t chr_count;
    static uint16_t _desc_str[MAX_DESC_BUF_SIZE];

    if (index == 0) {
        memcpy(&_desc_str[1], usb_performance_panel_string_desc[0], 2);
        chr_count = 1;
    } 
    else {
        if (index == 0xEE) {
            return (uint16_t*)desc_ms_os_10_str;
        }
        else {
            if (index >= USB_STRING_DESCRIPTOR_ARRAY_SIZE) {
                return NULL;
            }

            if (usb_performance_panel_string_desc[index] == NULL) {
                return NULL;
            }

            const char *str = usb_performance_panel_string_desc[index];
            chr_count = strnlen(str, MAX_DESC_BUF_SIZE - 1); // Buffer len - header

            // Convert ASCII string into UTF-16
            for (uint8_t i = 0; i < chr_count; i++) {
                _desc_str[1 + i] = str[i];
            }
        }
    }

    // First byte is length in bytes (including header), second byte is descriptor type (TUSB_DESC_STRING)
    _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2 * chr_count + 2);

    return _desc_str;
}
