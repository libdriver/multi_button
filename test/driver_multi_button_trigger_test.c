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
 * @file      driver_multi_button_trigger_test.c
 * @brief     driver multi_button trigger test source file
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

#include "driver_multi_button_trigger_test.h"

static multi_button_handle_t gs_handle;        /**< multi_button handle */
static volatile uint16_t gs_flag;              /**< flag */

/**
 * @brief     interface receive callback
 * @param[in] row is the requested row
 * @param[in] col is the requested col
 * @param[in] *data points to an multi_button_t structure
 * @note      none
 */
static void a_receive_callback(uint16_t row, uint16_t col, multi_button_t *data)
{
    switch (data->status)
    {
        case MULTI_BUTTON_STATUS_PRESS :
        {
            gs_flag |= 1 << 0;
            
            break;
        }
        case MULTI_BUTTON_STATUS_RELEASE :
        {
            gs_flag |= 1 << 1;
            
            break;
        }
        case MULTI_BUTTON_STATUS_SINGLE_CLICK :
        {
            gs_flag |= 1 << 2;
            multi_button_interface_debug_print("multi_button: row %d col %d irq single click.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_DOUBLE_CLICK :
        {
            gs_flag |= 1 << 3;
            multi_button_interface_debug_print("multi_button: row %d col %d irq double click.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_TRIPLE_CLICK :
        {
            gs_flag |= 1 << 4;
            multi_button_interface_debug_print("multi_button: row %d col %d irq triple click.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_REPEAT_CLICK :
        {
            gs_flag |= 1 << 5;
            multi_button_interface_debug_print("multi_button: row %d col %d irq repeat click with %d times.\n", row, col, data->times);
            
            break;
        }
        case MULTI_BUTTON_STATUS_SHORT_PRESS_START :
        {
            gs_flag |= 1 << 6;
            multi_button_interface_debug_print("multi_button: row %d col %d irq short press start.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_SHORT_PRESS_END :
        {
            gs_flag |= 1 << 7;
            multi_button_interface_debug_print("multi_button: row %d col %d irq short press end.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_LONG_PRESS_START :
        {
            gs_flag |= 1 << 8;
            multi_button_interface_debug_print("multi_button: row %d col %d irq long press start.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_LONG_PRESS_HOLD :
        {
            gs_flag |= 1 << 9;
            multi_button_interface_debug_print("multi_button: row %d col %d irq long press hold.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_LONG_PRESS_END :
        {
            gs_flag |= 1 << 10;
            multi_button_interface_debug_print("multi_button: row %d col %d irq long press end.\n", row, col);
            
            break;
        }
        default :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq unknown status.\n", row, col);
            
            break;
        }
    }
}

/**
 * @brief     trigger test
 * @param[in] row is the max row of buttons
 * @param[in] col is the max col of buttons
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t multi_button_trigger_test(uint8_t row, uint8_t col)
{
    uint8_t res;
    uint32_t timeout;
    multi_button_info_t info;
    
    /* link interface function */
    DRIVER_MULTI_BUTTON_LINK_INIT(&gs_handle, multi_button_handle_t);
    DRIVER_MULTI_BUTTON_LINK_MATRIX_INIT(&gs_handle, multi_button_interface_matrix_init);
    DRIVER_MULTI_BUTTON_LINK_MATRIX_DEINIT(&gs_handle, multi_button_interface_matrix_deinit);
    DRIVER_MULTI_BUTTON_LINK_MATRIX_WRITE_ROW(&gs_handle, multi_button_interface_matrix_write_row);
    DRIVER_MULTI_BUTTON_LINK_MATRIX_READ_ROW(&gs_handle, multi_button_interface_matrix_read_row);
    DRIVER_MULTI_BUTTON_LINK_TIMESTAMP_READ(&gs_handle, multi_button_interface_timestamp_read);
    DRIVER_MULTI_BUTTON_LINK_DELAY_MS(&gs_handle, multi_button_interface_delay_ms);
    DRIVER_MULTI_BUTTON_LINK_DEBUG_PRINT(&gs_handle, multi_button_interface_debug_print);
    DRIVER_MULTI_BUTTON_LINK_RECEIVE_CALLBACK(&gs_handle, a_receive_callback);
    
    /* get information */
    res = multi_button_info(&info);
    if (res != 0)
    {
        multi_button_interface_debug_print("multi_button: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        multi_button_interface_debug_print("multi_button: chip is %s.\n", info.chip_name);
        multi_button_interface_debug_print("multi_button: manufacturer is %s.\n", info.manufacturer_name);
        multi_button_interface_debug_print("multi_button: interface is %s.\n", info.interface);
        multi_button_interface_debug_print("multi_button: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        multi_button_interface_debug_print("multi_button: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        multi_button_interface_debug_print("multi_button: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        multi_button_interface_debug_print("multi_button: max current is %0.2fmA.\n", info.max_current_ma);
        multi_button_interface_debug_print("multi_button: max temperature is %0.1fC.\n", info.temperature_max);
        multi_button_interface_debug_print("multi_button: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* init */
    res = multi_button_init(&gs_handle, row, col);
    if (res != 0)
    {
        multi_button_interface_debug_print("multi_button: init failed.\n");
       
        return 1;
    }
    
    /* start trigger test */
    multi_button_interface_debug_print("multi_button: start trigger test.\n");
    
    /* 100s timeout */
    timeout = 10000;
    
    /* init 0 */
    gs_flag = 0;
    
    /* check timeout */
    while (timeout != 0)
    {
        /* check the flag */
        if ((gs_flag & 0x07FF) == 0x07FF)
        {
            break;
        }
        
        /* timeout -- */
        timeout--;
        
        /* delay 5ms */
        multi_button_interface_delay_ms(5);
        
        /* process */
        (void)multi_button_process(&gs_handle);
    }
    
    /* check the timeout */
    if (timeout == 0)
    {
        /* receive timeout */
        multi_button_interface_debug_print("multi_button: trigger timeout.\n");
        (void)multi_button_deinit(&gs_handle);
            
        return 1;
    }
    
    /* finish trigger test */
    multi_button_interface_debug_print("multi_button: finish trigger test.\n");
    (void)multi_button_deinit(&gs_handle);
    
    return 0;
}
