/* @file tinyusb_port_descriptor.c

    USB port descriptor to be reported to OS.
    SPDX-License-Identifier: WTFPL

*/

#include "common.h"
#include <device/usbd_pvt.h>

const uint16_t *usb_performance_panel_string_desc[] = {
    u"\u0306" u"\u0409\u0804",                       // 0: is supported language is English (0x0409) / Chinese simplified (Chinese (PRC))
    u"\u032C" u"East China Gold Medal",              // 1: Manufacturer
    u"\u033E" u"Enhanced USB Performance Panel",     // 2: Product
    u"\u031E" u"CustomizedByHZ",                     // 3: Serials will use unique ID if possible

    u"\u030A" u"华东金牌",                               // 4: Manufacturer
    u"\u0316" u"增强版USB性能面板",                         // 5: Product
    u"\u030C" u"由hz定制",                              // 6: Serials will use unique ID if possible    
};
const int usb_performance_panel_string_desc_size = sizeof(usb_performance_panel_string_desc) / sizeof(const uint16_t*);
const off_t usb_performance_panel_string_chinese_offset = 0x3; // Index starts from 1!

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
    .string_descriptor = (const char**)usb_performance_panel_string_desc,     // STUB: DOES NOT COMPATIABLE WITH TINYUSB STACK!
    .string_descriptor_count = usb_performance_panel_string_desc_size,
    .external_phy = false,
    .configuration_descriptor = usb_performance_panel_configuration_descriptor.bytes
};

const uint8_t desc_ms_os_10_str[] = {
	MS_OS_DESC_STRING_LENGTH,
	TUSB_DESC_STRING,
	'M', 0, 'S', 0, 'F', 0, 'T', 0, '1', 0, '0', 0, '0', 0, (uint8_t)VENDOR_REQUEST_MICROSOFT_1_0,0
};

const uint8_t desc_ms_os_10_header[] = {

    // WCID descriptor
    0x28, 0x00, 0x00, 0x00,   // dwLength
    0x00, 0x01,                                       // bcdVersion
    0x04, 0x00,                                       // wIndex
    0x01,                                             // bCount
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,         // bReserved_7

    // WCID function descriptor
    0x00,                                             // bFirstInterfaceNumber
    0x01,                                             // bReserved
    'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,         // cCID_8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // cSubCID_8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,               //bReserved_6
};

const uint8_t desc_ms_os_10_detail[] = {

  /// WCID property descriptor

    0x8e, 0x00, 0x00, 0x00,                           // dwLength
    0x00, 0x01,                                       // bcdVersion
    0x05, 0x00,                                       // wIndex
    0x01, 0x00,                                       // wCount

    /// registry propter descriptor

    0x84, 0x00, 0x00, 0x00,                           // dwSize
    0x01, 0x00, 0x00, 0x00,                           // dwPropertyDataType
    0x28, 0x00,                                       // wPropertyNameLength
    'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00,       // wcName_20
    'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00,       // wcName_20
    't', 0x00, 'e', 0x00, 'r', 0x00, 'f', 0x00,       // wcName_20
    'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00,       // wcName_20
    'U', 0x00, 'I', 0x00, 'D', 0x00, 0x00, 0x00,      // wcName_20
    0x4e, 0x00, 0x00, 0x00,                           // dwPropertyDataLength
    //c37cd10d-3934-49be-bb2a-318033d2187e
    '{', 0x00, 'C', 0x00, '3', 0x00, '7', 0x00,       // wcData_39
    'C', 0x00, 'D', 0x00, '1', 0x00, '0', 0x00,       // wcData_39
    'D', 0x00, '-', 0x00, '3', 0x00, '9', 0x00,       // wcData_39
    '3', 0x00, '4', 0x00, '-', 0x00, '4', 0x00,       // wcData_39
    '9', 0x00, 'B', 0x00, 'E', 0x00, '-', 0x00,       // wcData_39
    'B', 0x00, 'B', 0x00, '2', 0x00, 'A', 0x00,       // wcData_39
    '-', 0x00, '3', 0x00, '1', 0x00, '8', 0x00,       // wcData_39
    '0', 0x00, '3', 0x00, '3', 0x00, 'D', 0x00,       // wcData_39
    '2', 0x00, '1', 0x00, '8', 0x00, '7', 0x00,       // wcData_39
    'E', 0x00, '}', 0x00, 0x00, 0x00,                 // wcData_39
};
