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
 * @file      driver_ina3221_interrupt_test.c
 * @brief     driver ina3221 interrupt test source file
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

#include "driver_ina3221_interrupt_test.h"

static ina3221_handle_t gs_handle;        /**< ina3221 handle */

/**
 * @brief     interface receive callback
 * @param[in] type irq type
 * @note      none
 */
static void a_callback(uint8_t type)
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

/**
 * @brief  interrupt test irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t ina3221_interrupt_test_irq_handler(void)
{
    if (ina3221_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     interrupt test
 * @param[in] addr_pin iic device address
 * @param[in] r extern resistance
 * @param[in] critical_alert_limit_mv critical alert limit in mv
 * @param[in] warning_alert_limit_mv warning alert limit in mv
 * @param[in] shunt_voltage_sum_limit_mv shunt voltage sum limit in mv
 * @param[in] power_valid_upper_limit_mv power valid upper limit in mv
 * @param[in] power_valid_lower_limit_mv power valid lower limit in mv
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ina3221_interrupt_test(ina3221_address_t addr_pin, float r,
                               float critical_alert_limit_mv,
                               float warning_alert_limit_mv,
                               float shunt_voltage_sum_limit_mv,
                               float power_valid_upper_limit_mv,
                               float power_valid_lower_limit_mv,
                               uint32_t times)
{
    uint8_t res;
    uint32_t i;
    uint16_t reg;
    ina3221_info_t info;
    
    /* link interface function */
    DRIVER_INA3221_LINK_INIT(&gs_handle, ina3221_handle_t);
    DRIVER_INA3221_LINK_IIC_INIT(&gs_handle, ina3221_interface_iic_init);
    DRIVER_INA3221_LINK_IIC_DEINIT(&gs_handle, ina3221_interface_iic_deinit);
    DRIVER_INA3221_LINK_IIC_READ(&gs_handle, ina3221_interface_iic_read);
    DRIVER_INA3221_LINK_IIC_WRITE(&gs_handle, ina3221_interface_iic_write);
    DRIVER_INA3221_LINK_DELAY_MS(&gs_handle, ina3221_interface_delay_ms);
    DRIVER_INA3221_LINK_DEBUG_PRINT(&gs_handle, ina3221_interface_debug_print);
    DRIVER_INA3221_LINK_RECEIVE_CALLBACK(&gs_handle, a_callback);
    
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
    
    /* start interrupt test */
    ina3221_interface_debug_print("ina3221: start interrupt test.\n");
    
    /* set addr pin */
    res = ina3221_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set addr pin failed.\n");
       
        return 1;
    }

    /* set the r */
    res = ina3221_set_resistance(&gs_handle, r);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set resistance failed.\n");
       
        return 1;
    }
    
    /* init */
    res = ina3221_init(&gs_handle);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: init failed.\n");
       
        return 1;
    }
    
    /* set power down */
    res = ina3221_set_mode(&gs_handle, INA3221_MODE_POWER_DOWN);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable channel 1 */
    res = ina3221_set_channel(&gs_handle, INA3221_CHANNEL_1, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable channel 2 */
    res = ina3221_set_channel(&gs_handle, INA3221_CHANNEL_2, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable channel 3 */
    res = ina3221_set_channel(&gs_handle, INA3221_CHANNEL_3, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set adc average 1 */
    res = ina3221_set_adc_average(&gs_handle, INA3221_ADC_AVERAGE_1);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set bus voltage conversion time 1.1ms */
    res = ina3221_set_bus_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_1P1_MS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set shunt voltage conversion time 1.1ms */
    res = ina3221_set_shunt_voltage_conversion_time(&gs_handle, INA3221_CONVERSION_TIME_1P1_MS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable channel 1 summation */
    res = ina3221_set_channel_summation(&gs_handle, INA3221_CHANNEL_1, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable channel 1 summation */
    res = ina3221_set_channel_summation(&gs_handle, INA3221_CHANNEL_2, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable channel 1 summation */
    res = ina3221_set_channel_summation(&gs_handle, INA3221_CHANNEL_3, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable warning alert latch */
    res = ina3221_set_warning_alert_latch(&gs_handle, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set warning alert latch failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable critical alert latch */
    res = ina3221_set_critical_alert_latch(&gs_handle, INA3221_BOOL_TRUE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set critical alert latch failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* triggered */
    res = ina3221_set_mode(&gs_handle, INA3221_MODE_SHUNT_BUS_VOLTAGE_CONTINUOUS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* shunt voltage limit convert to register */
    res = ina3221_shunt_voltage_limit_convert_to_register(&gs_handle, critical_alert_limit_mv, &reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: shunt voltage limit convert to register failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set critical alert limit */
    res = ina3221_set_critical_alert_limit(&gs_handle, INA3221_CHANNEL_1, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set critical alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set critical alert limit */
    res = ina3221_set_critical_alert_limit(&gs_handle, INA3221_CHANNEL_2, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set critical alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set critical alert limit */
    res = ina3221_set_critical_alert_limit(&gs_handle, INA3221_CHANNEL_3, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set critical alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* shunt voltage limit convert to register */
    res = ina3221_shunt_voltage_limit_convert_to_register(&gs_handle, warning_alert_limit_mv, &reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: shunt voltage limit convert to register failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set warning alert limit */
    res = ina3221_set_warning_alert_limit(&gs_handle, INA3221_CHANNEL_1, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set warning alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set warning alert limit */
    res = ina3221_set_warning_alert_limit(&gs_handle, INA3221_CHANNEL_2, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set warning alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set warning alert limit */
    res = ina3221_set_warning_alert_limit(&gs_handle, INA3221_CHANNEL_3, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set warning alert limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* shunt voltage sum limit convert to register */
    res = ina3221_shunt_voltage_sum_limit_convert_to_register(&gs_handle, shunt_voltage_sum_limit_mv, &reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: shunt voltage sum limit convert to register failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set shunt voltage sum limit */
    res = ina3221_set_shunt_voltage_sum_limit(&gs_handle, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set shunt voltage sum limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* power limit convert to register */
    res = ina3221_power_limit_convert_to_register(&gs_handle, power_valid_upper_limit_mv, &reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: power limit convert to register failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set power valid upper limit */
    res = ina3221_set_power_valid_upper_limit(&gs_handle, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set power valid upper limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* power limit convert to register */
    res = ina3221_power_limit_convert_to_register(&gs_handle, power_valid_lower_limit_mv, &reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: power limit convert to register failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set power valid lower limit */
    res = ina3221_set_power_valid_lower_limit(&gs_handle, reg);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set power valid lower limit failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float mV;
        float mA;
        
        /* read shunt voltage */
        res = ina3221_read_shunt_voltage(&gs_handle, INA3221_CHANNEL_1, &raw, &mV);
        if (res != 0)
        {
            ina3221_interface_debug_print("ina3221: read shunt voltage failed.\n");
            (void)ina3221_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ina3221_interface_debug_print("ina3221: shunt voltage channel1 is %f mV.\n", mV);
        
        /* read shunt voltage */
        res = ina3221_read_shunt_voltage(&gs_handle, INA3221_CHANNEL_2, &raw, &mV);
        if (res != 0)
        {
            ina3221_interface_debug_print("ina3221: read shunt voltage failed.\n");
            (void)ina3221_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ina3221_interface_debug_print("ina3221: shunt voltage channel2 is %f mV.\n", mV);
        
        /* read shunt voltage */
        res = ina3221_read_shunt_voltage(&gs_handle, INA3221_CHANNEL_3, &raw, &mV);
        if (res != 0)
        {
            ina3221_interface_debug_print("ina3221: read shunt voltage failed.\n");
            (void)ina3221_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ina3221_interface_debug_print("ina3221: shunt voltage channel3 is %f mV.\n", mV);
        
        /* read bus voltage */
        res = ina3221_read_bus_voltage(&gs_handle, INA3221_CHANNEL_1, &raw, &mV);
        if (res != 0)
        {
            ina3221_interface_debug_print("ina3221: read bus voltage failed.\n");
            (void)ina3221_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ina3221_interface_debug_print("ina3221: bus voltage channel1 is %f mV.\n", mV);
        
        /* read bus voltage */
        res = ina3221_read_bus_voltage(&gs_handle, INA3221_CHANNEL_2, &raw, &mV);
        if (res != 0)
        {
            ina3221_interface_debug_print("ina3221: read bus voltage failed.\n");
            (void)ina3221_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ina3221_interface_debug_print("ina3221: bus voltage channel2 is %f mV.\n", mV);
        
        /* read bus voltage */
        res = ina3221_read_bus_voltage(&gs_handle, INA3221_CHANNEL_3, &raw, &mV);
        if (res != 0)
        {
            ina3221_interface_debug_print("ina3221: read bus voltage failed.\n");
            (void)ina3221_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ina3221_interface_debug_print("ina3221: bus voltage channel3 is %f mV.\n", mV);
        
        /* read current */
        res = ina3221_read_current(&gs_handle, INA3221_CHANNEL_1, &raw, &mA);
        if (res != 0)
        {
            ina3221_interface_debug_print("ina3221: read current failed.\n");
            (void)ina3221_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ina3221_interface_debug_print("ina3221: current channel1 is %f mA.\n", mA);
        
        /* read current */
        res = ina3221_read_current(&gs_handle, INA3221_CHANNEL_2, &raw, &mA);
        if (res != 0)
        {
            ina3221_interface_debug_print("ina3221: read current failed.\n");
            (void)ina3221_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ina3221_interface_debug_print("ina3221: current channel2 is %f mA.\n", mA);
        
        /* read current */
        res = ina3221_read_current(&gs_handle, INA3221_CHANNEL_3, &raw, &mA);
        if (res != 0)
        {
            ina3221_interface_debug_print("ina3221: read current failed.\n");
            (void)ina3221_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ina3221_interface_debug_print("ina3221: current channel3 is %f mA.\n", mA);
        
        /* read shunt voltage sum */
        res = ina3221_read_shunt_voltage_sum(&gs_handle, &raw, &mV);
        if (res != 0)
        {
            ina3221_interface_debug_print("ina3221: read shunt voltage sum failed.\n");
            (void)ina3221_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        ina3221_interface_debug_print("ina3221: shunt voltage sum is %f mV.\n", mV);
        
        /* delay 1000ms */
        ina3221_interface_delay_ms(1000);
    }
    
    /* set power down */
    res = ina3221_set_mode(&gs_handle, INA3221_MODE_POWER_DOWN);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish interrupt test */
    (void)ina3221_deinit(&gs_handle);
    ina3221_interface_debug_print("ina3221: finish interrupt test.\n");
    
    return 0;
}
