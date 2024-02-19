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

const union {
    const struct {
        tusb_desc_configuration_t configuration;
        tusb_desc_interface_t interface;
        tusb_desc_endpoint_t endpoint;
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
        .bNumEndpoints = 1,
        .bInterfaceClass = TUSB_CLASS_VENDOR_SPECIFIC,
        .bInterfaceSubClass = 0xFF,
        .bInterfaceProtocol = 0xFF,
        .iInterface = 2
    },

    .endpoint = {
        .bLength = sizeof(tusb_desc_endpoint_t),
        .bDescriptorType = TUSB_DESC_ENDPOINT,
        .bEndpointAddress = 1,
        .bmAttributes = {
            .xfer = TUSB_XFER_CONTROL
        },
        .wMaxPacketSize = 0x40,
        .bInterval = 0
    },
};

const tinyusb_config_t panel_usb_config = {
    .device_descriptor = &usb_performance_panel_device,
    .string_descriptor = usb_performance_panel_string_desc,
    .string_descriptor_count = sizeof(usb_performance_panel_string_desc) / sizeof(const char*),
    .external_phy = false,
    .configuration_descriptor = usb_performance_panel_configuration_descriptor.bytes
};