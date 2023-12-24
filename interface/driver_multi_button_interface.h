/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_multi_button_interface.h
 * @brief     driver multi_button interface header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2023-11-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/11/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_MULTI_BUTTON_INTERFACE_H
#define DRIVER_MULTI_BUTTON_INTERFACE_H

#include "driver_multi_button.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup multi_button_interface_driver multi_button interface driver function
 * @brief    multi_button interface driver modules
 * @ingroup  multi_button_driver
 * @{
 */

/**
 * @brief  interface matrix init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t multi_button_interface_matrix_init(void);

/**
 * @brief  interface matrix deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t multi_button_interface_matrix_deinit(void);

/**
 * @brief     interface matrix write row
 * @param[in] num is the row number
 * @param[in] level is the set level
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t multi_button_interface_matrix_write_row(uint16_t num, uint8_t level);

/**
 * @brief      interface matrix read row
 * @param[out] *col_array points to a col array buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t multi_button_interface_matrix_read_row(uint32_t *col_array);

/**
 * @brief     interface timestamp read
 * @param[in] *t points to a multi_button_time structure
 * @return    status code
 *            - 0 success
 *            - 1 read failed
 * @note      none
 */
uint8_t multi_button_interface_timestamp_read(multi_button_time_t *t);

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void multi_button_interface_delay_ms(uint32_t ms);

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void multi_button_interface_debug_print(const char *const fmt, ...);

/**
 * @brief     interface receive callback
 * @param[in] row is the requested row
 * @param[in] col is the requested col
 * @param[in] *data points to an multi_button_t structure
 * @note      none
 */
void multi_button_interface_receive_callback(uint16_t row, uint16_t col, multi_button_t *data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
