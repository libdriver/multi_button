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
 * @file      stm32f407_driver_multi_button_interface.c
 * @brief     stm32f407 driver multi_button interface source file
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
#include "delay.h"
#include "uart.h"
#include "tim.h"
#include <stdarg.h>

/**
 * @brief timer var definition
 */
static uint64_t gs_second = 0;        /**< second */

/**
 * @brief     timer callback
 * @param[in] us is the timer cnt
 * @note      none
 */
static void gs_tim_irq(uint32_t us)
{
    gs_second += us / 1000000;
}

/**
 * @brief  interface timer init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t multi_button_interface_timer_init(void)
{
    /* timer init */
    if (tim_init(1000000, gs_tim_irq) != 0)
    {
        return 1;
    }
    
    /* timer start */
    if (tim_start() != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  interface matrix init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t multi_button_interface_matrix_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* enable gpio clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    /* gpio init */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* gpio init */
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
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
    /* gpio deinit */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    
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
    uint16_t gpio_pin;
    GPIO_PinState pin_state;
    
    if (num == 0)
    {
        gpio_pin = GPIO_PIN_0;
    }
    else if (num == 1)
    {
        gpio_pin = GPIO_PIN_1;
    }
    else if (num == 2)
    {
        gpio_pin = GPIO_PIN_2;
    }
    else if (num == 3)
    {
        gpio_pin = GPIO_PIN_3;
    }
    else
    {
        return 1;
    }
    
    if (level != 0)
    {
        pin_state = GPIO_PIN_SET;
    }
    else
    {
        pin_state = GPIO_PIN_RESET;
    }
    
    HAL_GPIO_WritePin(GPIOA, gpio_pin, pin_state);
    
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
    GPIO_PinState pin_state;
    
    *col_array = 0;
    
    pin_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
    *col_array |= (pin_state == GPIO_PIN_RESET ? 0 : 1) << 0;
    
    pin_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
    *col_array |= (pin_state == GPIO_PIN_RESET ? 0 : 1) << 1;
    
    pin_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
    *col_array |= (pin_state == GPIO_PIN_RESET ? 0 : 1) << 2;
    
    pin_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
    *col_array |= (pin_state == GPIO_PIN_RESET ? 0 : 1) << 3;
    
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
    t->s = gs_second;
    t->us = tim_get_handle()->Instance->CNT;
    
    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void multi_button_interface_delay_ms(uint32_t ms)
{
    delay_ms(ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void multi_button_interface_debug_print(const char *const fmt, ...)
{
    char str[256];
    uint16_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    len = strlen((char *)str);
    (void)uart_write((uint8_t *)str, len);
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
