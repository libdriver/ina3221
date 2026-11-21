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
 * @file      driver_ina3221_register_test.c
 * @brief     driver ina3221 register test source file
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

#include "driver_ina3221_register_test.h"
#include <stdlib.h>
#include <math.h>

static ina3221_handle_t gs_handle;        /**< ina3221 handle */

/**
 * @brief     register test
 * @param[in] addr_pin iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ina3221_register_test(ina3221_address_t addr_pin)
{
    uint8_t res;
    float r;
    float r_check;
    uint16_t reg;
    uint16_t reg_check;
    float data_f;
    float data_check_f;
    ina3221_info_t info;
    ina3221_address_t addr;
    ina3221_bool_t enable;
    ina3221_adc_average_t average_check;
    ina3221_conversion_time_t ct_check;
    ina3221_mode_t mode_check;
    
    /* link interface function */
    DRIVER_INA3221_LINK_INIT(&gs_handle, ina3221_handle_t);
    DRIVER_INA3221_LINK_IIC_INIT(&gs_handle, ina3221_interface_iic_init);
    DRIVER_INA3221_LINK_IIC_DEINIT(&gs_handle, ina3221_interface_iic_deinit);
    DRIVER_INA3221_LINK_IIC_READ(&gs_handle, ina3221_interface_iic_read);
    DRIVER_INA3221_LINK_IIC_WRITE(&gs_handle, ina3221_interface_iic_write);
    DRIVER_INA3221_LINK_DELAY_MS(&gs_handle, ina3221_interface_delay_ms);
    DRIVER_INA3221_LINK_DEBUG_PRINT(&gs_handle, ina3221_interface_debug_print);
    DRIVER_INA3221_LINK_RECEIVE_CALLBACK(&gs_handle, ina3221_interface_receive_callback);
    
    /* get information */
    res = ina3221_info(&info);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        ina3221_interface_debug_print("ina3221: chip is %s.\n", info.chip_name);
        ina3221_interface_debug_print("ina3221: manufacturer is %s.\n", info.manufacturer_name);
        ina3221_interface_debug_print("ina3221: interface is %s.\n", info.interface);
        ina3221_interface_debug_print("ina3221: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ina3221_interface_debug_print("ina3221: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ina3221_interface_debug_print("ina3221: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ina3221_interface_debug_print("ina3221: max current is %0.2fmA.\n", info.max_current_ma);
        ina3221_interface_debug_print("ina3221: max temperature is %0.1fC.\n", info.temperature_max);
        ina3221_interface_debug_print("ina3221: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    ina3221_interface_debug_print("ina3221: start register test.\n");
    
    /* ina3221_set_addr_pin/ina3221_get_addr_pin test */
    ina3221_interface_debug_print("ina3221: ina3221_set_addr_pin/ina3221_get_addr_pin test.\n");
    
    /* set address 0 */
    res = ina3221_set_addr_pin(&gs_handle, INA3221_ADDRESS_0);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set addr pin failed.\n");
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set addr pin 0.\n");
    res = ina3221_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get addr pin failed.\n");
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check addr pin %s.\n", (addr == INA3221_ADDRESS_0) ? "ok" : "error");
    
    /* set address 1 */
    res = ina3221_set_addr_pin(&gs_handle, INA3221_ADDRESS_1);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set addr pin failed.\n");
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set addr pin 1.\n");
    res = ina3221_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get addr pin failed.\n");
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check addr pin %s.\n", (addr == INA3221_ADDRESS_1) ? "ok" : "error");
    
    /* set address 2 */
    res = ina3221_set_addr_pin(&gs_handle, INA3221_ADDRESS_2);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set addr pin failed.\n");
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set addr pin 2.\n");
    res = ina3221_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get addr pin failed.\n");
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check addr pin %s.\n", (addr == INA3221_ADDRESS_2) ? "ok" : "error");
    
    /* set address 3 */
    res = ina3221_set_addr_pin(&gs_handle, INA3221_ADDRESS_3);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set addr pin failed.\n");
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set addr pin 3.\n");
    res = ina3221_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get addr pin failed.\n");
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check addr pin %s.\n", (addr == INA3221_ADDRESS_3) ? "ok" : "error");
    
    /* ina3221_set_resistance/ina3221_get_resistance test */
    ina3221_interface_debug_print("ina3221: ina3221_set_resistance/ina3221_get_resistance test.\n");
    
    /* generate the r */
    r = (float)(rand() % 100) / 1000.0;
    res = ina3221_set_resistance(&gs_handle, r);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set resistance failed.\n");
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set resistance %f.\n", r);
    res = ina3221_get_resistance(&gs_handle, (float *)&r_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get resistance failed.\n");
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check resistance %f.\n", r_check);
    
    /* set addr pin */
    res = ina3221_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set addr pin failed.\n");
       
        return 1;
    }
    
    /* init */
    res = ina3221_init(&gs_handle);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: init failed.\n");
       
        return 1;
    }
    
    /* ina3221_set_channel/ina3221_get_channel test */
    ina3221_interface_debug_print("ina3221: ina3221_set_channel/ina3221_get_channel test.\n");
    
    /* channel 1 disable */
    res = ina3221_set_channel(&gs_handle, INA3221_CHANNEL_1, INA3221_BOOL_FALSE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: channel 1 disable.\n");
    res = ina3221_get_channel(&gs_handle, INA3221_CHANNEL_1, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel %s.\n", (enable == INA3221_BOOL_FALSE) ? "ok" : "error");
    
    /* channel 1 enable */
    res = ina3221_set_channel(&gs_handle, INA3221_CHANNEL_1, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: channel 1 enable.\n");
    res = ina3221_get_channel(&gs_handle, INA3221_CHANNEL_1, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel %s.\n", (enable == INA3221_BOOL_TRUE) ? "ok" : "error");
    
    /* channel 2 disable */
    res = ina3221_set_channel(&gs_handle, INA3221_CHANNEL_2, INA3221_BOOL_FALSE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: channel 2 disable.\n");
    res = ina3221_get_channel(&gs_handle, INA3221_CHANNEL_2, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel %s.\n", (enable == INA3221_BOOL_FALSE) ? "ok" : "error");
    
    /* channel 2 enable */
    res = ina3221_set_channel(&gs_handle, INA3221_CHANNEL_2, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: channel 2 enable.\n");
    res = ina3221_get_channel(&gs_handle, INA3221_CHANNEL_2, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel %s.\n", (enable == INA3221_BOOL_TRUE) ? "ok" : "error");
    
    /* channel 3 disable */
    res = ina3221_set_channel(&gs_handle, INA3221_CHANNEL_3, INA3221_BOOL_FALSE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: channel 3 disable.\n");
    res = ina3221_get_channel(&gs_handle, INA3221_CHANNEL_3, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel %s.\n", (enable == INA3221_BOOL_FALSE) ? "ok" : "error");
    
    /* channel 3 enable */
    res = ina3221_set_channel(&gs_handle, INA3221_CHANNEL_3, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: channel 3 enable.\n");
    res = ina3221_get_channel(&gs_handle, INA3221_CHANNEL_3, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel %s.\n", (enable == INA3221_BOOL_TRUE) ? "ok" : "error");
    
    /* ina3221_set_adc_average/ina3221_get_adc_average test */
    ina3221_interface_debug_print("ina3221: ina3221_set_adc_average/ina3221_get_adc_average test.\n");
    
    /* 1 sample */
    res = ina3221_set_adc_average(&gs_handle, INA3221_ADC_AVERAGE_1);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set adc average 1 sample.\n");
    res = ina3221_get_adc_average(&gs_handle, &average_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check adc average %s.\n", (average_check == INA3221_ADC_AVERAGE_1) ? "ok" : "error");
    
    /* 4 samples */
    res = ina3221_set_adc_average(&gs_handle, INA3221_ADC_AVERAGE_4);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set adc average 4 samples.\n");
    res = ina3221_get_adc_average(&gs_handle, &average_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check adc average %s.\n", (average_check == INA3221_ADC_AVERAGE_4) ? "ok" : "error");
    
    /* 16 samples */
    res = ina3221_set_adc_average(&gs_handle, INA3221_ADC_AVERAGE_16);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set adc average 16 samples.\n");
    res = ina3221_get_adc_average(&gs_handle, &average_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check adc average %s.\n", (average_check == INA3221_ADC_AVERAGE_16) ? "ok" : "error");
    
    /* 64 samples */
    res = ina3221_set_adc_average(&gs_handle, INA3221_ADC_AVERAGE_64);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set adc average 64 samples.\n");
    res = ina3221_get_adc_average(&gs_handle, &average_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check adc average %s.\n", (average_check == INA3221_ADC_AVERAGE_64) ? "ok" : "error");
    
    /* 128 samples */
    res = ina3221_set_adc_average(&gs_handle, INA3221_ADC_AVERAGE_128);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set adc average 128 samples.\n");
    res = ina3221_get_adc_average(&gs_handle, &average_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check adc average %s.\n", (average_check == INA3221_ADC_AVERAGE_128) ? "ok" : "error");
    
    /* 256 samples */
    res = ina3221_set_adc_average(&gs_handle, INA3221_ADC_AVERAGE_256);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set adc average 256 samples.\n");
    res = ina3221_get_adc_average(&gs_handle, &average_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check adc average %s.\n", (average_check == INA3221_ADC_AVERAGE_256) ? "ok" : "error");
    
    /* 512 samples */
    res = ina3221_set_adc_average(&gs_handle, INA3221_ADC_AVERAGE_512);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set adc average 512 samples.\n");
    res = ina3221_get_adc_average(&gs_handle, &average_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check adc average %s.\n", (average_check == INA3221_ADC_AVERAGE_512) ? "ok" : "error");
    
    /* 1024 samples */
    res = ina3221_set_adc_average(&gs_handle, INA3221_ADC_AVERAGE_1024);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set adc average 1024 samples.\n");
    res = ina3221_get_adc_average(&gs_handle, &average_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check adc average %s.\n", (average_check == INA3221_ADC_AVERAGE_1024) ? "ok" : "error");
    
    /* ina3221_set_bus_voltage_conversion_time/ina3221_get_bus_voltage_conversion_time test */
    ina3221_interface_debug_print("ina3221: ina3221_set_bus_voltage_conversion_time/ina3221_get_bus_voltage_conversion_time test.\n");
    
    /* 140 us */
    res = ina3221_set_bus_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_140_US);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set bus voltage conversion time 140 us.\n");
    res = ina3221_get_bus_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check bus voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_140_US) ? "ok" : "error");
    
    /* 204 us */
    res = ina3221_set_bus_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_204_US);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set bus voltage conversion time 204 us.\n");
    res = ina3221_get_bus_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check bus voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_204_US) ? "ok" : "error");
    
    /* 332 us */
    res = ina3221_set_bus_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_332_US);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set bus voltage conversion time 332 us.\n");
    res = ina3221_get_bus_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check bus voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_332_US) ? "ok" : "error");
    
    /* 588 us */
    res = ina3221_set_bus_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_588_US);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set bus voltage conversion time 588 us.\n");
    res = ina3221_get_bus_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check bus voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_588_US) ? "ok" : "error");
    
    /* 1.1 ms */
    res = ina3221_set_bus_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_1P1_MS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set bus voltage conversion time 1.1 ms.\n");
    res = ina3221_get_bus_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check bus voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_1P1_MS) ? "ok" : "error");
    
    /* 2.116 ms */
    res = ina3221_set_bus_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_2P116_MS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set bus voltage conversion time 2.116 ms.\n");
    res = ina3221_get_bus_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check bus voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_2P116_MS) ? "ok" : "error");
    
    /* 4.156 ms */
    res = ina3221_set_bus_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_4P156_MS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set bus voltage conversion time 4.156 ms.\n");
    res = ina3221_get_bus_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check bus voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_4P156_MS) ? "ok" : "error");
    
    /* 8.244 ms */
    res = ina3221_set_bus_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_8P244_MS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set bus voltage conversion time 8.244 ms.\n");
    res = ina3221_get_bus_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check bus voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_8P244_MS) ? "ok" : "error");
    
    /* ina3221_set_shunt_voltage_conversion_time/ina3221_get_shunt_voltage_conversion_time test */
    ina3221_interface_debug_print("ina3221: ina3221_set_shunt_voltage_conversion_time/ina3221_get_shunt_voltage_conversion_time test.\n");
    
    /* 140 us */
    res = ina3221_set_shunt_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_140_US);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set shunt voltage conversion time 140 us.\n");
    res = ina3221_get_shunt_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check shunt voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_140_US) ? "ok" : "error");
    
    /* 204 us */
    res = ina3221_set_shunt_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_204_US);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set shunt voltage conversion time 204 us.\n");
    res = ina3221_get_shunt_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check shunt voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_204_US) ? "ok" : "error");
    
    /* 332 us */
    res = ina3221_set_shunt_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_332_US);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set shunt voltage conversion time 332 us.\n");
    res = ina3221_get_shunt_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check shunt voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_332_US) ? "ok" : "error");
    
    /* 588 us */
    res = ina3221_set_shunt_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_588_US);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set shunt voltage conversion time 588 us.\n");
    res = ina3221_get_shunt_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check shunt voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_588_US) ? "ok" : "error");
    
    /* 1.1 ms */
    res = ina3221_set_shunt_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_1P1_MS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set shunt voltage conversion time 1.1 ms.\n");
    res = ina3221_get_shunt_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check shunt voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_1P1_MS) ? "ok" : "error");
    
    /* 2.116 ms */
    res = ina3221_set_shunt_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_2P116_MS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set shunt voltage conversion time 2.116 ms.\n");
    res = ina3221_get_shunt_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check shunt voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_2P116_MS) ? "ok" : "error");
    
    /* 4.156 ms */
    res = ina3221_set_shunt_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_4P156_MS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set shunt voltage conversion time 4.156 ms.\n");
    res = ina3221_get_shunt_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check shunt voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_4P156_MS) ? "ok" : "error");
    
    /* 8.244 ms */
    res = ina3221_set_shunt_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_8P244_MS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set shunt voltage conversion time 8.244 ms.\n");
    res = ina3221_get_shunt_voltage_conversion_time(&gs_handle, &ct_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check shunt voltage conversion time %s.\n", (ct_check == INA3221_CONVERSION_TIME_8P244_MS) ? "ok" : "error");
    
    /* 1 sample */
    res = ina3221_set_adc_average(&gs_handle, INA3221_ADC_AVERAGE_1);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    
    /* ina3221_set_mode/ina3221_get_mode test */
    ina3221_interface_debug_print("ina3221: ina3221_set_mode/ina3221_get_mode test.\n");
    
    /* power down */
    res = ina3221_set_mode(&gs_handle, INA3221_MODE_POWER_DOWN);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set mode power down.\n");
    res = ina3221_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check mode %s.\n", (mode_check == INA3221_MODE_POWER_DOWN) ? "ok" : "error");
    
    /* shunt voltage triggered */
    res = ina3221_set_mode(&gs_handle, INA3221_MODE_SHUNT_VOLTAGE_TRIGGERED);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set mode shunt voltage triggered.\n");
    res = ina3221_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check mode %s.\n", (mode_check == INA3221_MODE_SHUNT_VOLTAGE_TRIGGERED) ? "ok" : "error");
    
    /* bus voltage triggered */
    res = ina3221_set_mode(&gs_handle, INA3221_MODE_BUS_VOLTAGE_TRIGGERED);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set mode bus voltage triggered.\n");
    res = ina3221_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check mode %s.\n", (mode_check == INA3221_MODE_BUS_VOLTAGE_TRIGGERED) ? "ok" : "error");
    
    /* shunt and bus voltage triggered */
    res = ina3221_set_mode(&gs_handle, INA3221_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set mode shunt and bus voltage triggered.\n");
    res = ina3221_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check mode %s.\n", (mode_check == INA3221_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED) ? "ok" : "error");
    
    /* shunt voltage continuous */
    res = ina3221_set_mode(&gs_handle, INA3221_MODE_SHUNT_VOLTAGE_CONTINUOUS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set mode shunt voltage continuous.\n");
    res = ina3221_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check mode %s.\n", (mode_check == INA3221_MODE_SHUNT_VOLTAGE_CONTINUOUS) ? "ok" : "error");
    
    /* bus voltage continuous */
    res = ina3221_set_mode(&gs_handle, INA3221_MODE_BUS_VOLTAGE_CONTINUOUS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set mode bus voltage continuous.\n");
    res = ina3221_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check mode %s.\n", (mode_check == INA3221_MODE_BUS_VOLTAGE_CONTINUOUS) ? "ok" : "error");
    
    /* shunt and bus voltage continuous */
    res = ina3221_set_mode(&gs_handle, INA3221_MODE_SHUNT_BUS_VOLTAGE_CONTINUOUS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set mode shunt and bus voltage continuous.\n");
    res = ina3221_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check mode %s.\n", (mode_check == INA3221_MODE_SHUNT_BUS_VOLTAGE_CONTINUOUS) ? "ok" : "error");
    
    /* power down */
    res = ina3221_set_mode(&gs_handle, INA3221_MODE_POWER_DOWN);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    
    /* ina3221_set_critical_alert_limit/ina3221_get_critical_alert_limit test */
    ina3221_interface_debug_print("ina3221: ina3221_set_critical_alert_limit/ina3221_get_critical_alert_limit test.\n");
    
    /* channel 1 */
    reg = (uint16_t)((rand() % 0x2000) << 3);
    res = ina3221_set_critical_alert_limit(&gs_handle, INA3221_CHANNEL_1, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set critical alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set channel 1 critical alert limit 0x%04X.\n", reg);
    res = ina3221_get_critical_alert_limit(&gs_handle, INA3221_CHANNEL_1, &reg_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get critical alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel 1 critical alert limit 0x%04X %s.\n", reg, (reg == reg_check) ? "ok" : "error");
    
    /* channel 2 */
    reg = (uint16_t)((rand() % 0x2000) << 3);
    res = ina3221_set_critical_alert_limit(&gs_handle, INA3221_CHANNEL_2, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set critical alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set channel 2 critical alert limit 0x%04X.\n", reg);
    res = ina3221_get_critical_alert_limit(&gs_handle, INA3221_CHANNEL_2, &reg_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get critical alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel 2 critical alert limit 0x%04X %s.\n", reg, (reg == reg_check) ? "ok" : "error");
    
    /* channel 3 */
    reg = (uint16_t)((rand() % 0x2000) << 3);
    res = ina3221_set_critical_alert_limit(&gs_handle, INA3221_CHANNEL_3, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set critical alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set channel 3 critical alert limit 0x%04X.\n", reg);
    res = ina3221_get_critical_alert_limit(&gs_handle, INA3221_CHANNEL_3, &reg_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get critical alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel 3 critical alert limit 0x%04X %s.\n", reg, (reg == reg_check) ? "ok" : "error");
    
    /* ina3221_set_warning_alert_limit/ina3221_get_warning_alert_limit test */
    ina3221_interface_debug_print("ina3221: ina3221_set_warning_alert_limit/ina3221_get_warning_alert_limit test.\n");
    
    /* channel 1 */
    reg = (uint16_t)((rand() % 0x2000) << 3);
    res = ina3221_set_warning_alert_limit(&gs_handle, INA3221_CHANNEL_1, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set warning alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set channel 1 warning alert limit 0x%04X.\n", reg);
    res = ina3221_get_warning_alert_limit(&gs_handle, INA3221_CHANNEL_1, &reg_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get warning alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel 1 warning alert limit 0x%04X %s.\n", reg, (reg == reg_check) ? "ok" : "error");
    
    /* channel 2 */
    reg = (uint16_t)((rand() % 0x2000) << 3);
    res = ina3221_set_warning_alert_limit(&gs_handle, INA3221_CHANNEL_2, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set warning alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set channel 2 warning alert limit 0x%04X.\n", reg);
    res = ina3221_get_warning_alert_limit(&gs_handle, INA3221_CHANNEL_2, &reg_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get warning alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel 2 warning alert limit 0x%04X %s.\n", reg, (reg == reg_check) ? "ok" : "error");
    
    /* channel 3 */
    reg = (uint16_t)((rand() % 0x2000) << 3);
    res = ina3221_set_warning_alert_limit(&gs_handle, INA3221_CHANNEL_3, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set warning alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set channel 3 warning alert limit 0x%04X.\n", reg);
    res = ina3221_get_warning_alert_limit(&gs_handle, INA3221_CHANNEL_3, &reg_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get warning alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel 3 warning alert limit 0x%04X %s.\n", reg, (reg == reg_check) ? "ok" : "error");
    
    /* ina3221_set_shunt_voltage_sum_limit/ina3221_get_shunt_voltage_sum_limit test */
    ina3221_interface_debug_print("ina3221: ina3221_set_shunt_voltage_sum_limit/ina3221_get_shunt_voltage_sum_limit test.\n");
    
    /* bit 0 is reserved, keep it zero */
    reg = (uint16_t)((rand() % 0x8000U) << 1);
    res = ina3221_set_shunt_voltage_sum_limit(&gs_handle, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set shunt voltage sum limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set shunt voltage sum limit 0x%04X.\n", reg);
    res = ina3221_get_shunt_voltage_sum_limit(&gs_handle, &reg_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get shunt voltage sum limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check shunt voltage sum limit 0x%04X %s.\n", reg, (reg == reg_check) ? "ok" : "error");
    
    /* ina3221_set_power_valid_upper_limit/ina3221_get_power_valid_upper_limit test */
    ina3221_interface_debug_print("ina3221: ina3221_set_power_valid_upper_limit/ina3221_get_power_valid_upper_limit test.\n");
    
    /* bits 2-0 are reserved, keep them zero */
    reg = (uint16_t)((rand() % 0x1000) << 3);
    res = ina3221_set_power_valid_upper_limit(&gs_handle, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set power valid upper limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set power valid upper limit 0x%04X.\n", reg);
    res = ina3221_get_power_valid_upper_limit(&gs_handle, &reg_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get power valid upper limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check power valid upper limit 0x%04X %s.\n", reg, (reg == reg_check) ? "ok" : "error");
    
    /* ina3221_set_power_valid_lower_limit/ina3221_get_power_valid_lower_limit test */
    ina3221_interface_debug_print("ina3221: ina3221_set_power_valid_lower_limit/ina3221_get_power_valid_lower_limit test.\n");
    
    /* bits 2-0 are reserved, keep them zero */
    reg = (uint16_t)((rand() % 0x1000) << 3);
    res = ina3221_set_power_valid_lower_limit(&gs_handle, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set power valid lower limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: set power valid lower limit 0x%04X.\n", reg);
    res = ina3221_get_power_valid_lower_limit(&gs_handle, &reg_check);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get power valid lower limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check power valid lower limit 0x%04X %s.\n", reg, (reg == reg_check) ? "ok" : "error");
    
    /* ina3221_set_channel_summation/ina3221_get_channel_summation test */
    ina3221_interface_debug_print("ina3221: ina3221_set_channel_summation/ina3221_get_channel_summation test.\n");
    
    /* channel 1 disable */
    res = ina3221_set_channel_summation(&gs_handle, INA3221_CHANNEL_1, INA3221_BOOL_FALSE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: channel 1 summation disable.\n");
    res = ina3221_get_channel_summation(&gs_handle, INA3221_CHANNEL_1, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel 1 summation %s.\n", (enable == INA3221_BOOL_FALSE) ? "ok" : "error");
    
    /* channel 1 enable */
    res = ina3221_set_channel_summation(&gs_handle, INA3221_CHANNEL_1, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: channel 1 summation enable.\n");
    res = ina3221_get_channel_summation(&gs_handle, INA3221_CHANNEL_1, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel 1 summation %s.\n", (enable == INA3221_BOOL_TRUE) ? "ok" : "error");
    
    /* channel 2 disable */
    res = ina3221_set_channel_summation(&gs_handle, INA3221_CHANNEL_2, INA3221_BOOL_FALSE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: channel 2 summation disable.\n");
    res = ina3221_get_channel_summation(&gs_handle, INA3221_CHANNEL_2, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel 2 summation %s.\n", (enable == INA3221_BOOL_FALSE) ? "ok" : "error");
    
    /* channel 2 enable */
    res = ina3221_set_channel_summation(&gs_handle, INA3221_CHANNEL_2, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: channel 2 summation enable.\n");
    res = ina3221_get_channel_summation(&gs_handle, INA3221_CHANNEL_2, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel 2 summation %s.\n", (enable == INA3221_BOOL_TRUE) ? "ok" : "error");
    
    /* channel 3 disable */
    res = ina3221_set_channel_summation(&gs_handle, INA3221_CHANNEL_3, INA3221_BOOL_FALSE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: channel 3 summation disable.\n");
    res = ina3221_get_channel_summation(&gs_handle, INA3221_CHANNEL_3, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel 3 summation %s.\n", (enable == INA3221_BOOL_FALSE) ? "ok" : "error");
    
    /* channel 3 enable */
    res = ina3221_set_channel_summation(&gs_handle, INA3221_CHANNEL_3, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: channel 3 summation enable.\n");
    res = ina3221_get_channel_summation(&gs_handle, INA3221_CHANNEL_3, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check channel 3 summation %s.\n", (enable == INA3221_BOOL_TRUE) ? "ok" : "error");
    
    /* ina3221_set_warning_alert_latch/ina3221_get_warning_alert_latch test */
    ina3221_interface_debug_print("ina3221: ina3221_set_warning_alert_latch/ina3221_get_warning_alert_latch test.\n");
    
    /* transparent */
    res = ina3221_set_warning_alert_latch(&gs_handle, INA3221_BOOL_FALSE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set warning alert latch failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: disable warning alert latch.\n");
    res = ina3221_get_warning_alert_latch(&gs_handle, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get warning alert latch failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check warning alert latch %s.\n", (enable == INA3221_BOOL_FALSE) ? "ok" : "error");
    
    /* latch */
    res = ina3221_set_warning_alert_latch(&gs_handle, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set warning alert latch failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: enable warning alert latch.\n");
    res = ina3221_get_warning_alert_latch(&gs_handle, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get warning alert latch failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check warning alert latch %s.\n", (enable == INA3221_BOOL_TRUE) ? "ok" : "error");
    
    /* ina3221_set_critical_alert_latch/ina3221_get_critical_alert_latch test */
    ina3221_interface_debug_print("ina3221: ina3221_set_critical_alert_latch/ina3221_get_critical_alert_latch test.\n");
    
    /* transparent */
    res = ina3221_set_critical_alert_latch(&gs_handle, INA3221_BOOL_FALSE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set critical alert latch failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: disable critical alert latch.\n");
    res = ina3221_get_critical_alert_latch(&gs_handle, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get critical alert latch failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check critical alert latch %s.\n", (enable == INA3221_BOOL_FALSE) ? "ok" : "error");
    
    /* latch */
    res = ina3221_set_critical_alert_latch(&gs_handle, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set critical alert latch failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: enable critical alert latch.\n");
    res = ina3221_get_critical_alert_latch(&gs_handle, &enable);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: get critical alert latch failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check critical alert latch %s.\n", (enable == INA3221_BOOL_TRUE) ? "ok" : "error");
    
    /* ina3221_shunt_voltage_limit_convert_to_register/ina3221_shunt_voltage_limit_convert_to_data test */
    ina3221_interface_debug_print("ina3221: ina3221_shunt_voltage_limit_convert_to_register/ina3221_shunt_voltage_limit_convert_to_data test.\n");
    
    data_f = (float)(rand() % 32000) / 100.0f - 160.0f;
    res = ina3221_shunt_voltage_limit_convert_to_register(&gs_handle, data_f, &reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: shunt voltage limit convert to register failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    res = ina3221_shunt_voltage_limit_convert_to_data(&gs_handle, reg, &data_check_f);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: shunt voltage limit convert to data failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check shunt voltage limit %0.2f -> %0.2f %s.\n", data_f, data_check_f,
                                 (fabsf(data_f - data_check_f) <= 0.1f) ? "ok" : "error");
    
    /* ina3221_shunt_voltage_sum_limit_convert_to_register/ina3221_shunt_voltage_sum_limit_convert_to_data test */
    ina3221_interface_debug_print("ina3221: ina3221_shunt_voltage_sum_limit_convert_to_register/ina3221_shunt_voltage_sum_limit_convert_to_data test.\n");
    
    data_f = (float)(rand() % 120000) / 100.0f - 600.0f;
    res = ina3221_shunt_voltage_sum_limit_convert_to_register(&gs_handle, data_f, &reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: shunt voltage sum limit convert to register failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    res = ina3221_shunt_voltage_sum_limit_convert_to_data(&gs_handle, reg, &data_check_f);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: shunt voltage sum limit convert to data failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check shunt voltage sum limit %0.2f -> %0.2f %s.\n", data_f, data_check_f,
                                 (fabsf(data_f - data_check_f) <= 0.1f) ? "ok" : "error");
    
    /* ina3221_power_limit_convert_to_register/ina3221_power_limit_convert_to_data test */
    ina3221_interface_debug_print("ina3221: ina3221_power_limit_convert_to_register/ina3221_power_limit_convert_to_data test.\n");
    
    data_f = (float)(rand() % 320000) / 10.0f;
    res = ina3221_power_limit_convert_to_register(&gs_handle, data_f, &reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: power limit convert to register failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    res = ina3221_power_limit_convert_to_data(&gs_handle, reg, &data_check_f);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: power limit convert to data failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: check power limit %0.2f -> %0.2f %s.\n", data_f, data_check_f,
                                 (fabsf(data_f - data_check_f) <= 16.0f) ? "ok" : "error");
    
    /* ina3221_soft_reset test */
    ina3221_interface_debug_print("ina3221: ina3221_soft_reset test.\n");
    
    res = ina3221_soft_reset(&gs_handle);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: soft reset failed.\n");
        (void)ina3221_deinit(&gs_handle);
       
        return 1;
    }
    ina3221_interface_debug_print("ina3221: soft reset check %s.\n", (res == 0) ? "ok" : "error");
    
    /* finish register test */
    (void)ina3221_deinit(&gs_handle);
    ina3221_interface_debug_print("ina3221: finish register test.\n");
    
    return 0;
}
