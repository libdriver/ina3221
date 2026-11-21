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
 * @date      2026-12-13
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2026/12/13  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ina3221_basic.h"
#include "driver_ina3221_shot.h"
#include "driver_ina3221_interrupt.h"
#include "driver_ina3221_register_test.h"
#include "driver_ina3221_read_test.h"
#include "driver_ina3221_interrupt_test.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];        /**< uart buffer */
volatile uint16_t g_len;   /**< uart buffer length */

/**
 * @brief     ina3221 full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t ina3221(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"resistance", required_argument, NULL, 2},
        {"times", required_argument, NULL, 3},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    double r = 0.1;
    ina3221_address_t addr = INA3221_ADDRESS_0;
    
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
            
            /* addr */
            case 1 :
            {
                /* set the addr pin */
                if (strcmp("0", optarg) == 0)
                {
                    addr = INA3221_ADDRESS_0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr = INA3221_ADDRESS_1;
                }
                else if (strcmp("2", optarg) == 0)
                {
                    addr = INA3221_ADDRESS_2;
                }
                else if (strcmp("3", optarg) == 0)
                {
                    addr = INA3221_ADDRESS_3;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* resistance */
            case 2 :
            {
                /* set the resistance */
                r = atof(optarg);
                
                break;
            }

            /* running times */
            case 3 :
            {
                /* set the times */
                times = atol(optarg);
                
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
    if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;
        
        /* run the reg test */
        res = ina3221_register_test(addr);
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        uint8_t res;
        
        /* run the read test */
        res = ina3221_read_test(addr, r, times);
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* basic init */
        res = ina3221_basic_init(addr, r);
        if (res != 0)
        {
            return 1;
        }
        
        /* delay 1000ms */
        ina3221_interface_delay_ms(1000);
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            float mV;
            float mA;
            
            /* read data */
            res = ina3221_basic_read(INA3221_CHANNEL_1, &mV, &mA);
            if (res != 0)
            {
                (void)ina3221_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ina3221_interface_debug_print("ina3221: %d/%d.\n", i + 1, times);
            ina3221_interface_debug_print("ina3221: channel1.\n");
            ina3221_interface_debug_print("ina3221: bus voltage is %0.3fmV.\n", mV);
            ina3221_interface_debug_print("ina3221: current is %0.3fmA.\n", mA);
            
            /* read data */
            res = ina3221_basic_read(INA3221_CHANNEL_2, &mV, &mA);
            if (res != 0)
            {
                (void)ina3221_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ina3221_interface_debug_print("ina3221: channel2.\n");
            ina3221_interface_debug_print("ina3221: bus voltage is %0.3fmV.\n", mV);
            ina3221_interface_debug_print("ina3221: current is %0.3fmA.\n", mA);
            
            /* read data */
            res = ina3221_basic_read(INA3221_CHANNEL_3, &mV, &mA);
            if (res != 0)
            {
                (void)ina3221_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ina3221_interface_debug_print("ina3221: channel3.\n");
            ina3221_interface_debug_print("ina3221: bus voltage is %0.3fmV.\n", mV);
            ina3221_interface_debug_print("ina3221: current is %0.3fmA.\n", mA);
            
            /* delay 1000ms */
            ina3221_interface_delay_ms(1000);
        }
        
        /* deinit */
        (void)ina3221_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_shot", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* shot init */
        res = ina3221_shot_init(addr, r);
        if (res != 0)
        {
            return 1;
        }
        
        /* delay 1000ms */
        ina3221_interface_delay_ms(1000);

        /* loop */
        for (i = 0; i < times; i++)
        {
            float mV;
            float mA;
            
            /* read data */
            res = ina3221_shot_read(INA3221_CHANNEL_1, &mV, &mA);
            if (res != 0)
            {
                (void)ina3221_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ina3221_interface_debug_print("ina3221: %d/%d.\n", i + 1, times);
            ina3221_interface_debug_print("ina3221: channel1.\n");
            ina3221_interface_debug_print("ina3221: bus voltage is %0.3fmV.\n", mV);
            ina3221_interface_debug_print("ina3221: current is %0.3fmA.\n", mA);
            
            /* read data */
            res = ina3221_shot_read(INA3221_CHANNEL_2, &mV, &mA);
            if (res != 0)
            {
                (void)ina3221_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ina3221_interface_debug_print("ina3221: channel2.\n");
            ina3221_interface_debug_print("ina3221: bus voltage is %0.3fmV.\n", mV);
            ina3221_interface_debug_print("ina3221: current is %0.3fmA.\n", mA);
            
            /* read data */
            res = ina3221_shot_read(INA3221_CHANNEL_3, &mV, &mA);
            if (res != 0)
            {
                (void)ina3221_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ina3221_interface_debug_print("ina3221: channel3.\n");
            ina3221_interface_debug_print("ina3221: bus voltage is %0.3fmV.\n", mV);
            ina3221_interface_debug_print("ina3221: current is %0.3fmA.\n", mA);
            
            /* delay 1000ms */
            ina3221_interface_delay_ms(1000);
        }
        
        /* shot deinit */
        (void)ina3221_shot_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        ina3221_interface_debug_print("Usage:\n");
        ina3221_interface_debug_print("  ina3221(-i | --information)\n");
        ina3221_interface_debug_print("  ina3221(-h | --help)\n");
        ina3221_interface_debug_print("  ina3221(-p | --port\n");
        ina3221_interface_debug_print("  ina3221(-t reg | --test=reg) [--addr=<0 | 1 | 2 | 3>]\n");
        ina3221_interface_debug_print("  ina3221(-t read | --test=read) [--addr=<0 | 1 | 2 | 3>] [--resistance=<r>] [--times=<num>]\n");
        ina3221_interface_debug_print("  ina3221(-e read | --example=read) [--addr=<0 | 1 | 2 | 3>] [--resistance=<r>] [--times=<num>]\n");
        ina3221_interface_debug_print("  ina3221(-e shot | --example=shot) [--addr=<0 | 1 | 2 | 3>] [--resistance=<r>] [--times=<num>]\n");
        ina3221_interface_debug_print("\n");
        ina3221_interface_debug_print("Options:\n");
        ina3221_interface_debug_print("      --addr=<0 | 1 | 2 | 3>     Set the addr pin.([default: 0])\n");
        ina3221_interface_debug_print("  -e <read | shot>, --example=<read | shot>\n");
        ina3221_interface_debug_print("                                 Run the driver example.\n");
        ina3221_interface_debug_print("  -h, --help                     Show the help.\n");
        ina3221_interface_debug_print("  -i, --information              Show the chip information.\n");
        ina3221_interface_debug_print("  -p, --port                     Display the pin connections of the current board.\n");
        ina3221_interface_debug_print("      --resistance=<r>           Set the sample resistance.([default: 0.1])\n");
        ina3221_interface_debug_print("  -t <reg | read>, --test=<reg | read>\n");
        ina3221_interface_debug_print("                                 Run the driver test.\n");
        ina3221_interface_debug_print("      --times=<num>              Set the running times.([default: 3])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        ina3221_info_t info;
        
        /* print ina3221 info */
        ina3221_info(&info);
        ina3221_interface_debug_print("ina3221: chip is %s.\n", info.chip_name);
        ina3221_interface_debug_print("ina3221: manufacturer is %s.\n", info.manufacturer_name);
        ina3221_interface_debug_print("ina3221: interface is %s.\n", info.interface);
        ina3221_interface_debug_print("ina3221: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ina3221_interface_debug_print("ina3221: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ina3221_interface_debug_print("ina3221: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ina3221_interface_debug_print("ina3221: max current is %0.2fmA.\n", info.max_current_ma);
        ina3221_interface_debug_print("ina3221: max temperature is %0.1fC.\n", info.temperature_max);
        ina3221_interface_debug_print("ina3221: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        ina3221_interface_debug_print("ina3221: SCL connected to GPIOB PIN8.\n");
        ina3221_interface_debug_print("ina3221: SDA connected to GPIOB PIN9.\n");
        
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
    
    /* shell init && register ina3221 function */
    shell_init();
    shell_register("ina3221", ina3221);
    uart_print("ina3221: welcome to libdriver ina3221.\n");
    
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
                uart_print("ina3221: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("ina3221: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("ina3221: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("ina3221: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("ina3221: param is invalid.\n");
            }
            else
            {
                uart_print("ina3221: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
