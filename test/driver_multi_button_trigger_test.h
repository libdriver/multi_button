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
 * @file      driver_multi_button_trigger_test.h
 * @brief     driver multi_button trigger test header file
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

#ifndef DRIVER_MULTI_BUTTON_TRIGGER_TEST_H
#define DRIVER_MULTI_BUTTON_TRIGGER_TEST_H

#include "driver_multi_button_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup multi_button_test_driver multi_button test driver function
 * @brief    multi_button test driver modules
 * @ingroup  multi_button_driver
 * @{
 */

/**
 * @brief     trigger test
 * @param[in] row is the max row of buttons
 * @param[in] col is the max col of buttons
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t multi_button_trigger_test(uint8_t row, uint8_t col);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif