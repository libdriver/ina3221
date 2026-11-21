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
 * @file      driver_ina3221.h
 * @brief     driver ina3221 header file
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

#ifndef DRIVER_INA3221_H
#define DRIVER_INA3221_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ina3221_driver ina3221 driver function
 * @brief    ina3221 driver modules
 * @{
 */

/**
 * @addtogroup ina3221_basic_driver
 * @{
 */

/**
 * @brief ina3221 bool enumeration definition
 */
typedef enum
{
    INA3221_BOOL_FALSE = 0x00,        /**< false */
    INA3221_BOOL_TRUE  = 0x01,        /**< true */
} ina3221_bool_t;

/**
 * @brief ina3221 address enumeration definition
 */
typedef enum
{
    INA3221_ADDRESS_0 = (0x40 << 1),        /**< A0 = GND */
    INA3221_ADDRESS_1 = (0x41 << 1),        /**< A0 = VS+ */
    INA3221_ADDRESS_2 = (0x42 << 1),        /**< A0 = SDA */
    INA3221_ADDRESS_3 = (0x43 << 1),        /**< A0 = SCL */
} ina3221_address_t;

/**
 * @brief ina3221 channel enumeration definition
 */
typedef enum
{
    INA3221_CHANNEL_1 = 0,        /**< channel 1 */
    INA3221_CHANNEL_2 = 1,        /**< channel 2 */
    INA3221_CHANNEL_3 = 2,        /**< channel 3 */
} ina3221_channel_t;

/**
 * @brief ina3221 conversion time enumeration definition
 */
typedef enum
{
    INA3221_CONVERSION_TIME_140_US   = 0,        /**< 140μs */
    INA3221_CONVERSION_TIME_204_US   = 1,        /**< 204μs */
    INA3221_CONVERSION_TIME_332_US   = 2,        /**< 332μs */
    INA3221_CONVERSION_TIME_588_US   = 3,        /**< 588μs */
    INA3221_CONVERSION_TIME_1P1_MS   = 4,        /**< 1.1ms */
    INA3221_CONVERSION_TIME_2P116_MS = 5,        /**< 2.116ms */
    INA3221_CONVERSION_TIME_4P156_MS = 6,        /**< 4.156ms */
    INA3221_CONVERSION_TIME_8P244_MS = 7,        /**< 8.244ms */
} ina3221_conversion_time_t;

/**
 * @brief ina3221 adc average enumeration definition
 */
typedef enum
{
    INA3221_ADC_AVERAGE_1    = 0x0,        /**< 1 sample */
    INA3221_ADC_AVERAGE_4    = 0x1,        /**< 4 samples */
    INA3221_ADC_AVERAGE_16   = 0x2,        /**< 16 samples */
    INA3221_ADC_AVERAGE_64   = 0x3,        /**< 64 samples */
    INA3221_ADC_AVERAGE_128  = 0x4,        /**< 128 samples */
    INA3221_ADC_AVERAGE_256  = 0x5,        /**< 256 samples */
    INA3221_ADC_AVERAGE_512  = 0x6,        /**< 512 samples */
    INA3221_ADC_AVERAGE_1024 = 0x7,        /**< 1024 samples */
} ina3221_adc_average_t;

/**
 * @brief ina3221 mode enumeration definition
 */
typedef enum
{
    INA3221_MODE_POWER_DOWN                   = 0x0,        /**< power down */
    INA3221_MODE_SHUNT_VOLTAGE_TRIGGERED      = 0x1,        /**< shunt voltage triggered */
    INA3221_MODE_BUS_VOLTAGE_TRIGGERED        = 0x2,        /**< bus voltage triggered */
    INA3221_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED  = 0x3,        /**< shunt and bus triggered */
    INA3221_MODE_SHUNT_VOLTAGE_CONTINUOUS     = 0x5,        /**< shunt voltage continuous */
    INA3221_MODE_BUS_VOLTAGE_CONTINUOUS       = 0x6,        /**< bus voltage continuous */
    INA3221_MODE_SHUNT_BUS_VOLTAGE_CONTINUOUS = 0x7,        /**< shunt and bus voltage continuous */
} ina3221_mode_t;

/**
 * @brief ina3221 status enumeration definition
 */
typedef enum
{
    INA3221_STATUS_CRITICAL_ALERT_CHANNEL_1 = 9,        /**< critical alert channel 1 status */
    INA3221_STATUS_CRITICAL_ALERT_CHANNEL_2 = 8,        /**< critical alert channel 2 status */
    INA3221_STATUS_CRITICAL_ALERT_CHANNEL_3 = 7,        /**< critical alert channel 3 status */
    INA3221_STATUS_SUMMATION_ALERT          = 6,        /**< summation alert status */
    INA3221_STATUS_WARNING_ALERT_CHANNEL_1  = 5,        /**< warning alert channel 1 status */
    INA3221_STATUS_WARNING_ALERT_CHANNEL_2  = 4,        /**< warning alert channel 2 status */
    INA3221_STATUS_WARNING_ALERT_CHANNEL_3  = 3,        /**< warning alert channel 3 status */
    INA3221_STATUS_POWER_VALID_ALERT        = 2,        /**< power valid alert status */
    INA3221_STATUS_TIMING_CONTROL_ALERT     = 1,        /**< timing control alert status */
} ina3221_status_t;

/**
 * @brief ina3221 handle structure definition
 */
typedef struct ina3221_handle_s
{
    uint8_t iic_addr;                                                                   /**< iic device address */
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    void (*receive_callback)(uint8_t type);                                             /**< point to a receive_callback function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    float r;                                                                            /**< resistance */
    uint8_t inited;                                                                     /**< inited flag */
} ina3221_handle_t;

/**
 * @brief ina3221 information structure definition
 */
typedef struct ina3221_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} ina3221_info_t;

/**
 * @}
 */

/**
 * @defgroup ina3221_link_driver ina3221 link driver function
 * @brief    ina3221 link driver modules
 * @ingroup  ina3221_driver
 * @{
 */

/**
 * @brief     initialize ina3221_handle_t structure
 * @param[in] HANDLE pointer to an ina3221 handle structure
 * @param[in] STRUCTURE ina3221_handle_t
 * @note      none
 */
#define DRIVER_INA3221_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE pointer to an ina3221 handle structure
 * @param[in] FUC pointer to an iic_init function address
 * @note      none
 */
#define DRIVER_INA3221_LINK_IIC_INIT(HANDLE, FUC)           (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE pointer to an ina3221 handle structure
 * @param[in] FUC pointer to an iic_deinit function address
 * @note      none
 */
#define DRIVER_INA3221_LINK_IIC_DEINIT(HANDLE, FUC)         (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE pointer to an ina3221 handle structure
 * @param[in] FUC pointer to an iic_read function address
 * @note      none
 */
#define DRIVER_INA3221_LINK_IIC_READ(HANDLE, FUC)           (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE pointer to an ina3221 handle structure
 * @param[in] FUC pointer to an iic_write function address
 * @note      none
 */
#define DRIVER_INA3221_LINK_IIC_WRITE(HANDLE, FUC)          (HANDLE)->iic_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to an ina3221 handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_INA3221_LINK_DELAY_MS(HANDLE, FUC)           (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to an ina3221 handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_INA3221_LINK_DEBUG_PRINT(HANDLE, FUC)        (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE pointer to an ina3221 handle structure
 * @param[in] FUC pointer to a receive_callback function address
 * @note      none
 */
#define DRIVER_INA3221_LINK_RECEIVE_CALLBACK(HANDLE, FUC)   (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup ina3221_basic_driver ina3221 basic driver function
 * @brief    ina3221 basic driver modules
 * @ingroup  ina3221_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info pointer to an ina3221 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ina3221_info(ina3221_info_t *info);

/**
 * @brief     set the iic address pin
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ina3221_set_addr_pin(ina3221_handle_t *handle, ina3221_address_t addr_pin);

/**
 * @brief      get the iic address pin
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[out] *addr_pin pointer to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ina3221_get_addr_pin(ina3221_handle_t *handle, ina3221_address_t *addr_pin);

/**
 * @brief     set the resistance
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] resistance current sampling resistance value
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ina3221_set_resistance(ina3221_handle_t *handle, float resistance);

/**
 * @brief      get the resistance
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[out] *resistance pointer to a current sampling resistance value buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ina3221_get_resistance(ina3221_handle_t *handle, float *resistance);

/**
 * @brief     irq handler
 * @param[in] *handle pointer to an ina3221 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina3221_irq_handler(ina3221_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to an ina3221 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 id is invalid
 *            - 5 soft reset failed
 * @note      none
 */
uint8_t ina3221_init(ina3221_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to an ina3221 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t ina3221_deinit(ina3221_handle_t *handle);

/**
 * @brief      read the shunt voltage
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  channel input channel
 * @param[out] *raw pointer to raw data buffer
 * @param[out] *mV pointer to converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read shunt voltage failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid channel
 * @note       none
 */
uint8_t ina3221_read_shunt_voltage(ina3221_handle_t *handle, ina3221_channel_t channel, int16_t *raw, float *mV);

/**
 * @brief      read the bus voltage
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  channel input channel
 * @param[out] *raw pointer to raw data buffer
 * @param[out] *mV pointer to converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read bus voltage failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid channel
 * @note       none
 */
uint8_t ina3221_read_bus_voltage(ina3221_handle_t *handle, ina3221_channel_t channel, int16_t *raw, float *mV);

/**
 * @brief      read the current
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  channel input channel
 * @param[out] *raw pointer to raw data buffer
 * @param[out] *mA pointer to converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read current failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid channel
 *             - 5 r is zero
 * @note       none
 */
uint8_t ina3221_read_current(ina3221_handle_t *handle, ina3221_channel_t channel, int16_t *raw, float *mA);

/**
 * @brief      read the shunt voltage sum
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[out] *raw pointer to raw data buffer
 * @param[out] *mV pointer to converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read shunt voltage sum failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_read_shunt_voltage_sum(ina3221_handle_t *handle, int16_t *raw, float *mV);

/**
 * @brief     soft reset the chip
 * @param[in] *handle pointer to an ina3221 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 soft reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina3221_soft_reset(ina3221_handle_t *handle);

/**
 * @brief     set channel
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] channel input channel
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set channel failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid channel
 * @note      none
 */
uint8_t ina3221_set_channel(ina3221_handle_t *handle, ina3221_channel_t channel, ina3221_bool_t enable);

/**
 * @brief      get channel
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  channel input channel
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get channel failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid channel
 * @note       none
 */
uint8_t ina3221_get_channel(ina3221_handle_t *handle, ina3221_channel_t channel, ina3221_bool_t *enable);

/**
 * @brief     set adc average
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] average adc average
 * @return    status code
 *            - 0 success
 *            - 1 set adc average failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina3221_set_adc_average(ina3221_handle_t *handle, ina3221_adc_average_t average);

/**
 * @brief      get adc average
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[out] *average pointer to an adc average buffer
 * @return     status code
 *             - 0 success
 *             - 1 get adc average failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_get_adc_average(ina3221_handle_t *handle, ina3221_adc_average_t *average);

/**
 * @brief     set bus voltage conversion time
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] t conversion time
 * @return    status code
 *            - 0 success
 *            - 1 set bus voltage conversion time failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina3221_set_bus_voltage_conversion_time(ina3221_handle_t *handle, ina3221_conversion_time_t t);

/**
 * @brief      get bus voltage conversion time
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[out] *t pointer to a conversion time buffer
 * @return     status code
 *             - 0 success
 *             - 1 get bus voltage conversion time failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_get_bus_voltage_conversion_time(ina3221_handle_t *handle, ina3221_conversion_time_t *t);

/**
 * @brief     set shunt voltage conversion time
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] t conversion time
 * @return    status code
 *            - 0 success
 *            - 1 set shunt voltage conversion time failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina3221_set_shunt_voltage_conversion_time(ina3221_handle_t *handle, ina3221_conversion_time_t t);

/**
 * @brief      get shunt voltage conversion time
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[out] *t pointer to a conversion time buffer
 * @return     status code
 *             - 0 success
 *             - 1 get shunt voltage conversion time failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_get_shunt_voltage_conversion_time(ina3221_handle_t *handle, ina3221_conversion_time_t *t);

/**
 * @brief     set mode
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] mode adc mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina3221_set_mode(ina3221_handle_t *handle, ina3221_mode_t mode);

/**
 * @brief      get mode
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[out] *mode pointer to an adc mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_get_mode(ina3221_handle_t *handle, ina3221_mode_t *mode);

/**
 * @brief     set critical alert limit
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] channel input channel
 * @param[in] reg input reg
 * @return    status code
 *            - 0 success
 *            - 1 set critical alert limit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid channel
 * @note      none
 */
uint8_t ina3221_set_critical_alert_limit(ina3221_handle_t *handle, ina3221_channel_t channel, uint16_t reg);

/**
 * @brief      get critical alert limit
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  channel input channel
 * @param[out] *reg pointer to a reg buffer
 * @return     status code
 *             - 0 success
 *             - 1 get critical alert limit failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid channel
 * @note       none
 */
uint8_t ina3221_get_critical_alert_limit(ina3221_handle_t *handle, ina3221_channel_t channel, uint16_t *reg);

/**
 * @brief     set warning alert limit
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] channel input channel
 * @param[in] reg input reg
 * @return    status code
 *            - 0 success
 *            - 1 set warning alert limit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid channel
 * @note      none
 */
uint8_t ina3221_set_warning_alert_limit(ina3221_handle_t *handle, ina3221_channel_t channel, uint16_t reg);

/**
 * @brief      get warning alert limit
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  channel input channel
 * @param[out] *reg pointer to a reg buffer
 * @return     status code
 *             - 0 success
 *             - 1 get warning alert limit failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid channel
 * @note       none
 */
uint8_t ina3221_get_warning_alert_limit(ina3221_handle_t *handle, ina3221_channel_t channel, uint16_t *reg);

/**
 * @brief     set shunt voltage sum limit
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] reg input reg
 * @return    status code
 *            - 0 success
 *            - 1 set shunt voltage sum limit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina3221_set_shunt_voltage_sum_limit(ina3221_handle_t *handle, uint16_t reg);

/**
 * @brief      get shunt voltage sum limit
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[out] *reg pointer to a reg buffer
 * @return     status code
 *             - 0 success
 *             - 1 get shunt voltage sum limit failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_get_shunt_voltage_sum_limit(ina3221_handle_t *handle, uint16_t *reg);

/**
 * @brief     set power valid upper limit
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] reg input reg
 * @return    status code
 *            - 0 success
 *            - 1 set power valid upper limit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina3221_set_power_valid_upper_limit(ina3221_handle_t *handle, uint16_t reg);

/**
 * @brief      get power valid upper limit
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[out] *reg pointer to a reg buffer
 * @return     status code
 *             - 0 success
 *             - 1 get power valid upper limit failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_get_power_valid_upper_limit(ina3221_handle_t *handle, uint16_t *reg);

/**
 * @brief     set power valid lower limit
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] reg input reg
 * @return    status code
 *            - 0 success
 *            - 1 set power valid lower limit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina3221_set_power_valid_lower_limit(ina3221_handle_t *handle, uint16_t reg);

/**
 * @brief      get power valid lower limit
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[out] *reg pointer to a reg buffer
 * @return     status code
 *             - 0 success
 *             - 1 get power valid lower limit failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_get_power_valid_lower_limit(ina3221_handle_t *handle, uint16_t *reg);

/**
 * @brief     set channel summation
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] channel input channel
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set channel summation failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid channel
 * @note      none
 */
uint8_t ina3221_set_channel_summation(ina3221_handle_t *handle, ina3221_channel_t channel, ina3221_bool_t enable);

/**
 * @brief      get channel summation
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  channel input channel
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get channel summation failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid channel
 * @note       none
 */
uint8_t ina3221_get_channel_summation(ina3221_handle_t *handle, ina3221_channel_t channel, ina3221_bool_t *enable);

/**
 * @brief     enable or disable warning alert latch
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set warning alert latch failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina3221_set_warning_alert_latch(ina3221_handle_t *handle, ina3221_bool_t enable);

/**
 * @brief      get warning alert latch status
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get warning alert latch failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_get_warning_alert_latch(ina3221_handle_t *handle, ina3221_bool_t *enable);

/**
 * @brief     enable or disable critical alert latch
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set critical alert latch failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina3221_set_critical_alert_latch(ina3221_handle_t *handle, ina3221_bool_t enable);

/**
 * @brief      get critical alert latch status
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get critical alert latch failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_get_critical_alert_latch(ina3221_handle_t *handle, ina3221_bool_t *enable);

/**
 * @brief      convert the shunt voltage limit to the register raw data
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  mV input voltage
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_shunt_voltage_limit_convert_to_register(ina3221_handle_t *handle, float mV, uint16_t *reg);

/**
 * @brief      convert the register raw data to the shunt voltage limit
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  reg register raw data
 * @param[out] *mV pointer to a voltage buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_shunt_voltage_limit_convert_to_data(ina3221_handle_t *handle, uint16_t reg, float *mV);

/**
 * @brief      convert the shunt voltage sum limit to the register raw data
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  mV input voltage
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_shunt_voltage_sum_limit_convert_to_register(ina3221_handle_t *handle, float mV, uint16_t *reg);

/**
 * @brief      convert the register raw data to the shunt voltage sum limit
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  reg register raw data
 * @param[out] *mV pointer to a voltage buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_shunt_voltage_sum_limit_convert_to_data(ina3221_handle_t *handle, uint16_t reg, float *mV);

/**
 * @brief      convert the power limit to the register raw data
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  mV input voltage
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_power_limit_convert_to_register(ina3221_handle_t *handle, float mV, uint16_t *reg);

/**
 * @brief      convert the register raw data to the power limit
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  reg register raw data
 * @param[out] *mV pointer to a voltage buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_power_limit_convert_to_data(ina3221_handle_t *handle, uint16_t reg, float *mV);

/**
 * @}
 */

/**
 * @defgroup ina3221_extern_driver ina3221 extern driver function
 * @brief    ina3221 extern driver modules
 * @ingroup  ina3221_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] reg register address
 * @param[in] data written data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ina3221_set_reg(ina3221_handle_t *handle, uint8_t reg, uint16_t data);

/**
 * @brief      get the chip register
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  reg register address
 * @param[out] *data pointer to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ina3221_get_reg(ina3221_handle_t *handle, uint8_t reg, uint16_t *data);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
