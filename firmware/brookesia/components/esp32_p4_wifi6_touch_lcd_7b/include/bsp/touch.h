/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief BSP Touchscreen
 *
 * This file offers API for basic touchscreen initialization.
 * It is useful for users who want to use the touchscreen without the default Graphical Library LVGL.
 *
 * For standard LCD initialization with LVGL graphical library, you can call all-in-one function bsp_display_start().
 */

#pragma once
#include "esp_lcd_touch.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief BSP touch configuration structure
 *
 */
typedef struct {
    struct {
        unsigned int swap_xy: 1;   /*!< Swap X and Y after reading coordinates */
        unsigned int mirror_x: 1;  /*!< Mirror X after reading coordinates */
        unsigned int mirror_y: 1;  /*!< Mirror Y after reading coordinates */
    } flags;
} bsp_touch_config_t;

/**
 * @brief Get or create the board-owned touchscreen
 *
 * The BSP retains the GT911 and its I2C panel-IO handle for the display lifetime.
 * Repeated calls return the same handle; callers must not delete it. The first
 * successful call determines the coordinate transformation flags.
 *
 * @param[in]  config    touch configuration
 * @param[out] ret_touch esp_lcd_touch touchscreen handle
 * @return
 *      - ESP_OK         On success
 *      - Else           esp_lcd_touch failure
 */
esp_err_t bsp_touch_new(const bsp_touch_config_t *config, esp_lcd_touch_handle_t *ret_touch);

/**
 * @brief Borrow the display-owned touchscreen handle
 *
 * Direct readers may use this handle only while the LVGL adapter is paused.
 * The BSP retains ownership and callers must not delete the handle.
 *
 * @param[out] ret_touch esp_lcd_touch touchscreen handle
 * @return
 *      - ESP_OK         On success
 *      - ESP_ERR_INVALID_STATE before the board touchscreen is initialized
 *      - Else           ESP-IDF failure
 */
esp_err_t bsp_touch_get_shared(esp_lcd_touch_handle_t *ret_touch);

#ifdef __cplusplus
}
#endif
