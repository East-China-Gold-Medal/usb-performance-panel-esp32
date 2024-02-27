/* @file monitor_main.c

    The driver's main entry.
    SPDX-License-Identifier: WTFPL

*/

#include "common.h"
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define LEDC_FREQUENCY          (2000) // Frequency in Hertz.

static gpio_config_t led_config = {
    .pin_bit_mask = BIT15,
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE
};

void set_pwm (uint8_t channel,uint8_t value)
{
#ifndef LEDC_HIGH_SPEED_MODE
    if(ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel, value)!=ESP_OK) {
#else
    if(ledc_set_duty(channel&1, (ledc_channel_t)channel>>1, value)!=ESP_OK) {
#endif
        gpio_set_level(15,1);
    }

#ifndef LEDC_HIGH_SPEED_MODE
    if(ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel)!=ESP_OK) {
#else
    if(ledc_update_duty(channel&1, (ledc_channel_t)channel>>1)!=ESP_OK) {
#endif
        gpio_set_level(15,1);
    }
}

void app_main (void)
{
    gpio_config(&led_config);

    ESP_ERROR_CHECK(tinyusb_driver_install(&panel_usb_config));

    for(int i=0;i<VOLTEMETER_CHANNEL_COUNT;i++) {
        ledc_timer_config_t ledc_timer = {
#ifdef LEDC_HIGH_SPEED_MODE
            .speed_mode       = i&1,
#else
            .speed_mode       = LEDC_LOW_SPEED_MODE,
#endif
            .duty_resolution  = LEDC_TIMER_8_BIT,
#ifdef LEDC_HIGH_SPEED_MODE
            .timer_num        = i >> 2,
#else
            .timer_num        = i >> 1,
#endif
            .freq_hz          = LEDC_FREQUENCY,
            .clk_cfg          = LEDC_REF_TICK
        };
        if(ledc_timer_config(&ledc_timer)!=ESP_OK) {
            gpio_set_level(15,1);
        }
#ifdef LEDC_HIGH_SPEED_MODE
        if(ledc_set_pin(voltmeter_channels[i], i&1, (ledc_channel_t)(i>>1))!=ESP_OK) {
#else
        if(ledc_set_pin(voltmeter_channels[i], LEDC_LOW_SPEED_MODE, (ledc_channel_t)i)!=ESP_OK) {
#endif
            gpio_set_level(15,1);
        }
    }

    while (1) {
        if (tud_mounted()) {
            // Loop here.
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

const voltmeter_channel_t voltmeter_channels[VOLTEMETER_CHANNEL_COUNT] = {
    VOLTMETER_CHANNEL_0,
    VOLTMETER_CHANNEL_1,
    VOLTMETER_CHANNEL_2,
    VOLTMETER_CHANNEL_3,
    VOLTMETER_CHANNEL_4,
    VOLTMETER_CHANNEL_5,
    VOLTMETER_CHANNEL_6,
    VOLTMETER_CHANNEL_7,
#ifdef LEDC_HIGH_SPEED_MODE
    VOLTMETER_CHANNEL_8,
    VOLTMETER_CHANNEL_9,
    VOLTMETER_CHANNEL_10,
    VOLTMETER_CHANNEL_11,
    VOLTMETER_CHANNEL_12,
    VOLTMETER_CHANNEL_13,
    VOLTMETER_CHANNEL_14,
    VOLTMETER_CHANNEL_15
#endif
};
