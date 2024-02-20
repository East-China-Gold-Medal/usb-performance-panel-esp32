/* @file port_descriptor.c

    USB port descriptor to be reported to OS.
    SPDX-License-Identifier: WTFPL

*/

#include "common.h"
#include <device/usbd_pvt.h>

const char *usb_performance_panel_string_desc[] =
{
  (const char[]) { 0x09, 0x04 },    // 0: is supported language is English (0x0409)
  "East China Gold Medal",          // 1: Manufacturer
  "Enhanced USB Performance Panel", // 2: Product
  "CustomizedByHZ",                 // 3: Serials will use unique ID if possible
};

tusb_desc_device_t usb_performance_panel_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,

    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = 0x0c,
    .bDeviceProtocol = 0xbc,
    .bMaxPacketSize0 = 0x40,
    .idVendor = VID,
    .idProduct = PID_HZ,
    .bcdDevice = 0x0100,
    .iManufacturer = 0x1,
    .iProduct = 0x2,
    .iSerialNumber = 0x3,
    .bNumConfigurations = 0x1
};

const union {
    const struct {
        tusb_desc_configuration_t configuration;
        tusb_desc_interface_t interface;
    } __attribute__((packed));
    uint8_t bytes[1];
} usb_performance_panel_configuration_descriptor = {

    .configuration = {
        .bLength = sizeof(tusb_desc_configuration_t),
        .bDescriptorType = TUSB_DESC_CONFIGURATION,
        .wTotalLength = sizeof(usb_performance_panel_configuration_descriptor),
        .bNumInterfaces = 1,
        .bConfigurationValue = 1,
        .iConfiguration = 1,
        .bmAttributes =  TU_BIT(7) | TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP,
        .bMaxPower = 200 / 2
    },

    .interface = {
        .bLength = sizeof(tusb_desc_interface_t),
        .bDescriptorType = TUSB_DESC_INTERFACE,
        .bInterfaceNumber = 0,
        .bAlternateSetting = 0,
        .bNumEndpoints = 0, // Only default Control endpoint.
        .bInterfaceClass = TUSB_CLASS_VENDOR_SPECIFIC,
        .bInterfaceSubClass = 0xFF,
        .bInterfaceProtocol = 0xFF,
        .iInterface = 2
    },

};

const tinyusb_config_t panel_usb_config = {
    .device_descriptor = &usb_performance_panel_device,
    .string_descriptor = usb_performance_panel_string_desc,
    .string_descriptor_count = sizeof(usb_performance_panel_string_desc) / sizeof(const char*),
    .external_phy = false,
    .configuration_descriptor = usb_performance_panel_configuration_descriptor.bytes
};

extern void set_pwm (uint8_t channel,uint8_t value);

bool tud_vendor_control_xfer_cb (uint8_t rhport, uint8_t stage, tusb_control_request_t const * request)
{
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
