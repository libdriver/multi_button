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
 * @file      driver_multi_button.h
 * @brief     driver multi_button header file
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

#ifndef DRIVER_MULTI_BUTTON_H
#define DRIVER_MULTI_BUTTON_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup multi_button_driver multi_button driver function
 * @brief    multi_button driver modules
 * @{
 */

/**
 * @addtogroup multi_button_basic_driver
 * @{
 */

/**
 * @brief multi_button max number definition
 */
#ifndef MULTI_BUTTON_NUMBER
    #define MULTI_BUTTON_NUMBER        128          /**< max 128 number */
#endif

/**
 * @brief multi_button status enumeration definition
 */
typedef enum
{
    MULTI_BUTTON_STATUS_PRESS             = (1 << 0),        /**< press */
    MULTI_BUTTON_STATUS_RELEASE           = (1 << 1),        /**< release */
    MULTI_BUTTON_STATUS_SINGLE_CLICK      = (1 << 2),        /**< single click */
    MULTI_BUTTON_STATUS_DOUBLE_CLICK      = (1 << 3),        /**< double click */
    MULTI_BUTTON_STATUS_TRIPLE_CLICK      = (1 << 4),        /**< triple click */
    MULTI_BUTTON_STATUS_REPEAT_CLICK      = (1 << 5),        /**< repeat click */
    MULTI_BUTTON_STATUS_SHORT_PRESS_START = (1 << 6),        /**< short press start */
    MULTI_BUTTON_STATUS_SHORT_PRESS_END   = (1 << 7),        /**< short press end */
    MULTI_BUTTON_STATUS_LONG_PRESS_START  = (1 << 8),        /**< long press start */
    MULTI_BUTTON_STATUS_LONG_PRESS_HOLD   = (1 << 9),        /**< long press hold */
    MULTI_BUTTON_STATUS_LONG_PRESS_END    = (1 << 10),       /**< long press end */
} multi_button_status_t;

/**
 * @brief multi_button structure definition
 */
typedef struct multi_button_s
{
    uint16_t status;       /**< status */
    uint16_t times;        /**< times */
} multi_button_t;

/**
 * @brief multi_button time structure definition
 */
typedef struct multi_button_time_s
{
    uint64_t s;         /**< second */
    uint32_t us;        /**< microsecond */
} multi_button_time_t;

/**
 * @brief multi_button decode structure definition
 */
typedef struct multi_button_decode_s
{
    multi_button_time_t t;        /**< timestamp */
    uint32_t diff_us;             /**< diff us */
} multi_button_decode_t;

/**
 * @brief multi_button single structure definition
 */
typedef struct multi_button_single_s
{
    multi_button_time_t last_time;           /**< last time */
    multi_button_decode_t decode[16];        /**< decode buffer */
    uint16_t decode_len;                     /**< decode length */
    uint8_t short_triggered;                 /**< short triggered */
    uint8_t long_triggered;                  /**< long triggered */
    uint8_t level;                           /**< gpio level */
    uint8_t cnt;                             /**< detect times */
} multi_button_single_t;

/**
 * @brief multi_button handle structure definition
 */
typedef struct multi_button_handle_s
{
    uint8_t (*matrix_init)(void);                                                      /**< point to a matrix_init function address */
    uint8_t (*matrix_deinit)(void);                                                    /**< point to a matrix_deinit function address */
    uint8_t (*matrix_write_row)(uint16_t num, uint8_t level);                          /**< point to a matrix_write_row function address */
    uint8_t (*matrix_read_row)(uint32_t *col_array);                                   /**< point to a matrix_read_row function address */
    uint8_t (*timestamp_read)(multi_button_time_t *t);                                 /**< point to a timestamp_read function address */
    void (*delay_ms)(uint32_t ms);                                                     /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                   /**< point to a debug_print function address */
    void (*receive_callback)(uint16_t row, uint16_t col, multi_button_t *data);        /**< point to a receive_callback function address */
    multi_button_single_t button[MULTI_BUTTON_NUMBER];                                 /**< buttons */
    multi_button_time_t check_time;                                                    /**< check time */
    uint8_t inited;                                                                    /**< inited flag */
    uint8_t row;                                                                       /**< row */
    uint8_t col;                                                                       /**< col */
    uint32_t short_time;                                                               /**< short time */
    uint32_t long_time;                                                                /**< long time */
    uint32_t repeat_time;                                                              /**< repeat time */
    uint32_t interval;                                                                 /**< interval */
    uint32_t timeout;                                                                  /**< timeout */
    uint32_t repeat_cnt;                                                               /**< repeat cnt */
    uint32_t period;                                                                   /**< period */
} multi_button_handle_t;

/**
 * @brief multi_button information structure definition
 */
typedef struct multi_button_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} multi_button_info_t;

/**
 * @}
 */

/**
 * @defgroup multi_button_link_driver multi_button link driver function
 * @brief    multi_button link driver modules
 * @ingroup  multi_button_driver
 * @{
 */

/**
 * @brief     initialize multi_button_handle_t structure
 * @param[in] HANDLE points to a multi_button handle structure
 * @param[in] STRUCTURE is multi_button_handle_t
 * @note      none
 */
#define DRIVER_MULTI_BUTTON_LINK_INIT(HANDLE, STRUCTURE)           memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link matrix_init function
 * @param[in] HANDLE points to a multi_button handle structure
 * @param[in] FUC points to a matrix_init function address
 * @note      none
 */
#define DRIVER_MULTI_BUTTON_LINK_MATRIX_INIT(HANDLE, FUC)         (HANDLE)->matrix_init = FUC

/**
 * @brief     link matrix_deinit function
 * @param[in] HANDLE points to a multi_button handle structure
 * @param[in] FUC points to a matrix_deinit function address
 * @note      none
 */
#define DRIVER_MULTI_BUTTON_LINK_MATRIX_DEINIT(HANDLE, FUC)       (HANDLE)->matrix_deinit = FUC

/**
 * @brief     link matrix_write_row function
 * @param[in] HANDLE points to a multi_button handle structure
 * @param[in] FUC points to a matrix_write_row function address
 * @note      none
 */
#define DRIVER_MULTI_BUTTON_LINK_MATRIX_WRITE_ROW(HANDLE, FUC)    (HANDLE)->matrix_write_row = FUC

/**
 * @brief     link matrix_read_row function
 * @param[in] HANDLE points to a multi_button handle structure
 * @param[in] FUC points to a matrix_read_row function address
 * @note      none
 */
#define DRIVER_MULTI_BUTTON_LINK_MATRIX_READ_ROW(HANDLE, FUC)     (HANDLE)->matrix_read_row = FUC

/**
 * @brief     link timestamp_read function
 * @param[in] HANDLE points to a multi_button handle structure
 * @param[in] FUC points to a timestamp_read function address
 * @note      none
 */
#define DRIVER_MULTI_BUTTON_LINK_TIMESTAMP_READ(HANDLE, FUC)      (HANDLE)->timestamp_read = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to a multi_button handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_MULTI_BUTTON_LINK_DELAY_MS(HANDLE, FUC)            (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to a multi_button handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_MULTI_BUTTON_LINK_DEBUG_PRINT(HANDLE, FUC)         (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE points to a multi_button handle structure
 * @param[in] FUC points to a receive_callback function address
 * @note      none
 */
#define DRIVER_MULTI_BUTTON_LINK_RECEIVE_CALLBACK(HANDLE, FUC)    (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup multi_button_basic_driver multi_button basic driver function
 * @brief    multi_button basic driver modules
 * @ingroup  multi_button_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to a multi_button info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t multi_button_info(multi_button_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a multi_button handle structure
 * @param[in] row is the max row of buttons
 * @param[in] col is the max col of buttons
 * @return    status code
 *            - 0 success
 *            - 1 gpio initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 row or col is invalid
 *            - 5 matrix init failed
 * @note      none
 */
uint8_t multi_button_init(multi_button_handle_t *handle, uint8_t row, uint8_t col);

/**
 * @brief     close the chip
 * @param[in] *handle points to a multi_button handle structure
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t multi_button_deinit(multi_button_handle_t *handle);

/**
 * @brief     process
 * @param[in] *handle points to a multi_button handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t multi_button_process(multi_button_handle_t *handle);

/**
 * @brief     set timeout
 * @param[in] *handle points to a multi_button handle structure
 * @param[in] us is the set time
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t multi_button_set_timeout(multi_button_handle_t *handle, uint32_t us);

/**
 * @brief      get timeout
 * @param[in]  *handle points to a multi_button handle structure
 * @param[out] *us points to a time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t multi_button_get_timeout(multi_button_handle_t *handle, uint32_t *us);

/**
 * @brief     set interval
 * @param[in] *handle points to a multi_button handle structure
 * @param[in] us is the set time
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t multi_button_set_interval(multi_button_handle_t *handle, uint32_t us);

/**
 * @brief      get interval
 * @param[in]  *handle points to a multi_button handle structure
 * @param[out] *us points to a time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t multi_button_get_interval(multi_button_handle_t *handle, uint32_t *us);

/**
 * @brief     set short time
 * @param[in] *handle points to a multi_button handle structure
 * @param[in] us is the set time
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t multi_button_set_short_time(multi_button_handle_t *handle, uint32_t us);

/**
 * @brief      get short time
 * @param[in]  *handle points to a multi_button handle structure
 * @param[out] *us points to a time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t multi_button_get_short_time(multi_button_handle_t *handle, uint32_t *us);

/**
 * @brief     set long time
 * @param[in] *handle points to a multi_button handle structure
 * @param[in] us is the set time
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t multi_button_set_long_time(multi_button_handle_t *handle, uint32_t us);

/**
 * @brief      get long time
 * @param[in]  *handle points to a multi_button handle structure
 * @param[out] *us points to a time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t multi_button_get_long_time(multi_button_handle_t *handle, uint32_t *us);

/**
 * @brief     set repeat time
 * @param[in] *handle points to a multi_button handle structure
 * @param[in] us is the set time
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t multi_button_set_repeat_time(multi_button_handle_t *handle, uint32_t us);

/**
 * @brief      get repeat time
 * @param[in]  *handle points to a multi_button handle structure
 * @param[out] *us points to a time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t multi_button_get_repeat_time(multi_button_handle_t *handle, uint32_t *us);

/**
 * @brief     set repeat cnt
 * @param[in] *handle points to a multi_button handle structure
 * @param[in] cnt is the set repeat counter
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t multi_button_set_repeat_cnt(multi_button_handle_t *handle, uint32_t cnt);

/**
 * @brief      get repeat cnt
 * @param[in]  *handle points to a multi_button handle structure
 * @param[out] *cnt points to a counter buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t multi_button_get_repeat_cnt(multi_button_handle_t *handle, uint32_t *cnt);

/**
 * @brief     set period
 * @param[in] *handle points to a multi_button handle structure
 * @param[in] period is the set period
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t multi_button_set_period(multi_button_handle_t *handle, uint32_t period);

/**
 * @brief      get period
 * @param[in]  *handle points to a multi_button handle structure
 * @param[out] *period points to a period buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t multi_button_get_period(multi_button_handle_t *handle, uint32_t *period);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
