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
 * @file      driver_ina3221_interrupt.c
 * @brief     driver ina3221 interrupt source file
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

#include "driver_ina3221_interrupt.h"

static ina3221_handle_t gs_handle;        /**< ina3221 handle */

/**
 * @brief  interrupt irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t ina3221_interrupt_irq_handler(void)
{
    if (ina3221_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     interrupt example init
 * @param[in] addr_pin iic address pin
 * @param[in] r reference resistor value
 * @param[in] critical_alert_limit_mv critical alert limit in mv
 * @param[in] warning_alert_limit_mv warning alert limit in mv
 * @param[in] shunt_voltage_sum_limit_mv shunt voltage sum limit in mv
 * @param[in] power_valid_upper_limit_mv power valid upper limit in mv
 * @param[in] power_valid_lower_limit_mv power valid lower limit in mv
 * @param[in] *callback pointer to a callback function
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ina3221_interrupt_init(ina3221_address_t addr_pin, float r,
                               float critical_alert_limit_mv,
                               float warning_alert_limit_mv,
                               float shunt_voltage_sum_limit_mv,
                               float power_valid_upper_limit_mv,
                               float power_valid_lower_limit_mv,
                               void (*callback)(uint8_t type))
{
    uint8_t res;
    uint16_t reg;
    
    /* link interface function */
    DRIVER_INA3221_LINK_INIT(&gs_handle, ina3221_handle_t);
    DRIVER_INA3221_LINK_IIC_INIT(&gs_handle, ina3221_interface_iic_init);
    DRIVER_INA3221_LINK_IIC_DEINIT(&gs_handle, ina3221_interface_iic_deinit);
    DRIVER_INA3221_LINK_IIC_READ(&gs_handle, ina3221_interface_iic_read);
    DRIVER_INA3221_LINK_IIC_WRITE(&gs_handle, ina3221_interface_iic_write);
    DRIVER_INA3221_LINK_DELAY_MS(&gs_handle, ina3221_interface_delay_ms);
    DRIVER_INA3221_LINK_DEBUG_PRINT(&gs_handle, ina3221_interface_debug_print);
    DRIVER_INA3221_LINK_RECEIVE_CALLBACK(&gs_handle, callback);
    
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
    
    /* set default adc average */
    res = ina3221_set_adc_average(&gs_handle, INA3221_INTERRUPT_DEFAULT_ADC_AVERAGE);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set adc average failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default bus voltage conversion time */
    res = ina3221_set_bus_voltage_conversion_time(&gs_handle, INA3221_INTERRUPT_DEFAULT_BUS_VOLTAGE_CONVERSION_TIME);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set bus voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default shunt voltage conversion time */
    res = ina3221_set_shunt_voltage_conversion_time(&gs_handle, INA3221_INTERRUPT_DEFAULT_SHUNT_VOLTAGE_CONVERSION_TIME);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set shunt voltage conversion time failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default channel 1 summation */
    res = ina3221_set_channel_summation(&gs_handle, INA3221_CHANNEL_1, INA3221_INTERRUPT_DEFAULT_CHANNEL1_SUMMATION);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default channel 1 summation */
    res = ina3221_set_channel_summation(&gs_handle, INA3221_CHANNEL_2, INA3221_INTERRUPT_DEFAULT_CHANNEL2_SUMMATION);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default channel 1 summation */
    res = ina3221_set_channel_summation(&gs_handle, INA3221_CHANNEL_3, INA3221_INTERRUPT_DEFAULT_CHANNEL3_SUMMATION);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set channel summation failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default warning alert latch */
    res = ina3221_set_warning_alert_latch(&gs_handle, INA3221_INTERRUPT_DEFAULT_WARNING_ALERT_LATCH);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set warning alert latch failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default critical alert latch */
    res = ina3221_set_critical_alert_latch(&gs_handle, INA3221_INTERRUPT_DEFAULT_CRITICAL_ALERT_LATCH);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set critical alert latch failed.\n");
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
    
    /* start continuous */
    res = ina3221_set_mode(&gs_handle, INA3221_MODE_SHUNT_BUS_VOLTAGE_CONTINUOUS);
    if (res != 0)
    {
        ina3221_interface_debug_print("ina3221: set mode failed.\n");
        (void)ina3221_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief      interrupt example read
 * @param[in]  channel input channel
 * @param[out] *mV pointer to a mV buffer
 * @param[out] *mA pointer to a mA buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ina3221_interrupt_read(ina3221_channel_t channel, float *mV, float *mA)
{
    uint8_t res;
    int16_t raw;
    
    /* read bus voltage */
    res = ina3221_read_bus_voltage(&gs_handle, channel, &raw, mV);
    if (res != 0)
    {
        return 1;
    }
    
    /* read current */
    res = ina3221_read_current(&gs_handle, channel, &raw, mA);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  interrupt example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ina3221_interrupt_deinit(void)
{
    uint8_t res;
    
    /* set power down */
    res = ina3221_set_mode(&gs_handle, INA3221_MODE_POWER_DOWN);
    if (res != 0)
    {
        return 1;
    }
    
    /* deinit */
    res = ina3221_deinit(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      interrupt example read shunt voltage sum
 * @param[out] *mV pointer to a mV buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ina3221_interrupt_read_shunt_voltage_sum(float *mV)
{
    uint8_t res;
    int16_t raw;
    
    /* read shunt voltage sum */
    res = ina3221_read_shunt_voltage_sum(&gs_handle, &raw, mV);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}
