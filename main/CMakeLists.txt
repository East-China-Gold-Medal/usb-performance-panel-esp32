# @file port_descriptor.c
#
#   CMake file list for compilation.
#   SPDX-License-Identifier: WTFPL

idf_component_register(
    SRC_DIRS "."
    INCLUDE_DIRS "."
    REQUIRES driver tinyusb)
