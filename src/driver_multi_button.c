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
 * @file      driver_multi_button.c
 * @brief     driver multi_button source file
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

#include "driver_multi_button.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "General MULTI_BUTTON"     /**< chip name */
#define MANUFACTURER_NAME         "General"                  /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.8f                       /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                       /**< chip max supply voltage */
#define MAX_CURRENT               1.0f                       /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                     /**< chip min operating temperature */
#define TEMPERATURE_MAX           125.0f                     /**< chip max operating temperature */
#define DRIVER_VERSION            1000                       /**< driver version */

/**
 * @brief multi_button check definition
 */
#define MULTI_BUTTON_SHORT_TIME       (1000 * 1000)          /**< 1s */
#define MULTI_BUTTON_LONG_TIME        (3 * 1000 * 1000)      /**< 3s */
#define MULTI_BUTTON_REPEAT_TIME      (200 * 1000)           /**< 200ms */
#define MULTI_BUTTON_INTERVAL         (5 * 1000)             /**< 5ms */
#define MULTI_BUTTON_TIMEOUT          (1000 * 1000)          /**< 1s */
#define MULTI_BUTTON_REPEAT_CNT       (2)                    /**< 2 */
#define MULTI_BUTTON_PERIOD           (200 * 1000)           /**< 200ms */

/**
 * @brief     set param
 * @param[in] *handle points to a multi_button handle structure
 * @note      none
 */
static void a_multi_button_set_param(multi_button_handle_t *handle)
{
    handle->short_time = MULTI_BUTTON_SHORT_TIME;          /* set default short time */
    handle->long_time = MULTI_BUTTON_LONG_TIME;            /* set default long time */
    handle->repeat_time = MULTI_BUTTON_REPEAT_TIME;        /* set default repeat time */
    handle->interval = MULTI_BUTTON_INTERVAL;              /* set default interval */
    handle->timeout = MULTI_BUTTON_TIMEOUT;                /* set default timeout */
    handle->repeat_cnt = MULTI_BUTTON_REPEAT_CNT;          /* set default repeat cnt */
    handle->period = MULTI_BUTTON_PERIOD;                  /* set default period */
}

/**
 * @brief     reset all
 * @param[in] *handle points to a multi_button handle structure
 * @param[in] row is the set row
 * @param[in] col is the set col
 * @note      none
 */
static void a_multi_button_reset(multi_button_handle_t *handle, uint8_t row, uint8_t col)
{
    uint8_t res;
    uint16_t offset;
    multi_button_time_t t;
    
    offset = handle->col * row + col;                  /* get offset */
    handle->button[offset].decode_len = 0;             /* reset the decode */
    handle->button[offset].short_triggered = 0;        /* init 0 */
    handle->button[offset].long_triggered = 0;         /* init 0 */
    
    res = handle->timestamp_read(&t);                  /* timestamp read */
    if (res != 0)                                      /* check result */
    {
        return;                                        /* return error */
    }
    handle->button[offset].last_time.s = t.s;          /* save last time */
    handle->button[offset].last_time.us = t.us;        /* save last time */
}

/**
 * @brief     single period
 * @param[in] *handle points to a multi_button handle structure
 * @param[in] row is the set row
 * @param[in] col is the set col
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 check error
 * @note      none
 */
static uint8_t a_multi_button_single_period(multi_button_handle_t *handle, uint8_t row, uint8_t col)
{
    uint8_t res;
    uint16_t i;
    uint16_t len;
    uint16_t offset;
    int64_t diff;
    multi_button_time_t t;
    
    offset = handle->col * row + col;                                                                           /* get offset */
    if (handle->button[offset].decode_len != 0)                                                                 /* if decode len is not 0 */
    {
        res = handle->timestamp_read(&t);                                                                       /* timestamp read */
        if (res != 0)                                                                                           /* check result */
        {
            handle->debug_print("multi_button: timestamp read failed.\n");                                      /* timestamp read failed */
            
            return 1;                                                                                           /* return error */
        }
        if (handle->button[offset].decode_len == 1)                                                             /* short or long press */
        {
            diff = (int64_t)(t.s - handle->button[offset].decode[0].t.s) * 1000000 + 
                   (int64_t)(t.us - handle->button[offset].decode[0].t.us) ;                                    /* now - last time */
            if ((uint32_t)(diff) >= handle->short_time)                                                         /* check short time */
            {
                if (handle->button[offset].short_triggered == 0)                                                /* if no triggered */
                {
                    multi_button_t multi_button;
                    
                    multi_button.status = MULTI_BUTTON_STATUS_SHORT_PRESS_START;                                /* short press start */
                    multi_button.times = 0;                                                                     /* 0 times */
                    handle->receive_callback(row, col, &multi_button);                                          /* run the reception callback */
                    handle->button[offset].short_triggered = 1;                                                 /* set triggered */
                }
            }
            if ((uint32_t)(diff) >= handle->long_time)                                                          /* check long time */
            {
                if (handle->button[offset].long_triggered == 0)                                                 /* if no triggered */
                {
                    multi_button_t multi_button;
                    
                    multi_button.status = MULTI_BUTTON_STATUS_LONG_PRESS_START;                                 /* long press start */
                    multi_button.times = 0;                                                                     /* 0 times */
                    handle->receive_callback(row, col, &multi_button);                                          /* run the reception callback */
                    handle->button[offset].long_triggered = 1;                                                  /* set triggered */
                }
                else
                {
                    multi_button_t multi_button;
                    
                    multi_button.status = MULTI_BUTTON_STATUS_LONG_PRESS_HOLD;                                  /* long press hold */
                    multi_button.times = 0;                                                                     /* 0 times */
                    handle->receive_callback(row, col, &multi_button);                                          /* run the reception callback */
                }
            }
        }
        else if (handle->button[offset].decode_len == 2)                                                        /* single click, short or long press */
        {
            diff = (int64_t)(t.s - handle->button[offset].decode[1].t.s) * 1000000 + 
                   (int64_t)(t.us - handle->button[offset].decode[1].t.us) ;                                    /* now - last time */
            if (handle->button[offset].long_triggered != 0)                                                     /* if long no triggered */
            {
                multi_button_t multi_button;
                
                multi_button.status = MULTI_BUTTON_STATUS_LONG_PRESS_END;                                       /* long press end */
                multi_button.times = 0;                                                                         /* 0 times */
                handle->receive_callback(row, col, &multi_button);                                              /* run the reception callback */
                a_multi_button_reset(handle, row, col);                                                         /* reset all */
            }
            else if (handle->button[offset].short_triggered != 0)                                               /* if short no triggered */
            {
                multi_button_t multi_button;
                
                multi_button.status = MULTI_BUTTON_STATUS_SHORT_PRESS_END;                                      /* long press end */
                multi_button.times = 0;                                                                         /* 0 times */
                handle->receive_callback(row, col, &multi_button);                                              /* run the reception callback */
                a_multi_button_reset(handle, row, col);                                                         /* reset all */
            }
            else
            {
                if ((uint32_t)(diff) >= handle->repeat_time)                                                    /* check repeat time */
                {
                    multi_button_t multi_button;
                    
                    multi_button.status = MULTI_BUTTON_STATUS_SINGLE_CLICK;                                     /* single click */
                    multi_button.times = 1;                                                                     /* 1 times */
                    handle->receive_callback(row, col, &multi_button);                                          /* run the reception callback */
                    a_multi_button_reset(handle, row, col);                                                     /* reset all */
                }
            }
        }
        else if (handle->button[offset].decode_len == 4)                                                        /* double click */
        {
            diff = (int64_t)(t.s - handle->button[offset].last_time.s) * 1000000 + 
                   (int64_t)(t.us - handle->button[offset].last_time.us) ;                                      /* now - last time */
            if ((uint32_t)(diff) >= handle->repeat_time)                                                        /* check repeat time */
            {
                multi_button_t multi_button;
                
                len = handle->button[offset].decode_len - 1;                                                    /* len - 1 */
                for (i = 0; i < len; i++)                                                                       /* diff all time */
                {
                    int64_t diff2;
                    
                    diff2 = (int64_t)((int64_t)handle->button[offset].decode[i + 1].t.s -
                            (int64_t)handle->button[offset].decode[i].t.s) * 1000000 + 
                            (int64_t)((int64_t)handle->button[offset].decode[i + 1].t.us -
                            (int64_t)handle->button[offset].decode[i].t.us);                                    /* diff time */
                    handle->button[offset].decode[i].diff_us = (uint32_t)diff2;                                 /* save the time diff2 */
                }
                handle->button[offset].decode[3].diff_us = (uint32_t)diff;                                      /* save the time diff */
                
                for (i = 1; i < len; i += 2)                                                                    /* check decode length */
                {
                    if (handle->button[offset].decode[i].diff_us >= handle->repeat_time)                        /* check repeat time */
                    {
                        handle->debug_print("multi_button: double click error.\n");                             /* double click error */
                        a_multi_button_reset(handle, row, col);                                                 /* reset all */
                        
                        return 4;                                                                               /* return error */
                    }
                }
                
                multi_button.status = MULTI_BUTTON_STATUS_DOUBLE_CLICK;                                         /* double click */
                multi_button.times = 2;                                                                         /* 2 times */
                handle->receive_callback(row, col, &multi_button);                                              /* run the reception callback */
                a_multi_button_reset(handle, row, col);                                                         /* reset all */
            }
        }
        else if (handle->button[offset].decode_len == 6)                                                        /* triple click */
        {
            diff = (int64_t)(t.s - handle->button[offset].last_time.s) * 1000000 + 
                   (int64_t)(t.us - handle->button[offset].last_time.us) ;                                      /* now - last time */
            if ((uint32_t)(diff) >= handle->repeat_time)                                                        /* check repeat time */
            {
                multi_button_t multi_button;
                
                len = handle->button[offset].decode_len - 1;                                                    /* len - 1 */
                for (i = 0; i < len; i++)                                                                       /* diff all time */
                {
                    int64_t diff2;
                    
                    diff2 = (int64_t)((int64_t)handle->button[offset].decode[i + 1].t.s -
                            (int64_t)handle->button[offset].decode[i].t.s) * 1000000 + 
                            (int64_t)((int64_t)handle->button[offset].decode[i + 1].t.us -
                            (int64_t)handle->button[offset].decode[i].t.us);                                    /* diff time */
                    handle->button[offset].decode[i].diff_us = (uint32_t)diff2;                                 /* save the time diff2 */
                }
                handle->button[offset].decode[5].diff_us = (uint32_t)diff;                                      /* save the time diff */
                
                for (i = 1; i < len; i += 2)                                                                    /* check decode length */
                {
                    if (handle->button[offset].decode[i].diff_us >= handle->repeat_time)                        /* check repeat time */
                    {
                        handle->debug_print("multi_button: triple click error.\n");                             /* triple click error */
                        a_multi_button_reset(handle, row, col);                                                 /* reset all */
                        
                        return 4;                                                                               /* return error */
                    }
                }
                
                multi_button.status = MULTI_BUTTON_STATUS_TRIPLE_CLICK;                                         /* triple click */
                multi_button.times = 3;                                                                         /* 3 times */
                handle->receive_callback(row, col, &multi_button);                                              /* run the reception callback */
                a_multi_button_reset(handle, row, col);                                                         /* reset all */
            }
        }
        else
        {
            if ((handle->button[offset].decode_len > 6) && (handle->button[offset].decode_len % 2 == 0))        /* check time */
            {
                diff = (int64_t)(t.s - handle->button[offset].last_time.s) * 1000000 + 
                       (int64_t)(t.us - handle->button[offset].last_time.us) ;                                  /* now - last time */
                if ((uint32_t)(diff) >= handle->repeat_time)                                                    /* check repeat time */
                {
                    multi_button_t multi_button;
                    
                    len = handle->button[offset].decode_len - 1;                                                /* len - 1 */
                    for (i = 0; i < len; i++)                                                                   /* diff all time */
                    {
                        int64_t diff2;
                        
                        diff2 = (int64_t)((int64_t)handle->button[offset].decode[i + 1].t.s -
                                (int64_t)handle->button[offset].decode[i].t.s) * 1000000 + 
                                (int64_t)((int64_t)handle->button[offset].decode[i + 1].t.us -
                                (int64_t)handle->button[offset].decode[i].t.us);                                /* diff time */
                        handle->button[offset].decode[i].diff_us = (uint32_t)diff2;                             /* save the time diff2 */
                    }
                    handle->button[offset].decode[len].diff_us = (uint32_t)diff;                                /* save the time diff */
                    
                    for (i = 1; i < len; i += 2)                                                                /* check decode length */
                    {
                        if (handle->button[offset].decode[i].diff_us >= handle->repeat_time)                    /* check repeat time */
                        {
                            handle->debug_print("multi_button: repeat click error.\n");                         /* repeat click error */
                            a_multi_button_reset(handle, row, col);                                             /* reset all */
                            
                            return 4;                                                                           /* return error */
                        }
                    }
                    
                    multi_button.status = MULTI_BUTTON_STATUS_REPEAT_CLICK;                                     /* repeat click */
                    multi_button.times = handle->button[offset].decode_len / 2;                                 /* decode times */
                    handle->receive_callback(row, col, &multi_button);                                          /* run the reception callback */
                    a_multi_button_reset(handle, row, col);                                                     /* reset all */
                }
            }
            else
            {
                diff = (int64_t)(t.s - handle->button[offset].last_time.s) * 1000000 + 
                       (int64_t)(t.us - handle->button[offset].last_time.us) ;                                  /* now - last time */
                if ((uint32_t)(diff) >= handle->timeout)                                                        /* check timeout */
                {
                    handle->debug_print("multi_button: reset checking.\n");                                     /* reset checking */
                    a_multi_button_reset(handle, row, col);                                                     /* reset all */
                    
                    return 4;                                                                                   /* return error */
                }
            }
        }
    }
    
    return 0;                                                                                                   /* success return 0 */
}

/**
 * @brief     multi button single
 * @param[in] *handle points to a multi_button handle structure
 * @param[in] row is the set row
 * @param[in] col is the set col
 * @param[in] level is the gpio level
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 trigger error
 *            - 5 trigger too fast
 * @note      press is 1
 *            release is 0
 */
static uint8_t a_multi_button_single(multi_button_handle_t *handle, uint8_t row, uint8_t col, uint8_t level)
{
    uint8_t res;
    uint8_t press_release;
    uint16_t offset;
    int64_t diff;
    multi_button_time_t t;
    
    offset = handle->col * row + col;                                                            /* get offset */
    if (handle->button[offset].level != level)                                                   /* if level changed */
    {
        handle->button[offset].cnt++;                                                            /* cnt++ */
        if (handle->button[offset].cnt > handle->repeat_cnt)                                     /* check repeat cnt */
        {
            handle->button[offset].level = level;                                                /* set level */
            handle->button[offset].cnt = 0;                                                      /* init cnt 0 */
            if (handle->button[offset].level != 0)                                               /* if now is high level */
            {
                press_release = 0;                                                               /* release */
            }
            else
            {
                press_release = 1;                                                               /* press */
            }
        }
        else
        {
            return 0;                                                                            /* not reach cnt */
        }
    }
    else
    {
        handle->button[offset].cnt = 0;                                                          /* init cnt 0 */
        
        return 0;                                                                                /* no change */
    }
    
    res = handle->timestamp_read(&t);                                                            /* timestamp read */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("multi_button: timestamp read failed.\n");                           /* timestamp read failed */
        
        return 1;                                                                                /* return error */
    }
    diff = (int64_t)(t.s - handle->button[offset].last_time.s) * 1000000 + 
           (int64_t)(t.us - handle->button[offset].last_time.us) ;                               /* now - last time */
    if (press_release != 0)                                                                      /* if press */
    {
        if ((handle->button[offset].decode_len % 2) == 0)                                        /* press */
        {
            if ((uint32_t)(diff) < handle->interval)                                             /* check diff */
            {
                handle->debug_print("multi_button: press too fast.\n");                          /* trigger too fast */
                a_multi_button_reset(handle, row, col);                                          /* reset all */
                
                return 5;                                                                        /* success return 0 */
            }
        }
    }
    else                                                                                         /* if release */
    {
        if ((handle->button[offset].decode_len % 2) != 0)                                        /* release */
        {
            if ((uint32_t)(diff) < handle->interval)                                             /* check diff */
            {
                handle->debug_print("multi_button: release too fast.\n");                        /* release too fast */
                a_multi_button_reset(handle, row, col);                                          /* reset all */
                
                return 5;                                                                        /* success return 0 */
            }
        }
    }
    
    if (press_release != 0)                                                                      /* check press release */
    {
        if (handle->receive_callback != NULL)                                                    /* if not null */
        {
            multi_button_t multi_button;
            
            multi_button.status = MULTI_BUTTON_STATUS_PRESS;                                     /* press */
            multi_button.times = 0;                                                              /* 0 times */
            handle->receive_callback(row, col, &multi_button);                                   /* run the reception callback */
        }
    }
    else
    {
        if (handle->receive_callback != NULL)                                                    /* if not null */
        {
            multi_button_t multi_button;
            
            multi_button.status = MULTI_BUTTON_STATUS_RELEASE;                                   /* release */
            multi_button.times = 0;                                                              /* 0 times */
            handle->receive_callback(row, col, &multi_button);                                   /* run the reception callback */
        }
    }
    
    if (handle->button[offset].decode_len >= 15)                                                 /* check the max length */
    {
        a_multi_button_reset(handle, row, col);                                                  /* reset all */
    }
    if (press_release != 0)                                                                      /* if press */
    {
        if ((handle->button[offset].decode_len % 2) == 0)                                        /* press */
        {
            handle->button[offset].decode[handle->button[offset].decode_len].t.s = t.s;          /* save s */
            handle->button[offset].decode[handle->button[offset].decode_len].t.us = t.us;        /* save us */
            handle->button[offset].decode_len++;                                                 /* length++ */
        }
        else
        {
            handle->debug_print("multi_button: double press.\n");                                /* double press */
            a_multi_button_reset(handle, row, col);                                              /* reset all */
            
            return 4;                                                                            /* return error */
        }
    }
    else                                                                                         /* if release */
    {
        if ((handle->button[offset].decode_len % 2) != 0)                                        /* release */
        {
            handle->button[offset].decode[handle->button[offset].decode_len].t.s = t.s;          /* save s */
            handle->button[offset].decode[handle->button[offset].decode_len].t.us = t.us;        /* save us */
            handle->button[offset].decode_len++;                                                 /* length++ */
        }
        else
        {
            handle->debug_print("multi_button: double release.\n");                              /* double release */
            a_multi_button_reset(handle, row, col);                                              /* reset all */
            
            return 4;                                                                            /* return error */
        }
    }
    handle->button[offset].last_time.s = t.s;                                                    /* save last time */
    handle->button[offset].last_time.us = t.us;                                                  /* save last time */
    
    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief     multi button set matrix level
 * @param[in] *handle points to a multi_button handle structure
 * @param[in] row is the set row
 * @note      none
 */
static uint8_t a_multi_button_set_matrix_level(multi_button_handle_t *handle, uint8_t row)
{
    uint8_t i;
    uint8_t res;
    
    for (i = 0; i < handle->row; i++)                                                 /* loop all row */
    {
        if (i != row)                                                                 /* not set row */
        {
            res = handle->matrix_write_row(i, 1);                                     /* set row high */
            if (res != 0)                                                             /* check result */
            {
                handle->debug_print("multi_button: matrix write row failed.\n");      /* matrix write row failed */
                
                return 1;                                                             /* return error */
            }
        }
        else
        {
            res = handle->matrix_write_row(i, 0);                                     /* set row low */
            if (res != 0)                                                             /* check result */
            {
                handle->debug_print("multi_button: matrix write row failed.\n");      /* matrix write row failed */
                
                return 1;                                                             /* return error */
            }
        }
    }
    handle->delay_ms(1);                                                              /* delay 1ms */
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t multi_button_process(multi_button_handle_t *handle)
{
    uint8_t i;
    uint8_t j;
    uint8_t res; 
    uint32_t col_array;
    int64_t diff;
    multi_button_time_t t;
    
    res = handle->timestamp_read(&t);                                                                   /* timestamp read */
    if (res != 0)                                                                                       /* check result */
    {
        handle->debug_print("multi_button: timestamp read failed.\n");                                  /* timestamp read failed */
        
        return 1;                                                                                       /* return error */
    }
    diff = (int64_t)(t.s - handle->check_time.s) * 1000000 + 
           (int64_t)(t.us - handle->check_time.us) ;                                                    /* now - last time */
    if ((uint32_t)(diff) > handle->period)                                                              /* check period */
    {
        handle->check_time.s = t.s;                                                                     /* save s */
        handle->check_time.us = t.us;                                                                   /* save us */
    }
    for (i = 0; i < handle->row; i++)                                                                   /* loop all row */
    {
        res = a_multi_button_set_matrix_level(handle, i);                                               /* set matrix level */
        if (res != 0)                                                                                   /* check result */
        {
            return 1;                                                                                   /* return error */
        }
        res = handle->matrix_read_row(&col_array);                                                      /* read row */
        if (res != 0)                                                                                   /* check result */
        {
            handle->debug_print("multi_button: matrix read row failed.\n");                             /* matrix read failed */
            
            return 1;                                                                                   /* return error */
        }
        for (j = 0; j < handle->col; j++)                                                               /* loop col all */
        {
            res = a_multi_button_single(handle, i, j, (col_array >> j) & 0x1);                          /* single process */
            if (res != 0)                                                                               /* check result */
            {
                return 1;                                                                               /* return error */
            }
            if ((uint32_t)(diff) >= handle->period)                                                     /* check period */
            {
                res = a_multi_button_single_period(handle, i, j);                                       /* read period */
                if (res != 0)                                                                           /* check result */
                {
                    return 1;                                                                           /* return error */
                }
            }
        }
    }
    
    return 0;                                                                                           /* success return 0 */
}

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
uint8_t multi_button_init(multi_button_handle_t *handle, uint8_t row, uint8_t col)
{
    uint8_t res;
    uint8_t i;
    uint8_t j;
    uint16_t offset;
    multi_button_time_t t;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->debug_print == NULL)                                                        /* check debug_print */
    {
        return 3;                                                                           /* return error */
    }
    
    if (row == 0)                                                                           /* check row */
    {
        handle->debug_print("multi_button: row can't be 0.\n");                             /* row can't be 0 */
        
        return 4;                                                                           /* return error */
    }
    if (col == 0)                                                                           /* check col */
    {
        handle->debug_print("multi_button: col can't be 0.\n");                             /* col can't be 0 */
        
        return 4;                                                                           /* return error */
    }
    if (row > 32)                                                                           /* check row */
    {
        handle->debug_print("multi_button: row > 32.\n");                                   /* row > 32 */
        
        return 4;                                                                           /* return error */
    }
    if (col > 32)                                                                           /* check col */
    {
        handle->debug_print("multi_button: col > 32.\n");                                   /* col > 32 */
        
        return 4;                                                                           /* return error */
    }
    if (row * col > MULTI_BUTTON_NUMBER)                                                    /* check row and col size */
    {
        handle->debug_print("multi_button: row * col > %d.\n", MULTI_BUTTON_NUMBER);        /* row * col > MULTI_BUTTON_NUMBER */
        
        return 4;                                                                           /* return error */
    }
    
    if (handle->matrix_init == NULL)                                                        /* check matrix_init */
    {
        handle->debug_print("multi_button: matrix_init is null.\n");                        /* matrix_init is null */
        
        return 3;                                                                           /* return error */
    }
    if (handle->matrix_deinit == NULL)                                                      /* check matrix_deinit */
    {
        handle->debug_print("multi_button: matrix_deinit is null.\n");                      /* matrix_deinit is null */
        
        return 3;                                                                           /* return error */
    }
    if (handle->matrix_write_row == NULL)                                                   /* check matrix_write_row */
    {
        handle->debug_print("multi_button: matrix_write_row is null.\n");                   /* matrix_write_row is null */
        
        return 3;                                                                           /* return error */
    }
    if (handle->matrix_read_row == NULL)                                                    /* check matrix_read_row */
    {
        handle->debug_print("multi_button: matrix_read_row is null.\n");                    /* matrix_read_row is null */
        
        return 3;                                                                           /* return error */
    }
    if (handle->timestamp_read == NULL)                                                     /* check timestamp_read */
    {
        handle->debug_print("multi_button: timestamp_read is null.\n");                     /* timestamp_read is null */
        
        return 3;                                                                           /* return error */
    }
    if (handle->delay_ms == NULL)                                                           /* check delay_ms */
    {
        handle->debug_print("multi_button: delay_ms is null.\n");                           /* delay_ms is null */
        
        return 3;                                                                           /* return error */
    }
    if (handle->receive_callback == NULL)                                                   /* check receive_callback */
    {
        handle->debug_print("multi_button: receive_callback is null.\n");                   /* receive_callback is null */
        
        return 3;                                                                           /* return error */
    }
    
    handle->row = row;                                                                      /* set row */
    handle->col = col;                                                                      /* set col */
    res = handle->timestamp_read(&t);                                                       /* timestamp read */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("multi_button: timestamp read failed.\n");                      /* timestamp read failed */
        
        return 1;                                                                           /* return error */
    }
    handle->check_time.s = t.s;                                                             /* save the current s */
    handle->check_time.us = t.us;                                                           /* save the current us */
    a_multi_button_set_param(handle);                                                       /* set params */
    for (i = 0; i < row; i++)                                                               /* loop row */
    {
        for (j = 0; j < col; j++)                                                           /* loop col */
        {
            offset = handle->row * i + j;                                                   /* get offset */
            a_multi_button_reset(handle, row, col);                                         /* reset all */
            memset(&handle->button[offset], 0, sizeof(multi_button_single_t));              /* clear button */
            handle->button[offset].cnt = 0;                                                 /* init cnt 0 */
            handle->button[offset].level = 1;                                               /* init level high */
            handle->button[offset].last_time.s = t.s;                                       /* save last time */
            handle->button[offset].last_time.us = t.us;                                     /* save last time */
        }
    }
    res = handle->matrix_init();                                                            /* matrix init */
    if (res != 0)                                                                           /* check the result */
    {
        handle->debug_print("multi_button: matrix init failed.\n");                         /* matrix init failed */
        
        return 5;                                                                           /* return error */
    }
    
    handle->inited = 1;                                                                     /* flag inited */
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle points to a multi_button handle structure
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t multi_button_deinit(multi_button_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    res = handle->matrix_deinit();                                           /* matrix deinit */
    if (res != 0)                                                            /* check the result */
    {
        handle->debug_print("multi_button: matrix deinit failed.\n");        /* matrix deinit failed */
        
        return 1;                                                            /* return error */
    }
    handle->inited = 0;                                                      /* flag close */
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t multi_button_set_timeout(multi_button_handle_t *handle, uint32_t us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->timeout = us;           /* set timeout */
    
    return 0;                       /* success return 0 */
}

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
uint8_t multi_button_get_timeout(multi_button_handle_t *handle, uint32_t *us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *us = handle->timeout;          /* get timeout */
    
    return 0;                       /* success return 0 */
}

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
uint8_t multi_button_set_interval(multi_button_handle_t *handle, uint32_t us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->interval = us;          /* set interval */
    
    return 0;                       /* success return 0 */
}

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
uint8_t multi_button_get_interval(multi_button_handle_t *handle, uint32_t *us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *us = handle->interval;         /* get interval */
    
    return 0;                       /* success return 0 */
}

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
uint8_t multi_button_set_short_time(multi_button_handle_t *handle, uint32_t us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->short_time = us;        /* set short time */
    
    return 0;                       /* success return 0 */
}

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
uint8_t multi_button_get_short_time(multi_button_handle_t *handle, uint32_t *us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *us = handle->short_time;       /* get short time */
    
    return 0;                       /* success return 0 */
}

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
uint8_t multi_button_set_long_time(multi_button_handle_t *handle, uint32_t us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->long_time = us;         /* set long time */
    
    return 0;                       /* success return 0 */
}

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
uint8_t multi_button_get_long_time(multi_button_handle_t *handle, uint32_t *us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *us = handle->long_time;        /* get long time */
    
    return 0;                       /* success return 0 */
}

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
uint8_t multi_button_set_repeat_time(multi_button_handle_t *handle, uint32_t us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->repeat_time = us;       /* set repeat time */
    
    return 0;                       /* success return 0 */
}

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
uint8_t multi_button_get_repeat_time(multi_button_handle_t *handle, uint32_t *us)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *us = handle->repeat_time;      /* get repeat time */
    
    return 0;                       /* success return 0 */
}

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
uint8_t multi_button_set_repeat_cnt(multi_button_handle_t *handle, uint32_t cnt)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->repeat_cnt = cnt;       /* set repeat cnt */
    
    return 0;                       /* success return 0 */
}

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
uint8_t multi_button_get_repeat_cnt(multi_button_handle_t *handle, uint32_t *cnt)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *cnt = handle->repeat_cnt;      /* get repeat cnt */
    
    return 0;                       /* success return 0 */
}

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
uint8_t multi_button_set_period(multi_button_handle_t *handle, uint32_t period)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->period = period;        /* set period */
    
    return 0;                       /* success return 0 */
}

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
uint8_t multi_button_get_period(multi_button_handle_t *handle, uint32_t *period)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *period = handle->period;       /* get period */
    
    return 0;                       /* success return 0 */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to a multi_button info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t multi_button_info(multi_button_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(multi_button_info_t));                   /* initialize multi_button info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "GPIO", 8);                            /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
