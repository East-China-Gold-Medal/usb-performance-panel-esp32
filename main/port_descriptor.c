/* @file port_descriptor.c

    USB port descriptor to be reported to OS.
    SPDX-License-Identifier: WTFPL

*/

#include "common.h"

const char *usb_performance_panel_string_desc[] =
{
  (const char[]) { 0x09, 0x04 },    // 0: is supported language is English (0x0409)
  "East China Gold Medal",          // 1: Manufacturer
  "Enhanced USB Performance Panel", // 2: Product
  "CustomizedByHZ",                 // 3: Serials will use unique ID if possible
};

#if 0
typedef struct {
    union {
        const tusb_desc_device_t *device_descriptor; /*!< Pointer to a device descriptor. If set to NULL, the TinyUSB device will use a default device descriptor whose values are set in Kconfig */
        const tusb_desc_device_t *descriptor  __attribute__((deprecated)); /*!< Alias to `device_descriptor` for backward compatibility */
    };
    const char **string_descriptor;            /*!< Pointer to array of string descriptors. If set to NULL, TinyUSB device will use a default string descriptors whose values are set in Kconfig */
    int string_descriptor_count;               /*!< Number of descriptors in above array */
    bool external_phy;                         /*!< Should USB use an external PHY */
    const uint8_t *configuration_descriptor;   /*!< Pointer to a configuration descriptor. If set to NULL, TinyUSB device will use a default configuration descriptor whose values are set in Kconfig */
    bool self_powered;                         /*!< This is a self-powered USB device. USB VBUS must be monitored. */
    int vbus_monitor_io;                       /*!< GPIO for VBUS monitoring. Ignored if not self_powered. */
} tinyusb_config_t;
#endif

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

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + 1*TUD_HID_DESC_LEN)

tusb_desc_endpoint_t usb_performance_panel_endpoint0 =
  {
    .bLength          = sizeof(tusb_desc_endpoint_t),
    .bDescriptorType  = TUSB_DESC_ENDPOINT,
    .bEndpointAddress = 0,
    .bmAttributes     = { .xfer = TUSB_XFER_CONTROL },
    .wMaxPacketSize   = 64,
    .bInterval        = 0
  };

uint8_t const usb_performance_panel_configuration_descriptor[] = {
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 200),
    // The initialization macro DOES NOT provide a CONTROL method.
    7, TUSB_DESC_ENDPOINT, 0, TUSB_XFER_CONTROL, U16_TO_U8S_LE(TUD_OPT_HIGH_SPEED ? 512 : 64), 1,
};

const tinyusb_config_t panel_usb_config = {
    .device_descriptor = &usb_performance_panel_device,
    .string_descriptor = usb_performance_panel_string_desc,
    .string_descriptor_count = sizeof(usb_performance_panel_string_desc) / sizeof(const char*),
    .external_phy = false,
    .configuration_descriptor = usb_performance_panel_configuration_descriptor
};