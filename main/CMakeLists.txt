# @file port_descriptor.c
#
#   CMake file list for compilation.
#   SPDX-License-Identifier: WTFPL

idf_component_register(
    SRCS "port_descriptor.c" "monitor_main.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES driver
    )
