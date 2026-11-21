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
 * @file      driver_ina3221_shot.c
 * @brief     driver ina3221 shot source file
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

#ifndef DRIVER_INA3221_SHOT_H
#define DRIVER_INA3221_SHOT_H

#include "driver_ina3221_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup ina3221_example_driver
 * @{
 */

/**
 * @brief ina3221 shot example default definition
 */
#define INA3221_SHOT_DEFAULT_ADC_AVERAGE                          INA3221_ADC_AVERAGE_1                 /**< 1 sample */
#define INA3221_SHOT_DEFAULT_BUS_VOLTAGE_CONVERSION_TIME          INA3221_CONVERSION_TIME_1P1_MS        /**< 1.1ms */
#define INA3221_SHOT_DEFAULT_SHUNT_VOLTAGE_CONVERSION_TIME        INA3221_CONVERSION_TIME_1P1_MS        /**< 1.1ms */
#define INA3221_SHOT_DEFAULT_CHANNEL1_SUMMATION                   INA3221_BOOL_TRUE                     /**< enable */
#define INA3221_SHOT_DEFAULT_CHANNEL2_SUMMATION                   INA3221_BOOL_TRUE                     /**< enable */
#define INA3221_SHOT_DEFAULT_CHANNEL3_SUMMATION                   INA3221_BOOL_TRUE                     /**< enable */
#define INA3221_SHOT_DEFAULT_WARNING_ALERT_LATCH                  INA3221_BOOL_TRUE                     /**< enable */
#define INA3221_SHOT_DEFAULT_CRITICAL_ALERT_LATCH                 INA3221_BOOL_TRUE                     /**< enable */

/**
 * @brief     shot example init
 * @param[in] addr_pin iic address pin
 * @param[in] r reference resistor value
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ina3221_shot_init(ina3221_address_t addr_pin, float r);

/**
 * @brief  shot example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ina3221_shot_deinit(void);

/**
 * @brief      shot example read
 * @param[in]  channel input channel
 * @param[out] *mV pointer to a mV buffer
 * @param[out] *mA pointer to a mA buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ina3221_shot_read(ina3221_channel_t channel, float *mV, float *mA);

/**
 * @brief      shot example read shunt voltage sum
 * @param[out] *mV pointer to a mV buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ina3221_shot_read_shunt_voltage_sum(float *mV);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
