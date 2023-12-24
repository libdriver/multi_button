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
 * @file      driver_multi_button_interface_template.c
 * @brief     driver multi_button interface template source file
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

#include "driver_multi_button_interface.h"

/**
 * @brief  interface matrix init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t multi_button_interface_matrix_init(void)
{
    return 0;
}

/**
 * @brief  interface matrix deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t multi_button_interface_matrix_deinit(void)
{
    return 0;
}

/**
 * @brief     interface matrix write row
 * @param[in] num is the row number
 * @param[in] level is the set level
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t multi_button_interface_matrix_write_row(uint16_t num, uint8_t level)
{
    return 0;
}

/**
 * @brief      interface matrix read row
 * @param[out] *col_array points to a col array buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t multi_button_interface_matrix_read_row(uint32_t *col_array)
{
    return 0;
}

/**
 * @brief     interface timestamp read
 * @param[in] *t points to a multi_button_time structure
 * @return    status code
 *            - 0 success
 *            - 1 read failed
 * @note      none
 */
uint8_t multi_button_interface_timestamp_read(multi_button_time_t *t)
{
    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void multi_button_interface_delay_ms(uint32_t ms)
{
    
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void multi_button_interface_debug_print(const char *const fmt, ...)
{
    
}

/**
 * @brief     interface receive callback
 * @param[in] row is the requested row
 * @param[in] col is the requested col
 * @param[in] *data points to an multi_button_t structure
 * @note      none
 */
void multi_button_interface_receive_callback(uint16_t row, uint16_t col, multi_button_t *data)
{
    switch (data->status)
    {
        case MULTI_BUTTON_STATUS_PRESS :
        {
            break;
        }
        case MULTI_BUTTON_STATUS_RELEASE :
        {
            break;
        }
        case MULTI_BUTTON_STATUS_SINGLE_CLICK :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq single click.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_DOUBLE_CLICK :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq double click.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_TRIPLE_CLICK :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq triple click.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_REPEAT_CLICK :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq repeat click with %d times.\n", row, col, data->times);
            
            break;
        }
        case MULTI_BUTTON_STATUS_SHORT_PRESS_START :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq short press start.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_SHORT_PRESS_END :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq short press end.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_LONG_PRESS_START :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq long press start.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_LONG_PRESS_HOLD :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq long press hold.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_LONG_PRESS_END :
        {
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
