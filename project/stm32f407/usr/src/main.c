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
 * @file      main.c
 * @brief     main source file
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
#include "driver_multi_button_trigger_test.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "tim.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                                         /**< uart buffer */
volatile uint16_t g_len;                                    /**< uart buffer length */
volatile uint16_t g_flag;                                   /**< interrupt flag */
extern uint8_t multi_button_interface_timer_init(void);     /**< timer init function */

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
            g_flag |= 1 << 0;
            
            break;
        }
        case MULTI_BUTTON_STATUS_RELEASE :
        {
            g_flag |= 1 << 1;
            
            break;
        }
        case MULTI_BUTTON_STATUS_SINGLE_CLICK :
        {
            g_flag |= 1 << 2;
            multi_button_interface_debug_print("multi_button: row %d col %d irq single click.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_DOUBLE_CLICK :
        {
            g_flag |= 1 << 3;
            multi_button_interface_debug_print("multi_button: row %d col %d irq double click.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_TRIPLE_CLICK :
        {
            g_flag |= 1 << 4;
            multi_button_interface_debug_print("multi_button: row %d col %d irq triple click.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_REPEAT_CLICK :
        {
            g_flag |= 1 << 5;
            multi_button_interface_debug_print("multi_button: row %d col %d irq repeat click with %d times.\n", row, col, data->times);
            
            break;
        }
        case MULTI_BUTTON_STATUS_SHORT_PRESS_START :
        {
            g_flag |= 1 << 6;
            multi_button_interface_debug_print("multi_button: row %d col %d irq short press start.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_SHORT_PRESS_END :
        {
            g_flag |= 1 << 7;
            multi_button_interface_debug_print("multi_button: row %d col %d irq short press end.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_LONG_PRESS_START :
        {
            g_flag |= 1 << 8;
            multi_button_interface_debug_print("multi_button: row %d col %d irq long press start.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_LONG_PRESS_HOLD :
        {
            g_flag |= 1 << 9;
            multi_button_interface_debug_print("multi_button: row %d col %d irq long press hold.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_LONG_PRESS_END :
        {
            g_flag |= 1 << 10;
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
 * @brief     multi_button full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t multi_button(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    char short_options[] = "hipe:t:";
    struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);

                break;
            }

            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_trigger", type) == 0)
    {
        uint8_t res;

        /* run the trigger test */
        res = multi_button_trigger_test(4, 4);
        if (res != 0)
        {
            return 1;
        }

        return 0;
    }
    else if (strcmp("e_trigger", type) == 0)
    {
        uint8_t res;
        uint32_t timeout;

        /* basic init */
        res = multi_button_basic_init(4, 4, a_receive_callback);
        if (res != 0)
        {
            return 1;
        }

        /* 100s timeout */
        timeout = 10000;
        
        /* init 0 */
        g_flag = 0;
        
        /* check timeout */
        while (timeout != 0)
        {
            /* check the flag */
            if ((g_flag & 0x07FF) == 0x07FF)
            {
                break;
            }
            
            /* timeout -- */
            timeout--;
            
            /* delay 5ms */
            multi_button_interface_delay_ms(5);
            
            /* process */
            (void)multi_button_basic_process();
        }

        /* check the timeout */
        if (timeout == 0)
        {
            /* receive timeout */
            multi_button_interface_debug_print("multi_button: trigger timeout.\n");
            (void)multi_button_basic_deinit();

            return 1;
        }

        /* trigger all */
        multi_button_interface_debug_print("multi_button: trigger all.\n");

        /* basic deinit */
        (void)multi_button_basic_deinit();

        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        multi_button_interface_debug_print("Usage:\n");
        multi_button_interface_debug_print("  multi_button (-i | --information)\n");
        multi_button_interface_debug_print("  multi_button (-h | --help)\n");
        multi_button_interface_debug_print("  multi_button (-p | --port)\n");
        multi_button_interface_debug_print("  multi_button (-t trigger | --test=trigger)\n");
        multi_button_interface_debug_print("  multi_button (-e trigger | --example=trigger)\n");
        multi_button_interface_debug_print("\n");
        multi_button_interface_debug_print("Options:\n");
        multi_button_interface_debug_print("  -e <trigger>, --example=<trigger>    Run the driver example.\n");
        multi_button_interface_debug_print("  -h, --help                           Show the help.\n");
        multi_button_interface_debug_print("  -i, --information                    Show the chip information.\n");
        multi_button_interface_debug_print("  -p, --port                           Display the pin connections of the current board.\n");
        multi_button_interface_debug_print("  -t <trigger>, --test=<trigger>       Run the driver test.\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        multi_button_info_t info;

        /* print multi_button info */
        multi_button_info(&info);
        multi_button_interface_debug_print("multi_button: chip is %s.\n", info.chip_name);
        multi_button_interface_debug_print("multi_button: manufacturer is %s.\n", info.manufacturer_name);
        multi_button_interface_debug_print("multi_button: interface is %s.\n", info.interface);
        multi_button_interface_debug_print("multi_button: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        multi_button_interface_debug_print("multi_button: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        multi_button_interface_debug_print("multi_button: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        multi_button_interface_debug_print("multi_button: max current is %0.2fmA.\n", info.max_current_ma);
        multi_button_interface_debug_print("multi_button: max temperature is %0.1fC.\n", info.temperature_max);
        multi_button_interface_debug_print("multi_button: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        multi_button_interface_debug_print("multi_button: ROW0 connected to GPIOA PIN0.\n");
        multi_button_interface_debug_print("multi_button: ROW1 connected to GPIOA PIN1.\n");
        multi_button_interface_debug_print("multi_button: ROW2 connected to GPIOA PIN2.\n");
        multi_button_interface_debug_print("multi_button: ROW3 connected to GPIOA PIN3.\n");
        multi_button_interface_debug_print("multi_button: COL0 connected to GPIOA PIN4.\n");
        multi_button_interface_debug_print("multi_button: COL1 connected to GPIOA PIN5.\n");
        multi_button_interface_debug_print("multi_button: COL2 connected to GPIOA PIN6.\n");
        multi_button_interface_debug_print("multi_button: COL3 connected to GPIOA PIN7.\n");

        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;

    /* stm32f407 clock init and hal init */
    clock_init();

    /* delay init */
    delay_init();

    /* uart init */
    uart_init(115200);

    /* timer init */
    multi_button_interface_timer_init();

    /* shell init && register multi_button function */
    shell_init();
    shell_register("multi_button", multi_button);
    uart_print("multi_button: welcome to libdriver multi_button.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("multi_button: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("multi_button: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("multi_button: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("multi_button: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("multi_button: param is invalid.\n");
            }
            else
            {
                uart_print("multi_button: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
