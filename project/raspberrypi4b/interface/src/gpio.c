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
 * @file      gpio.c
 * @brief     gpio source file
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

#include "gpio.h"
#include <gpiod.h>

/**
 * @brief gpio device name definition
 */
#define GPIO_DEVICE_NAME "/dev/gpiochip0"        /**< gpio device name */

/**
 * @brief gpio device line definition
 */
#define GPIO_DEVICE_LINE_ROW0        17        /**< gpio device line */
#define GPIO_DEVICE_LINE_ROW1        27        /**< gpio device line */
#define GPIO_DEVICE_LINE_ROW2        22        /**< gpio device line */
#define GPIO_DEVICE_LINE_ROW3        5        /**< gpio device line */
#define GPIO_DEVICE_LINE_COL0        6        /**< gpio device line */
#define GPIO_DEVICE_LINE_COL1        13        /**< gpio device line */
#define GPIO_DEVICE_LINE_COL2        19        /**< gpio device line */
#define GPIO_DEVICE_LINE_COL3        26        /**< gpio device line */

/**
 * @brief global var definition
 */
static struct gpiod_chip *gs_chip;                         /**< gpio chip handle */
static struct gpiod_line *gs_line_row0;                    /**< gpio line row0 handle */
static struct gpiod_line *gs_line_row1;                    /**< gpio line row1 handle */
static struct gpiod_line *gs_line_row2;                    /**< gpio line row2 handle */
static struct gpiod_line *gs_line_row3;                    /**< gpio line row3 handle */
static struct gpiod_line *gs_line_col0;                    /**< gpio line col0 handle */
static struct gpiod_line *gs_line_col1;                    /**< gpio line col1 handle */
static struct gpiod_line *gs_line_col2;                    /**< gpio line col2 handle */
static struct gpiod_line *gs_line_col3;                    /**< gpio line col3 handle */

/**
 * @brief  gpio matrix init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t gpio_matrix_init(void)
{
    uint8_t res;
    
    /* open the gpio group */
    gs_chip = gpiod_chip_open(GPIO_DEVICE_NAME);
    if (gs_chip == NULL)
    {
        perror("gpio: open failed.\n");

        return 1;
    }
    
    /* get the gpio line */
    gs_line_row0 = gpiod_chip_get_line(gs_chip, GPIO_DEVICE_LINE_ROW0);
    if (gs_line_row0 == NULL) 
    {
        perror("gpio: get line failed.\n");
        gpiod_chip_close(gs_chip);

        return 1;
    }
    
    /* get the gpio line */
    gs_line_row1 = gpiod_chip_get_line(gs_chip, GPIO_DEVICE_LINE_ROW1);
    if (gs_line_row1 == NULL) 
    {
        perror("gpio: get line failed.\n");
        gpiod_chip_close(gs_chip);

        return 1;
    }

    /* get the gpio line */
    gs_line_row2 = gpiod_chip_get_line(gs_chip, GPIO_DEVICE_LINE_ROW2);
    if (gs_line_row2 == NULL) 
    {
        perror("gpio: get line failed.\n");
        gpiod_chip_close(gs_chip);

        return 1;
    }

    /* get the gpio line */
    gs_line_row3 = gpiod_chip_get_line(gs_chip, GPIO_DEVICE_LINE_ROW3);
    if (gs_line_row3 == NULL) 
    {
        perror("gpio: get line failed.\n");
        gpiod_chip_close(gs_chip);

        return 1;
    }

    /* get the gpio line */
    gs_line_col0 = gpiod_chip_get_line(gs_chip, GPIO_DEVICE_LINE_COL0);
    if (gs_line_col0 == NULL) 
    {
        perror("gpio: get line failed.\n");
        gpiod_chip_close(gs_chip);

        return 1;
    }

    /* get the gpio line */
    gs_line_col1 = gpiod_chip_get_line(gs_chip, GPIO_DEVICE_LINE_COL1);
    if (gs_line_col1 == NULL) 
    {
        perror("gpio: get line failed.\n");
        gpiod_chip_close(gs_chip);

        return 1;
    }

    /* get the gpio line */
    gs_line_col2 = gpiod_chip_get_line(gs_chip, GPIO_DEVICE_LINE_COL2);
    if (gs_line_col2 == NULL) 
    {
        perror("gpio: get line failed.\n");
        gpiod_chip_close(gs_chip);

        return 1;
    }

    /* get the gpio line */
    gs_line_col3 = gpiod_chip_get_line(gs_chip, GPIO_DEVICE_LINE_COL3);
    if (gs_line_col3 == NULL) 
    {
        perror("gpio: get line failed.\n");
        gpiod_chip_close(gs_chip);

        return 1;
    }

    /* set output */
    if (gpiod_line_request_output(gs_line_row0, "gpio_output", GPIOD_LINE_ACTIVE_STATE_HIGH) != 0)
    {
        return 1;
    }

    /* set output */
    if (gpiod_line_request_output(gs_line_row1, "gpio_output", GPIOD_LINE_ACTIVE_STATE_HIGH) != 0)
    {
        return 1;
    }

    /* set output */
    if (gpiod_line_request_output(gs_line_row2, "gpio_output", GPIOD_LINE_ACTIVE_STATE_HIGH) != 0)
    {
        return 1;
    }

    /* set output */
    if (gpiod_line_request_output(gs_line_row3, "gpio_output", GPIOD_LINE_ACTIVE_STATE_HIGH) != 0)
    {
        return 1;
    }

    if (gpiod_line_request_input_flags(gs_line_col0, "gpio_input", GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP) != 0)
    {
        return 1;
    }

    /* set input */
    if (gpiod_line_request_input_flags(gs_line_col1, "gpio_input", GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP) != 0) 
    {
        return 1;
    }

    /* set input */
    if (gpiod_line_request_input_flags(gs_line_col2, "gpio_input", GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP) != 0) 
    {
        return 1;
    }

    /* set input */
    if (gpiod_line_request_input_flags(gs_line_col3, "gpio_input", GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP) != 0) 
    {
        return 1;
    }

    return 0;
}

/**
 * @brief     gpio write row
 * @param[in] num is the row number
 * @param[in] level is the set level
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t gpio_matrix_write_row(uint16_t num, uint8_t level)
{
    struct gpiod_line *line;

    if (num == 0)
    {
        line = gs_line_row0;
    }
    else if (num == 1)
    {
        line = gs_line_row1;
    }
    else if (num == 2)
    {
        line = gs_line_row2;
    }
    else if (num == 3)
    {
        line = gs_line_row3;
    }
    else
    {
        return 1;
    }

    /* set the value */
    if (gpiod_line_set_value(line, level) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      gpio matrix read row
 * @param[out] *col_array points to a col array buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t gpio_matrix_read_row(uint32_t *col_array)
{
    int res;
    uint8_t value;

    *col_array = 0;

    res = gpiod_line_get_value(gs_line_col0);
    if (res < 0)
    {
        return 1;
    }
    value = (uint8_t)(res);
    *col_array |= value << 0;

    res = gpiod_line_get_value(gs_line_col1);
    if (res < 0)
    {
        return 1;
    }
    value = (uint8_t)(res);
    *col_array |= value << 1;

    res = gpiod_line_get_value(gs_line_col2);
    if (res < 0)
    {
        return 1;
    }
    value = (uint8_t)(res);
    *col_array |= value << 2;

    res = gpiod_line_get_value(gs_line_col3);
    if (res < 0)
    {
        return 1;
    }
    value = (uint8_t)(res);
    *col_array |= value << 3;

     return 0;
}

/**
 * @brief  gpio matrix deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t gpio_matrix_deinit(void)
{
    /* close the gpio */
    gpiod_chip_close(gs_chip);
    
    return 0;
}
