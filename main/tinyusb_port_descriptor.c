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

const uint8_t desc_ms_os_20[] =
{
    // Set header: length, type, windows version, total length
    U16_TO_U8S_LE(0x000A), U16_TO_U8S_LE(MS_OS_20_SET_HEADER_DESCRIPTOR), U32_TO_U8S_LE(0x06030000), U16_TO_U8S_LE(MS_OS_20_DESC_LEN),

    // Configuration subset header: length, type, configuration index, reserved, configuration total length
    U16_TO_U8S_LE(0x0008), U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_CONFIGURATION), 0, 0, U16_TO_U8S_LE(MS_OS_20_DESC_LEN-0x0A),

    // Function Subset header: length, type, first interface, reserved, subset length
    U16_TO_U8S_LE(0x0008), U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_FUNCTION), 0, 0, U16_TO_U8S_LE(MS_OS_20_DESC_LEN-0x0A-0x08),

    // MS OS 2.0 Compatible ID descriptor: length, type, compatible ID, sub compatible ID
    U16_TO_U8S_LE(0x0014), U16_TO_U8S_LE(MS_OS_20_FEATURE_COMPATBLE_ID), 'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // sub-compatible

    // MS OS 2.0 Registry property descriptor: length, type
    U16_TO_U8S_LE(MS_OS_20_DESC_LEN-0x0A-0x08-0x08-0x14), U16_TO_U8S_LE(MS_OS_20_FEATURE_REG_PROPERTY),
    U16_TO_U8S_LE(0x0007), U16_TO_U8S_LE(0x002A), // wPropertyDataType, wPropertyNameLength and PropertyName "DeviceInterfaceGUIDs\0" in UTF-16
    'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00, 't', 0x00, 'e', 0x00,
    'r', 0x00, 'f', 0x00, 'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00, 0x00, 0x00,
    U16_TO_U8S_LE(0x0050), // wPropertyDataLength
    //bPropertyData: “{975F44D9-0D08-43FD-8B3E-127CA8AFFF9D}”.
    '{', 0x00, '9', 0x00, '7', 0x00, '5', 0x00, 'F', 0x00, '4', 0x00, '4', 0x00, 'D', 0x00, '9', 0x00, '-', 0x00,
    '0', 0x00, 'D', 0x00, '0', 0x00, '8', 0x00, '-', 0x00, '4', 0x00, '3', 0x00, 'F', 0x00, 'D', 0x00, '-', 0x00,
    '8', 0x00, 'B', 0x00, '3', 0x00, 'E', 0x00, '-', 0x00, '1', 0x00, '2', 0x00, '7', 0x00, 'C', 0x00, 'A', 0x00,
    '8', 0x00, 'A', 0x00, 'F', 0x00, 'F', 0x00, 'F', 0x00, '9', 0x00, 'D', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00
};

TU_VERIFY_STATIC(sizeof(desc_ms_os_20) == MS_OS_20_DESC_LEN, "Incorrect size");

uint8_t const desc_bos[] = {
    /* BOS Descriptor */
    0x05,                       // Descriptor size
    TUSB_DESC_BOS,              // Device descriptor type BOS
    0x21, 0x00,                 // Length 0x21 (33) this and all sub descriptors
    0x01,                       // Number of device capability descriptors

    /* Platform Device Capability Descriptor */
    0x1C,                                   // 28 bytes bLength
    TUSB_DESC_DEVICE_CAPABILITY,            // Platform Descriptor type
    DEVICE_CAPABILITY_PLATFORM,             // bDevCapabilityType PLATFORM
    0,                                      // bReserved
    0xD8, 0xDD, 0x60, 0xDF,                 // PlatformCapabilityUUID
    0x45, 0x89,                             // MS OS2.0 Descriptor
    0x4C, 0xC7,                             // D8DD60DF-4589-4CC7-9CD2-659D9E648A9F
    0x9C, 0xD2, 0x65, 0x9D, 0x9E, 0x64, 0x8A, 0x9F,
                                            // CapabilityData
    0x00, 0x00, 0x03, 0x06,                 // dwWindowsVersion for Windows 10 and later
    MS_OS_20_DESC_LEN, 0x00,                // wLength 0xB2
    VENDOR_REQUEST_MICROSOFT,               // bMS_VendorCode - any value. e.g. 0x01
    0x00                                    // bAltEnumCmd 0
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
        case VENDOR_REQUEST_MICROSOFT: {
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