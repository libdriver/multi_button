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
 * @file      driver_multi_button_basic.c
 * @brief     driver multi_button basic source file
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

#include "driver_multi_button_basic.h"

static multi_button_handle_t gs_handle;        /**< multi_button handle */

/**
 * @brief     basic example init
 * @param[in] row is the max row of buttons
 * @param[in] col is the max col of buttons
 * @param[in] *callback points to an irq callback address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t multi_button_basic_init(uint8_t row, uint8_t col, void (*callback)(uint16_t row, uint16_t col, multi_button_t *data))
{
    uint8_t res;
    
    /* link interface function */
    DRIVER_MULTI_BUTTON_LINK_INIT(&gs_handle, multi_button_handle_t);
    DRIVER_MULTI_BUTTON_LINK_MATRIX_INIT(&gs_handle, multi_button_interface_matrix_init);
    DRIVER_MULTI_BUTTON_LINK_MATRIX_DEINIT(&gs_handle, multi_button_interface_matrix_deinit);
    DRIVER_MULTI_BUTTON_LINK_MATRIX_WRITE_ROW(&gs_handle, multi_button_interface_matrix_write_row);
    DRIVER_MULTI_BUTTON_LINK_MATRIX_READ_ROW(&gs_handle, multi_button_interface_matrix_read_row);
    DRIVER_MULTI_BUTTON_LINK_TIMESTAMP_READ(&gs_handle, multi_button_interface_timestamp_read);
    DRIVER_MULTI_BUTTON_LINK_DELAY_MS(&gs_handle, multi_button_interface_delay_ms);
    DRIVER_MULTI_BUTTON_LINK_DEBUG_PRINT(&gs_handle, multi_button_interface_debug_print);
    DRIVER_MULTI_BUTTON_LINK_RECEIVE_CALLBACK(&gs_handle, callback);
    
    /* init */
    res = multi_button_init(&gs_handle, row, col);
    if (res != 0)
    {
        multi_button_interface_debug_print("multi_button: init failed.\n");
       
        return 1;
    }
    
    /* set default timeout */
    res = multi_button_set_timeout(&gs_handle, MULTI_BUTTON_BASIC_DEFAULT_TIMEOUT);
    if (res != 0)
    {
        multi_button_interface_debug_print("multi_button: set timeout failed.\n");
        (void)multi_button_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default interval */
    res = multi_button_set_interval(&gs_handle, MULTI_BUTTON_BASIC_DEFAULT_INTERVAL);
    if (res != 0)
    {
        multi_button_interface_debug_print("multi_button: set interval failed.\n");
        (void)multi_button_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default short time */
    res = multi_button_set_short_time(&gs_handle, MULTI_BUTTON_BASIC_DEFAULT_SHORT_TIME);
    if (res != 0)
    {
        multi_button_interface_debug_print("multi_button: set short time failed.\n");
        (void)multi_button_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default long time */
    res = multi_button_set_long_time(&gs_handle, MULTI_BUTTON_BASIC_DEFAULT_LONG_TIME);
    if (res != 0)
    {
        multi_button_interface_debug_print("multi_button: set long time failed.\n");
        (void)multi_button_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default repeat time */
    res = multi_button_set_repeat_time(&gs_handle, MULTI_BUTTON_BASIC_DEFAULT_REPEAT_TIME);
    if (res != 0)
    {
        multi_button_interface_debug_print("multi_button: set repeat time failed.\n");
        (void)multi_button_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default repeat cnt */
    res = multi_button_set_repeat_cnt(&gs_handle, MULTI_BUTTON_BASIC_DEFAULT_REPEAT_CNT);
    if (res != 0)
    {
        multi_button_interface_debug_print("multi_button: set repeat cnt failed.\n");
        (void)multi_button_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default set period */
    res = multi_button_set_period(&gs_handle, MULTI_BUTTON_BASIC_DEFAULT_PERIOD);
    if (res != 0)
    {
        multi_button_interface_debug_print("multi_button: set period failed.\n");
        (void)multi_button_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t multi_button_basic_deinit(void)
{
    if (multi_button_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example process
 * @note   none
 */
void multi_button_basic_process(void)
{
    /* process */
    (void)multi_button_process(&gs_handle);
}
