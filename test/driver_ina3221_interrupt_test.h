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

#ifndef DRIVER_INA3221_INTERRUPT_TEST_H
#define DRIVER_INA3221_INTERRUPT_TEST_H

#include "driver_ina3221_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup ina3221_test_driver
 * @{
 */

/**
 * @brief  interrupt test irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t ina3221_interrupt_test_irq_handler(void);

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
                               uint32_t times);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
