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
 * @file      gpio.h
 * @brief     gpio header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-11-11
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/11/11  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef GPIO_H
#define GPIO_H

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @defgroup gpio gpio function
 * @brief    gpio function modules
 * @{
 */

/**
 * @brief  gpio matrix init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t gpio_matrix_init(void);

/**
 * @brief     gpio write row
 * @param[in] num row number
 * @param[in] level set level
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t gpio_matrix_write_row(uint16_t num, uint8_t level);

/**
 * @brief      gpio matrix read row
 * @param[out] *col_array pointer to a col array buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t gpio_matrix_read_row(uint32_t *col_array);

/**
 * @brief  gpio matrix deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t gpio_matrix_deinit(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
