/**
 * Copyright (C) LibDriver 2015-2021 All rights reserved
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
 * @file      stm32f407_driver_ina3221_interface.c
 * @brief     stm32f407 driver ina3221 interface source file
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

#include "driver_ina3221_interface.h"
#include "delay.h"
#include "iic.h"
#include "uart.h"
#include <stdarg.h>

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t ina3221_interface_iic_init(void)
{
    return iic_init();
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t ina3221_interface_iic_deinit(void)
{
    return iic_deinit();
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr iic device write address
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ina3221_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    return iic_read(addr, reg, buf, len);
}

/**
 * @brief     interface iic bus write
 * @param[in] addr iic device write address
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t ina3221_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    return iic_write(addr, reg, buf, len);
}

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void ina3221_interface_delay_ms(uint32_t ms)
{
    delay_ms(ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void ina3221_interface_debug_print(const char *const fmt, ...)
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
 * @param[in] type irq type
 * @note      none
 */
void ina3221_interface_receive_callback(uint8_t type)
{
    switch (type)
    {
        case INA3221_STATUS_CRITICAL_ALERT_CHANNEL_1 :
        {
            ina3221_interface_debug_print("ina3221: irq critical alert channel 1.\n");
            
            break;
        }
        case INA3221_STATUS_CRITICAL_ALERT_CHANNEL_2 :
        {
            ina3221_interface_debug_print("ina3221: irq critical alert channel 2.\n");
            
            break;
        }
        case INA3221_STATUS_CRITICAL_ALERT_CHANNEL_3 :
        {
            ina3221_interface_debug_print("ina3221: irq critical alert channel 3.\n");
            
            break;
        }
        case INA3221_STATUS_SUMMATION_ALERT :
        {
            ina3221_interface_debug_print("ina3221: irq summation alert.\n");
            
            break;
        }
        case INA3221_STATUS_WARNING_ALERT_CHANNEL_1 :
        {
            ina3221_interface_debug_print("ina3221: irq warning alert channel 1.\n");
            
            break;
        }
        case INA3221_STATUS_WARNING_ALERT_CHANNEL_2 :
        {
            ina3221_interface_debug_print("ina3221: irq warning alert channel 2.\n");
            
            break;
        }
        case INA3221_STATUS_WARNING_ALERT_CHANNEL_3 :
        {
            ina3221_interface_debug_print("ina3221: irq warning alert channel 3.\n");
            
            break;
        }
        case INA3221_STATUS_POWER_VALID_ALERT :
        {
            ina3221_interface_debug_print("ina3221: irq power valid alert.\n");
            
            break;
        }
        case INA3221_STATUS_TIMING_CONTROL_ALERT :
        {
            ina3221_interface_debug_print("ina3221: irq timing control alert.\n");
            
            break;
        }
        default :
        {
            ina3221_interface_debug_print("ina3221: unknown code.\n");
            
            break;
        }
    }
}
