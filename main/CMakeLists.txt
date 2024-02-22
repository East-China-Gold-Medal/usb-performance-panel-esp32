# @file port_descriptor.c
#
#   CMake file list for compilation.
#   SPDX-License-Identifier: WTFPL

idf_component_register(
    SRCS "tinyusb_callback.c" "monitor_main.c" "tinyusb_port_descriptor.c"
    INCLUDE_DIRS "."
    WHOLE_ARCHIVE
    REQUIRES driver tinyusb)

target_link_libraries(${COMPONENT_LIB} INTERFACE "-Wl,--wrap=tud_descriptor_string_cb")
