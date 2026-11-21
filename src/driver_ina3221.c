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
 * @file      driver_ina3221.c
 * @brief     driver ina3221 source file
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

#include "driver_ina3221.h" 
#include <float.h>
#include <math.h>

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Texas Instruments INA3221"        /**< chip name */
#define MANUFACTURER_NAME         "Texas Instruments"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.7f                               /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                               /**< chip max supply voltage */
#define MAX_CURRENT               0.45f                              /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                             /**< chip min operating temperature */
#define TEMPERATURE_MAX           125.0f                             /**< chip max operating temperature */
#define DRIVER_VERSION            1000                               /**< driver version */

/**
 * @brief chip register definition
 */
#define INA3221_REG_CONF                                  0x00        /**< configuration register */
#define INA3221_REG_CHANNEL_1_SHUNT_VOLTAGE               0x01        /**< channel 1 shunt voltage register */
#define INA3221_REG_CHANNEL_1_BUS_VOLTAGE                 0x02        /**< channel 1 bus voltage register */
#define INA3221_REG_CHANNEL_2_SHUNT_VOLTAGE               0x03        /**< channel 2 shunt voltage register */
#define INA3221_REG_CHANNEL_2_BUS_VOLTAGE                 0x04        /**< channel 2 bus voltage register */
#define INA3221_REG_CHANNEL_3_SHUNT_VOLTAGE               0x05        /**< channel 3 shunt voltage register */
#define INA3221_REG_CHANNEL_3_BUS_VOLTAGE                 0x06        /**< channel 3 bus voltage register */
#define INA3221_REG_CHANNEL_1_CRITICAL_ALERT_LIMIT        0x07        /**< channel 1 critical alert limit register  */
#define INA3221_REG_CHANNEL_1_WARNING_ALERT_LIMIT         0x08        /**< channel 1 warning alert limit register */
#define INA3221_REG_CHANNEL_2_CRITICAL_ALERT_LIMIT        0x09        /**< channel 2 critical alert limit register  */
#define INA3221_REG_CHANNEL_2_WARNING_ALERT_LIMIT         0x0A        /**< channel 2 warning alert limit register */
#define INA3221_REG_CHANNEL_3_CRITICAL_ALERT_LIMIT        0x0B        /**< channel 3 critical alert limit register */
#define INA3221_REG_CHANNEL_3_WARNING_ALERT_LIMIT         0x0C        /**< channel 3 warning alert limit register */
#define INA3221_REG_SHUNT_VOLTAGE_SUM                     0x0D        /**< shunt voltage sum register */
#define INA3221_REG_SHUNT_VOLTAGE_SUM_LIMIT               0x0E        /**< shunt voltage sum limit register */
#define INA3221_REG_ENABLE_MASK                           0x0F        /**< enable mask register */
#define INA3221_REG_POWER_VALID_UPPER_LIMIT               0x10        /**< power valid upper limit register */
#define INA3221_REG_POWER_VALID_LOWER_LIMIT               0x11        /**< power valid lower limit register */
#define INA3221_REG_MANUFACTURER_ID                       0xFE        /**< manufacturer id register */
#define INA3221_REG_DIE_ID                                0xFF        /**< die id register */

/**
 * @brief      iic interface read bytes
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[in]  reg iic register address
 * @param[out] *data pointer to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_ina3221_iic_read(ina3221_handle_t *handle, uint8_t reg, uint16_t *data)
{
    uint8_t buf[2];
    
    memset(buf, 0, sizeof(uint8_t) * 2);                                        /* clear the buffer */
    if (handle->iic_read(handle->iic_addr, reg, (uint8_t *)buf, 2) != 0)        /* read data */
    {
        return 1;                                                               /* return error */
    }
    else
    {
        *data = (uint16_t)buf[0] << 8 | buf[1];                                 /* get data */
        
        return 0;                                                               /* success return 0 */
    }
}

/**
 * @brief     iic interface write bytes
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] reg iic register address
 * @param[in] data written data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ina3221_iic_write(ina3221_handle_t *handle, uint8_t reg, uint16_t data)
{
    uint8_t buf[2];
    
    buf[0] = (uint8_t)((data >> 8) & 0xFF);                                      /* get msb */
    buf[1] = (uint8_t)((data >> 0) & 0xFF);                                      /* get lsb */
    if (handle->iic_write(handle->iic_addr, reg, (uint8_t *)buf, 2) != 0)        /* write data */
    {
        return 1;                                                                /* return error */
    }
    
    return 0;                                                                    /* success return 0 */
}

/**
 * @brief     set the resistance
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] resistance current sampling resistance value
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ina3221_set_resistance(ina3221_handle_t *handle, float resistance)
{
    if (handle == NULL)            /* check handle */
    {
        return 2;                  /* return error */
    }
    
    handle->r = resistance;        /* set resistance */
    
    return 0;                      /* success return 0 */
}

/**
 * @brief      get the resistance
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[out] *resistance pointer to a current sampling resistance value buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ina3221_get_resistance(ina3221_handle_t *handle, float *resistance)
{
    if (handle == NULL)            /* check handle */
    {
        return 2;                  /* return error */
    }
    
    *resistance = handle->r;       /* get resistance */
    
    return 0;                      /* success return 0 */
}

/**
 * @brief     set the iic address pin
 * @param[in] *handle pointer to an ina3221 handle structure
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ina3221_set_addr_pin(ina3221_handle_t *handle, ina3221_address_t addr_pin)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr_pin;        /* set pin */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the iic address pin
 * @param[in]  *handle pointer to an ina3221 handle structure
 * @param[out] *addr_pin pointer to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ina3221_get_addr_pin(ina3221_handle_t *handle, ina3221_address_t *addr_pin)
{
    if (handle == NULL)                                      /* check handle */
    {
        return 2;                                            /* return error */
    }
    
    *addr_pin = (ina3221_address_t)(handle->iic_addr);        /* get pin */
    
    return 0;                                                /* success return 0 */
}

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
uint8_t ina3221_soft_reset(ina3221_handle_t *handle)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
       
        return 1;                                                                 /* return error */
    }
    prev &= ~(1 << 15);                                                           /* clear soft reset */
    prev |= 1 << 15;                                                              /* set soft reset */
    res = a_ina3221_iic_write(handle, INA3221_REG_CONF, (uint16_t )prev);         /* write config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ina3221: write conf register failed.\n");            /* write conf register failed */
       
        return 1;                                                                 /* return error */
    }
    handle->delay_ms(10);                                                         /* delay 10ms */
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t ina3221_set_channel(ina3221_handle_t *handle, ina3221_channel_t channel, ina3221_bool_t enable)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    if (channel == INA3221_CHANNEL_1)                                                 /* channel 1 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
           
            return 1;                                                                 /* return error */
        }
        prev &= ~(1 << 14);                                                           /* clear settings */
        prev |= enable << 14;                                                         /* set bool */
        res = a_ina3221_iic_write(handle, INA3221_REG_CONF, (uint16_t )prev);         /* write config */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("ina3221: write conf register failed.\n");            /* write conf register failed */
           
            return 1;                                                                 /* return error */
        }
        
        return 0;                                                                     /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_2)
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
           
            return 1;                                                                 /* return error */
        }
        prev &= ~(1 << 13);                                                           /* clear settings */
        prev |= enable << 13;                                                         /* set bool */
        res = a_ina3221_iic_write(handle, INA3221_REG_CONF, (uint16_t )prev);         /* write config */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("ina3221: write conf register failed.\n");            /* write conf register failed */
           
            return 1;                                                                 /* return error */
        }
        
        return 0;                                                                     /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_3)
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
           
            return 1;                                                                 /* return error */
        }
        prev &= ~(1 << 12);                                                           /* clear settings */
        prev |= enable << 12;                                                         /* set bool */
        res = a_ina3221_iic_write(handle, INA3221_REG_CONF, (uint16_t )prev);         /* write config */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("ina3221: write conf register failed.\n");            /* write conf register failed */
           
            return 1;                                                                 /* return error */
        }
        
        return 0;                                                                     /* success return 0 */
    }
    else
    {
        handle->debug_print("ina3221: invalid channel.\n");                           /* invalid channel */
       
        return 4;                                                                     /* return error */
    }
}

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
uint8_t ina3221_get_channel(ina3221_handle_t *handle, ina3221_channel_t channel, ina3221_bool_t *enable)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    if (channel == INA3221_CHANNEL_1)                                                 /* channel 1 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
           
            return 1;                                                                 /* return error */
        }
        *enable = (ina3221_bool_t)((prev >> 14) & 0x01);                              /* set bool */
        
        return 0;                                                                     /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_2)
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
           
            return 1;                                                                 /* return error */
        }
        *enable = (ina3221_bool_t)((prev >> 13) & 0x01);                              /* set bool */
        
        return 0;                                                                     /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_3)
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
           
            return 1;                                                                 /* return error */
        }
        *enable = (ina3221_bool_t)((prev >> 12) & 0x01);                              /* set bool */
        
        return 0;                                                                     /* success return 0 */
    }
    else
    {
        handle->debug_print("ina3221: invalid channel.\n");                           /* invalid channel */
       
        return 4;                                                                     /* return error */
    }
}

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
uint8_t ina3221_set_adc_average(ina3221_handle_t *handle, ina3221_adc_average_t average)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
       
        return 1;                                                                 /* return error */
    }
    prev &= ~(0x07 << 9);                                                         /* clear settings */
    prev |= average << 9;                                                         /* set average */
    res = a_ina3221_iic_write(handle, INA3221_REG_CONF, (uint16_t )prev);         /* write config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ina3221: write conf register failed.\n");            /* write conf register failed */
       
        return 1;                                                                 /* return error */
    }
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t ina3221_get_adc_average(ina3221_handle_t *handle, ina3221_adc_average_t *average)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
       
        return 1;                                                                 /* return error */
    }
    *average = (ina3221_adc_average_t)((prev >> 9) & 0x07);                       /* set average */
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t ina3221_set_bus_voltage_conversion_time(ina3221_handle_t *handle, ina3221_conversion_time_t t)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
       
        return 1;                                                                 /* return error */
    }
    prev &= ~(0x07 << 6);                                                         /* clear settings */
    prev |= t << 6;                                                               /* set time */
    res = a_ina3221_iic_write(handle, INA3221_REG_CONF, (uint16_t )prev);         /* write config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ina3221: write conf register failed.\n");            /* write conf register failed */
       
        return 1;                                                                 /* return error */
    }
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t ina3221_get_bus_voltage_conversion_time(ina3221_handle_t *handle, ina3221_conversion_time_t *t)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
       
        return 1;                                                                 /* return error */
    }
    *t = (ina3221_conversion_time_t)((prev >> 6) & 0x07);                         /* set time */
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t ina3221_set_shunt_voltage_conversion_time(ina3221_handle_t *handle, ina3221_conversion_time_t t)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
       
        return 1;                                                                 /* return error */
    }
    prev &= ~(0x07 << 3);                                                         /* clear settings */
    prev |= t << 3;                                                               /* set time */
    res = a_ina3221_iic_write(handle, INA3221_REG_CONF, (uint16_t )prev);         /* write config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ina3221: write conf register failed.\n");            /* write conf register failed */
       
        return 1;                                                                 /* return error */
    }
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t ina3221_get_shunt_voltage_conversion_time(ina3221_handle_t *handle, ina3221_conversion_time_t *t)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
       
        return 1;                                                                 /* return error */
    }
    *t = (ina3221_conversion_time_t)((prev >> 3) & 0x07);                         /* set time */
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t ina3221_set_mode(ina3221_handle_t *handle, ina3221_mode_t mode)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
       
        return 1;                                                                 /* return error */
    }
    prev &= ~(0x07 << 0);                                                         /* clear settings */
    prev |= mode << 0;                                                            /* set mode */
    res = a_ina3221_iic_write(handle, INA3221_REG_CONF, (uint16_t )prev);         /* write config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ina3221: write conf register failed.\n");            /* write conf register failed */
       
        return 1;                                                                 /* return error */
    }
    
    if ((mode >= INA3221_MODE_SHUNT_VOLTAGE_TRIGGERED) &&                         /* check mode */
        (mode <= INA3221_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED))
    {
        uint8_t channel_count;
        uint8_t avg_index;
        uint8_t vbusct_index;
        uint8_t vshct_index;
        uint32_t total_us;
        uint32_t wait_ms;
        const uint16_t conversion_time_us[8] =
        {
            140, 204, 332, 588, 1100, 2116, 4156, 8244
        };                                                                        /* conversion timetable in us (Table 8-4) */
        const uint16_t average_sample[8] =
        {
            1, 4, 16, 64, 128, 256, 512, 1024
        };                                                                        /* average sample count table (Table 8-4) */

        channel_count = (uint8_t)(((prev >> 14) & 0x01) +                         /* count enabled channels */
                                  ((prev >> 13) & 0x01) +
                                  ((prev >> 12) & 0x01));
        avg_index = (uint8_t)((prev >> 9) & 0x07);                                /* get averaging mode */
        vbusct_index = (uint8_t)((prev >> 6) & 0x07);                             /* get bus conversion time index */
        vshct_index = (uint8_t)((prev >> 3) & 0x07);                              /* get shunt conversion time index */

        if (mode == INA3221_MODE_SHUNT_VOLTAGE_TRIGGERED)                         /* shunt only */
        {
            total_us = (uint32_t)channel_count * average_sample[avg_index] *
                        conversion_time_us[vshct_index];
        }
        else if (mode == INA3221_MODE_BUS_VOLTAGE_TRIGGERED)                      /* bus only */
        {
            total_us = (uint32_t)channel_count * average_sample[avg_index] *
                        conversion_time_us[vbusct_index];
        }
        else                                                                      /* shunt and bus */
        {
            total_us = (uint32_t)channel_count * average_sample[avg_index] *
                       (conversion_time_us[vshct_index] + 
                        conversion_time_us[vbusct_index]);                        /* both */
        }

        wait_ms = (total_us + total_us / 10 + 999) / 1000;                        /* add 10% margin, convert us to ms */
        handle->delay_ms(wait_ms);                                                /* wait for all conversions to complete */

        return 0;                                                                 /* success return 0 */
    }
    else
    {
        return 0;                                                                 /* success return 0 */
    }
}

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
uint8_t ina3221_get_mode(ina3221_handle_t *handle, ina3221_mode_t *mode)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);        /* read config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("ina3221: read conf register failed.\n");             /* read conf register failed */
       
        return 1;                                                                 /* return error */
    }
    *mode = (ina3221_mode_t)((prev >> 0) & 0x07);                                 /* set mode */
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t ina3221_read_shunt_voltage(ina3221_handle_t *handle, ina3221_channel_t channel, int16_t *raw, float *mV)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }
    
    if (channel == INA3221_CHANNEL_1)                                                                    /* channel 1 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_1_SHUNT_VOLTAGE, (uint16_t *)&prev);        /* read shunt voltage */
        if (res != 0)                                                                                    /* check result */
        {
            handle->debug_print("ina3221: read shunt voltage register failed.\n");                       /* read shunt voltage register failed */
           
            return 1;                                                                                    /* return error */
        }
        if ((prev & 0x8000U) != 0)                                                                       /* check msb */
        {
            *raw = (int16_t)((prev >> 3) | ((uint16_t)0x7 << 13));                                       /* convert data */
        }
        else
        {
            *raw = (int16_t)(prev >> 3);                                                                 /* convert data */
        }
        *mV = (float)(*raw) * 0.04f;                                                                     /* convert raw data */
        
        return 0;                                                                                        /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_2)                                                               /* channel 2 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_2_SHUNT_VOLTAGE, (uint16_t *)&prev);        /* read shunt voltage */
        if (res != 0)                                                                                    /* check result */
        {
            handle->debug_print("ina3221: read shunt voltage register failed.\n");                       /* read shunt voltage register failed */
           
            return 1;                                                                                    /* return error */
        }
        if ((prev & 0x8000U) != 0)                                                                       /* check msb */
        {
            *raw = (int16_t)((prev >> 3) | ((uint16_t)0x7 << 13));                                       /* convert data */
        }
        else
        {
            *raw = (int16_t)(prev >> 3);                                                                 /* convert data */
        }
        *mV = (float)(*raw) * 0.04f;                                                                     /* convert raw data */
        
        return 0;                                                                                        /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_3)                                                               /* channel 3 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_3_SHUNT_VOLTAGE, (uint16_t *)&prev);        /* read shunt voltage */
        if (res != 0)                                                                                    /* check result */
        {
            handle->debug_print("ina3221: read shunt voltage register failed.\n");                       /* read shunt voltage register failed */
           
            return 1;                                                                                    /* return error */
        }
        if ((prev & 0x8000U) != 0)                                                                       /* check msb */
        {
            *raw = (int16_t)((prev >> 3) | ((uint16_t)0x7 << 13));                                       /* convert data */
        }
        else
        {
            *raw = (int16_t)(prev >> 3);                                                                 /* convert data */
        }
        *mV = (float)(*raw) * 0.04f;                                                                     /* convert raw data */
        
        return 0;                                                                                        /* success return 0 */
    }
    else
    {
        handle->debug_print("ina3221: invalid channel.\n");                                              /* invalid channel */
       
        return 4;                                                                                        /* return error */
    }
}

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
uint8_t ina3221_read_bus_voltage(ina3221_handle_t *handle, ina3221_channel_t channel, int16_t *raw, float *mV)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                                                /* check handle */
    {
        return 2;                                                                                      /* return error */
    }
    if (handle->inited != 1)                                                                           /* check handle initialization */
    {
        return 3;                                                                                      /* return error */
    }
    
    if (channel == INA3221_CHANNEL_1)                                                                  /* channel 1 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_1_BUS_VOLTAGE, (uint16_t *)&prev);        /* read bus voltage */
        if (res != 0)                                                                                  /* check result */
        {
            handle->debug_print("ina3221: read bus voltage register failed.\n");                       /* bus shunt voltage register failed */
           
            return 1;                                                                                  /* return error */
        }
        if ((prev & 0x8000U) != 0)                                                                     /* check msb */
        {
            *raw = (int16_t)((prev >> 3) | ((uint16_t)0x7 << 13));                                     /* convert data */
        }
        else
        {
            *raw = (int16_t)(prev >> 3);                                                               /* convert data */
        }
        *mV = (float)(*raw) * 8.0f;                                                                    /* convert raw data */
        
        return 0;                                                                                      /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_2)                                                             /* channel 2 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_2_BUS_VOLTAGE, (uint16_t *)&prev);        /* read bus voltage */
        if (res != 0)                                                                                  /* check result */
        {
            handle->debug_print("ina3221: read bus voltage register failed.\n");                       /* bus shunt voltage register failed */
           
            return 1;                                                                                  /* return error */
        }
        if ((prev & 0x8000U) != 0)                                                                     /* check msb */
        {
            *raw = (int16_t)((prev >> 3) | ((uint16_t)0x7 << 13));                                     /* convert data */
        }
        else
        {
            *raw = (int16_t)(prev >> 3);                                                               /* convert data */
        }
        *mV = (float)(*raw) * 8.0f;                                                                    /* convert raw data */
        
        return 0;                                                                                      /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_3)                                                             /* channel 3 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_3_BUS_VOLTAGE, (uint16_t *)&prev);        /* read bus voltage */
        if (res != 0)                                                                                  /* check result */
        {
            handle->debug_print("ina3221: read bus voltage register failed.\n");                       /* bus shunt voltage register failed */
           
            return 1;                                                                                  /* return error */
        }
        if ((prev & 0x8000U) != 0)                                                                     /* check msb */
        {
            *raw = (int16_t)((prev >> 3) | ((uint16_t)0x7 << 13));                                     /* convert data */
        }
        else
        {
            *raw = (int16_t)(prev >> 3);                                                               /* convert data */
        }
        *mV = (float)(*raw) * 8.0f;                                                                    /* convert raw data */
        
        return 0;                                                                                      /* success return 0 */
    }
    else
    {
        handle->debug_print("ina3221: invalid channel.\n");                                            /* invalid channel */
       
        return 4;                                                                                      /* return error */
    }
}

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
uint8_t ina3221_read_current(ina3221_handle_t *handle, ina3221_channel_t channel, int16_t *raw, float *mA)
{
    uint8_t res;
    uint16_t prev;
    float mV;
   
    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }
    if (fabsf(handle->r) < FLT_MIN)                                                                      /* check zero */
    {
        handle->debug_print("ina3221: r is zero.\n");                                                    /* r is zero */
       
        return 5;                                                                                        /* return error */
    }
    
    if (channel == INA3221_CHANNEL_1)                                                                    /* channel 1 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_1_SHUNT_VOLTAGE, (uint16_t *)&prev);        /* read shunt voltage */
        if (res != 0)                                                                                    /* check result */
        {
            handle->debug_print("ina3221: read shunt voltage register failed.\n");                       /* read shunt voltage register failed */
           
            return 1;                                                                                    /* return error */
        }
        if ((prev & 0x8000U) != 0)                                                                       /* check msb */
        {
            *raw = (int16_t)((prev >> 3) | ((uint16_t)0x7 << 13));                                       /* convert data */
        }
        else
        {
            *raw = (int16_t)(prev >> 3);                                                                 /* convert data */
        }
        mV = (float)(*raw) * 0.04f;                                                                      /* convert raw data */
        *mA = mV / handle->r;                                                                            /* convert the current */
        
        return 0;                                                                                        /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_2)                                                               /* channel 2 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_2_SHUNT_VOLTAGE, (uint16_t *)&prev);        /* read shunt voltage */
        if (res != 0)                                                                                    /* check result */
        {
            handle->debug_print("ina3221: read shunt voltage register failed.\n");                       /* read shunt voltage register failed */
           
            return 1;                                                                                    /* return error */
        }
        if ((prev & 0x8000U) != 0)                                                                       /* check msb */
        {
            *raw = (int16_t)((prev >> 3) | ((uint16_t)0x7 << 13));                                       /* convert data */
        }
        else
        {
            *raw = (int16_t)(prev >> 3);                                                                 /* convert data */
        }
        mV = (float)(*raw) * 0.04f;                                                                      /* convert raw data */
        *mA = mV / handle->r;                                                                            /* convert the current */
        
        return 0;                                                                                        /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_3)                                                               /* channel 3 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_3_SHUNT_VOLTAGE, (uint16_t *)&prev);        /* read shunt voltage */
        if (res != 0)                                                                                    /* check result */
        {
            handle->debug_print("ina3221: read shunt voltage register failed.\n");                       /* read shunt voltage register failed */
           
            return 1;                                                                                    /* return error */
        }
        if ((prev & 0x8000U) != 0)                                                                       /* check msb */
        {
            *raw = (int16_t)((prev >> 3) | ((uint16_t)0x7 << 13));                                       /* convert data */
        }
        else
        {
            *raw = (int16_t)(prev >> 3);                                                                 /* convert data */
        }
        mV = (float)(*raw) * 0.04f;                                                                      /* convert raw data */
        *mA = mV / handle->r;                                                                            /* convert the current */
        
        return 0;                                                                                        /* success return 0 */
    }
    else
    {
        handle->debug_print("ina3221: invalid channel.\n");                                              /* invalid channel */
       
        return 4;                                                                                        /* return error */
    }
}

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
uint8_t ina3221_set_critical_alert_limit(ina3221_handle_t *handle, ina3221_channel_t channel, uint16_t reg)
{
    uint8_t res;
   
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }
    
    if (channel == INA3221_CHANNEL_1)                                                              /* channel 1 */
    {
        res = a_ina3221_iic_write(handle, INA3221_REG_CHANNEL_1_CRITICAL_ALERT_LIMIT, reg);        /* write critical alert limit */
        if (res != 0)                                                                              /* check result */
        {
            handle->debug_print("ina3221: write critical alert limit register failed.\n");         /* write critical alert limit register failed */
           
            return 1;                                                                              /* return error */
        }
        
        return 0;                                                                                  /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_2)                                                         /* channel 2 */
    {
        res = a_ina3221_iic_write(handle, INA3221_REG_CHANNEL_2_CRITICAL_ALERT_LIMIT, reg);        /* write critical alert limit */
        if (res != 0)                                                                              /* check result */
        {
            handle->debug_print("ina3221: write critical alert limit register failed.\n");         /* write critical alert limit register failed */
           
            return 1;                                                                              /* return error */
        }
        
        return 0;                                                                                  /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_3)                                                         /* channel 3 */
    {
        res = a_ina3221_iic_write(handle, INA3221_REG_CHANNEL_3_CRITICAL_ALERT_LIMIT, reg);        /* write critical alert limit */
        if (res != 0)                                                                              /* check result */
        {
            handle->debug_print("ina3221: write critical alert limit register failed.\n");         /* write critical alert limit register failed */
           
            return 1;                                                                              /* return error */
        }
        
        return 0;                                                                                  /* success return 0 */
    }
    else
    {
        handle->debug_print("ina3221: invalid channel.\n");                                        /* invalid channel */
       
        return 4;                                                                                  /* return error */
    }
}

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
uint8_t ina3221_get_critical_alert_limit(ina3221_handle_t *handle, ina3221_channel_t channel, uint16_t *reg)
{
    uint8_t res;
   
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    if (channel == INA3221_CHANNEL_1)                                                             /* channel 1 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_1_CRITICAL_ALERT_LIMIT, reg);        /* read critical alert limit */
        if (res != 0)                                                                             /* check result */
        {
            handle->debug_print("ina3221: read critical alert limit register failed.\n");         /* read critical alert limit register failed */
           
            return 1;                                                                             /* return error */
        }
        
        return 0;                                                                                 /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_2)                                                        /* channel 2 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_2_CRITICAL_ALERT_LIMIT, reg);        /* read critical alert limit */
        if (res != 0)                                                                             /* check result */
        {
            handle->debug_print("ina3221: read critical alert limit register failed.\n");         /* read critical alert limit register failed */
           
            return 1;                                                                             /* return error */
        }
        
        return 0;                                                                                 /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_3)                                                        /* channel 3 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_3_CRITICAL_ALERT_LIMIT, reg);        /* read critical alert limit */
        if (res != 0)                                                                             /* check result */
        {
            handle->debug_print("ina3221: read critical alert limit register failed.\n");         /* read critical alert limit register failed */
           
            return 1;                                                                             /* return error */
        }
        
        return 0;                                                                                 /* success return 0 */
    }
    else
    {
        handle->debug_print("ina3221: invalid channel.\n");                                       /* invalid channel */
       
        return 4;                                                                                 /* return error */
    }
}

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
uint8_t ina3221_set_warning_alert_limit(ina3221_handle_t *handle, ina3221_channel_t channel, uint16_t reg)
{
    uint8_t res;
   
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }
    
    if (channel == INA3221_CHANNEL_1)                                                              /* channel 1 */
    {
        res = a_ina3221_iic_write(handle, INA3221_REG_CHANNEL_1_WARNING_ALERT_LIMIT, reg);         /* write warning alert limit */
        if (res != 0)                                                                              /* check result */
        {
            handle->debug_print("ina3221: write warning alert limit register failed.\n");          /* write warning alert limit register failed */
           
            return 1;                                                                              /* return error */
        }
        
        return 0;                                                                                  /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_2)                                                         /* channel 2 */
    {
        res = a_ina3221_iic_write(handle, INA3221_REG_CHANNEL_2_WARNING_ALERT_LIMIT, reg);         /* write warning alert limit */
        if (res != 0)                                                                              /* check result */
        {
            handle->debug_print("ina3221: write warning alert limit register failed.\n");          /* write warning alert limit register failed */
           
            return 1;                                                                              /* return error */
        }
        
        return 0;                                                                                  /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_3)                                                         /* channel 3 */
    {
        res = a_ina3221_iic_write(handle, INA3221_REG_CHANNEL_3_WARNING_ALERT_LIMIT, reg);         /* write warning alert limit */
        if (res != 0)                                                                              /* check result */
        {
            handle->debug_print("ina3221: write warning alert limit register failed.\n");          /* write warning alert limit register failed */
           
            return 1;                                                                              /* return error */
        }
        
        return 0;                                                                                  /* success return 0 */
    }
    else
    {
        handle->debug_print("ina3221: invalid channel.\n");                                        /* invalid channel */
       
        return 4;                                                                                  /* return error */
    }
}

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
uint8_t ina3221_get_warning_alert_limit(ina3221_handle_t *handle, ina3221_channel_t channel, uint16_t *reg)
{
    uint8_t res;
   
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    if (channel == INA3221_CHANNEL_1)                                                             /* channel 1 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_1_WARNING_ALERT_LIMIT, reg);         /* read warning alert limit */
        if (res != 0)                                                                             /* check result */
        {
            handle->debug_print("ina3221: read warning alert limit register failed.\n");          /* read warning alert limit register failed */
           
            return 1;                                                                             /* return error */
        }
        
        return 0;                                                                                 /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_2)                                                        /* channel 2 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_2_WARNING_ALERT_LIMIT, reg);         /* read warning alert limit */
        if (res != 0)                                                                             /* check result */
        {
            handle->debug_print("ina3221: read warning alert limit register failed.\n");          /* read warning alert limit register failed */
           
            return 1;                                                                             /* return error */
        }
        
        return 0;                                                                                 /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_3)                                                        /* channel 3 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_CHANNEL_3_WARNING_ALERT_LIMIT, reg);         /* read warning alert limit */
        if (res != 0)                                                                             /* check result */
        {
            handle->debug_print("ina3221: read warning alert limit register failed.\n");          /* read warning alert limit register failed */
           
            return 1;                                                                             /* return error */
        }
        
        return 0;                                                                                 /* success return 0 */
    }
    else
    {
        handle->debug_print("ina3221: invalid channel.\n");                                       /* invalid channel */
       
        return 4;                                                                                 /* return error */
    }
}

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
uint8_t ina3221_read_shunt_voltage_sum(ina3221_handle_t *handle, int16_t *raw, float *mV)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }

    res = a_ina3221_iic_read(handle, INA3221_REG_SHUNT_VOLTAGE_SUM, (uint16_t *)&prev);        /* read shunt voltage sum */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("ina3221: read shunt voltage sum failed.\n");                      /* read shunt voltage sum failed */
       
        return 1;                                                                              /* return error */
    }
    if ((prev & 0x8000U) != 0)                                                                 /* check msb */
    {
        *raw = (int16_t)((prev >> 1) | ((uint16_t)0x1 << 15));                                 /* convert data */
    }
    else
    {
        *raw = (int16_t)(prev >> 1);                                                           /* convert data */
    }
    *mV = (float)(*raw) * 0.04f;                                                               /* convert raw data */
    
    return 0;                                                                                  /* success return 0 */
}

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
uint8_t ina3221_set_shunt_voltage_sum_limit(ina3221_handle_t *handle, uint16_t reg)
{
    uint8_t res;
   
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }

    res = a_ina3221_iic_write(handle, INA3221_REG_SHUNT_VOLTAGE_SUM_LIMIT, reg);        /* write shunt voltage sum limit */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("ina3221: write shunt voltage sum limit failed.\n");        /* write shunt voltage sum limit failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t ina3221_get_shunt_voltage_sum_limit(ina3221_handle_t *handle, uint16_t *reg)
{
    uint8_t res;
   
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }

    res = a_ina3221_iic_read(handle, INA3221_REG_SHUNT_VOLTAGE_SUM_LIMIT, reg);        /* read shunt voltage sum limit */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("ina3221: read shunt voltage sum limit failed.\n");        /* read shunt voltage sum limit failed */
       
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t ina3221_set_power_valid_upper_limit(ina3221_handle_t *handle, uint16_t reg)
{
    uint8_t res;
   
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }

    res = a_ina3221_iic_write(handle, INA3221_REG_POWER_VALID_UPPER_LIMIT, reg);        /* write power valid upper limit */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("ina3221: write power valid upper limit failed.\n");        /* write power valid upper limit failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t ina3221_get_power_valid_upper_limit(ina3221_handle_t *handle, uint16_t *reg)
{
    uint8_t res;
   
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }

    res = a_ina3221_iic_read(handle, INA3221_REG_POWER_VALID_UPPER_LIMIT, reg);        /* read power valid upper limit */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("ina3221: read power valid upper limit failed.\n");        /* read power valid upper limit failed */
       
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t ina3221_set_power_valid_lower_limit(ina3221_handle_t *handle, uint16_t reg)
{
    uint8_t res;
   
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }

    res = a_ina3221_iic_write(handle, INA3221_REG_POWER_VALID_LOWER_LIMIT, reg);        /* write power valid lower limit */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("ina3221: write power valid lower limit failed.\n");        /* write power valid lower limit failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t ina3221_get_power_valid_lower_limit(ina3221_handle_t *handle, uint16_t *reg)
{
    uint8_t res;
   
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }

    res = a_ina3221_iic_read(handle, INA3221_REG_POWER_VALID_LOWER_LIMIT, reg);        /* read power valid lower limit */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("ina3221: read power valid lower limit failed.\n");        /* read power valid lower limit failed */
       
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t ina3221_set_warning_alert_latch(ina3221_handle_t *handle, ina3221_bool_t enable)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_ENABLE_MASK, (uint16_t *)&prev);        /* read enable mask */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("ina3221: read enable mask register failed.\n");             /* read enable mask register failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~(1 << 11);                                                                  /* clear settings */
    prev |= enable << 11;                                                                /* set bool */
    res = a_ina3221_iic_write(handle, INA3221_REG_ENABLE_MASK, (uint16_t )prev);         /* write enable mask */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("ina3221: write enable mask register failed.\n");            /* write enable mask register failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t ina3221_get_warning_alert_latch(ina3221_handle_t *handle, ina3221_bool_t *enable)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_ENABLE_MASK, (uint16_t *)&prev);        /* read enable mask */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("ina3221: read enable mask register failed.\n");             /* read enable mask register failed */
       
        return 1;                                                                        /* return error */
    }
    *enable = (ina3221_bool_t)((prev >> 11) & 0x01);                                     /* get bool */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t ina3221_set_critical_alert_latch(ina3221_handle_t *handle, ina3221_bool_t enable)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_ENABLE_MASK, (uint16_t *)&prev);        /* read enable mask */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("ina3221: read enable mask register failed.\n");             /* read enable mask register failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~(1 << 10);                                                                  /* clear settings */
    prev |= enable << 10;                                                                /* set bool */
    res = a_ina3221_iic_write(handle, INA3221_REG_ENABLE_MASK, (uint16_t )prev);         /* write enable mask */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("ina3221: write enable mask register failed.\n");            /* write enable mask register failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t ina3221_get_critical_alert_latch(ina3221_handle_t *handle, ina3221_bool_t *enable)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_ENABLE_MASK, (uint16_t *)&prev);        /* read enable mask */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("ina3221: read enable mask register failed.\n");             /* read enable mask register failed */
       
        return 1;                                                                        /* return error */
    }
    *enable = (ina3221_bool_t)((prev >> 10) & 0x01);                                     /* get bool */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t ina3221_set_channel_summation(ina3221_handle_t *handle, ina3221_channel_t channel, ina3221_bool_t enable)
{
    uint8_t res;
    uint16_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    if (channel == INA3221_CHANNEL_1)                                                        /* channel 1 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_ENABLE_MASK, (uint16_t *)&prev);        /* read enable mask */
        if (res != 0)                                                                        /* check result */
        {
            handle->debug_print("ina3221: read enable mask register failed.\n");             /* read enable mask register failed */
           
            return 1;                                                                        /* return error */
        }
        prev &= ~(1 << 14);                                                                  /* clear settings */
        prev |= enable << 14;                                                                /* set bool */
        res = a_ina3221_iic_write(handle, INA3221_REG_ENABLE_MASK, (uint16_t )prev);         /* write enable mask */
        if (res != 0)                                                                        /* check result */
        {
            handle->debug_print("ina3221: write enable mask register failed.\n");            /* write enable mask register failed */
           
            return 1;                                                                        /* return error */
        }
        
        return 0;                                                                            /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_2)                                                   /* channel 2 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_ENABLE_MASK, (uint16_t *)&prev);        /* read enable mask */
        if (res != 0)                                                                        /* check result */
        {
            handle->debug_print("ina3221: read enable mask register failed.\n");             /* read enable mask register failed */
           
            return 1;                                                                        /* return error */
        }
        prev &= ~(1 << 13);                                                                  /* clear settings */
        prev |= enable << 13;                                                                /* set bool */
        res = a_ina3221_iic_write(handle, INA3221_REG_ENABLE_MASK, (uint16_t )prev);         /* write enable mask */
        if (res != 0)                                                                        /* check result */
        {
            handle->debug_print("ina3221: write enable mask register failed.\n");            /* write enable mask register failed */
           
            return 1;                                                                        /* return error */
        }
        
        return 0;                                                                            /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_3)                                                   /* channel 3 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_ENABLE_MASK, (uint16_t *)&prev);        /* read enable mask */
        if (res != 0)                                                                        /* check result */
        {
            handle->debug_print("ina3221: read enable mask register failed.\n");             /* read enable mask register failed */
           
            return 1;                                                                        /* return error */
        }
        prev &= ~(1 << 12);                                                                  /* clear settings */
        prev |= enable << 12;                                                                /* set bool */
        res = a_ina3221_iic_write(handle, INA3221_REG_ENABLE_MASK, (uint16_t )prev);         /* write enable mask */
        if (res != 0)                                                                        /* check result */
        {
            handle->debug_print("ina3221: write enable mask register failed.\n");            /* write enable mask register failed */
           
            return 1;                                                                        /* return error */
        }
        
        return 0;                                                                            /* success return 0 */
    }
    else
    {
        handle->debug_print("ina3221: invalid channel.\n");                                  /* invalid channel */
       
        return 4;                                                                            /* return error */
    }
}

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
uint8_t ina3221_get_channel_summation(ina3221_handle_t *handle, ina3221_channel_t channel, ina3221_bool_t *enable)
{
    uint8_t res;
    uint16_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    if (channel == INA3221_CHANNEL_1)                                                        /* channel 1 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_ENABLE_MASK, (uint16_t *)&prev);        /* read enable mask */
        if (res != 0)                                                                        /* check result */
        {
            handle->debug_print("ina3221: read enable mask register failed.\n");             /* read enable mask register failed */
           
            return 1;                                                                        /* return error */
        }
        *enable = (ina3221_bool_t)((prev >> 14) & 0x01);                                     /* set bool */
        
        return 0;                                                                            /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_2)                                                   /* channel 2 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_ENABLE_MASK, (uint16_t *)&prev);        /* read enable mask */
        if (res != 0)                                                                        /* check result */
        {
            handle->debug_print("ina3221: read enable mask register failed.\n");             /* read enable mask register failed */
           
            return 1;                                                                        /* return error */
        }
        *enable = (ina3221_bool_t)((prev >> 13) & 0x01);                                     /* set bool */
        
        return 0;                                                                            /* success return 0 */
    }
    else if (channel == INA3221_CHANNEL_3)                                                   /* channel 3 */
    {
        res = a_ina3221_iic_read(handle, INA3221_REG_ENABLE_MASK, (uint16_t *)&prev);        /* read enable mask */
        if (res != 0)                                                                        /* check result */
        {
            handle->debug_print("ina3221: read enable mask register failed.\n");             /* read enable mask register failed */
           
            return 1;                                                                        /* return error */
        }
        *enable = (ina3221_bool_t)((prev >> 12) & 0x01);                                     /* set bool */
        
        return 0;                                                                            /* success return 0 */
    }
    else
    {
        handle->debug_print("ina3221: invalid channel.\n");                                  /* invalid channel */
       
        return 4;                                                                            /* return error */
    }
}

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
uint8_t ina3221_shunt_voltage_limit_convert_to_register(ina3221_handle_t *handle, float mV, uint16_t *reg)
{
    int16_t raw;
    
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }
    if (handle->inited != 1)            /* check handle initialization */
    {
        return 3;                       /* return error */
    }
    
    raw = (int16_t)(mV / 0.04f);        /* convert real data to register data */
    *reg = raw << 3;                    /* convert real data to register data */
    
    return 0;                           /* success return 0 */
}

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
uint8_t ina3221_shunt_voltage_limit_convert_to_data(ina3221_handle_t *handle, uint16_t reg, float *mV)
{
    int16_t raw;
    
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    if ((reg & 0x8000U) != 0)                                       /* check msb */
    {
        raw = (int16_t)((reg >> 3) | ((uint16_t)0x7 << 13));        /* convert data */
    }
    else
    {
        raw = (int16_t)(reg >> 3);                                  /* convert data */
    }
    *mV = (float)raw * 0.04f;                                       /* convert */
    
    return 0;                                                       /* success return 0 */
}

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
uint8_t ina3221_shunt_voltage_sum_limit_convert_to_register(ina3221_handle_t *handle, float mV, uint16_t *reg)
{
    int16_t raw;
    
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }
    if (handle->inited != 1)            /* check handle initialization */
    {
        return 3;                       /* return error */
    }
    
    raw = (int16_t)(mV / 0.04f);        /* convert real data to register data */
    *reg = raw << 1;                    /* convert real data to register data */
    
    return 0;                           /* success return 0 */
}

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
uint8_t ina3221_shunt_voltage_sum_limit_convert_to_data(ina3221_handle_t *handle, uint16_t reg, float *mV)
{
    int16_t raw;
    
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    if ((reg & 0x8000U) != 0)                                       /* check msb */
    {
        raw = (int16_t)((reg >> 1) | ((uint16_t)0x1 << 15));        /* convert data */
    }
    else
    {
        raw = (int16_t)(reg >> 1);                                  /* convert data */
    }
    *mV = (float)raw * 0.04f;                                       /* convert */
    
    return 0;                                                       /* success return 0 */
}

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
uint8_t ina3221_power_limit_convert_to_register(ina3221_handle_t *handle, float mV, uint16_t *reg)
{
    int16_t raw;
    
    if (handle == NULL)                /* check handle */
    {
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    raw = (int16_t)(mV / 8.0f);        /* convert real data to register data */
    *reg = raw << 3;                   /* convert real data to register data */
    
    return 0;                          /* success return 0 */
}

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
uint8_t ina3221_power_limit_convert_to_data(ina3221_handle_t *handle, uint16_t reg, float *mV)
{
    int16_t raw;
    
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    if ((reg & 0x8000U) != 0)                                       /* check msb */
    {
        raw = (int16_t)((reg >> 3) | ((uint16_t)0x7 << 13));        /* convert data */
    }
    else
    {
        raw = (int16_t)(reg >> 3);                                  /* convert data */
    }
    *mV = (float)raw * 8.0f;                                        /* convert */
    
    return 0;                                                       /* success return 0 */
}

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
uint8_t ina3221_irq_handler(ina3221_handle_t *handle)
{
    uint8_t res;
    uint16_t prev;
    
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_ENABLE_MASK, (uint16_t *)&prev);              /* read enable mask */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("ina3221: read enable mask register failed.\n");                   /* read enable mask register failed */
       
        return 1;                                                                              /* return error */
    }
    
    if (((prev >> 9) & 0x01) != 0)                                                             /* check flag */
    {
        if (handle->receive_callback != NULL)                                                  /* check null */
        {
            handle->receive_callback((uint8_t)INA3221_STATUS_CRITICAL_ALERT_CHANNEL_1);        /* run the callback */
        }
    }
    if (((prev >> 8) & 0x01) != 0)                                                             /* check flag */
    {
        if (handle->receive_callback != NULL)                                                  /* check null */
        {
            handle->receive_callback((uint8_t)INA3221_STATUS_CRITICAL_ALERT_CHANNEL_2);        /* run the callback */
        }
    }
    if (((prev >> 7) & 0x01) != 0)                                                             /* check flag */
    {
        if (handle->receive_callback != NULL)                                                  /* check null */
        {
            handle->receive_callback((uint8_t)INA3221_STATUS_CRITICAL_ALERT_CHANNEL_3);        /* run the callback */
        }
    }
    if (((prev >> 6) & 0x01) != 0)                                                             /* check flag */
    {
        if (handle->receive_callback != NULL)                                                  /* check null */
        {
            handle->receive_callback((uint8_t)INA3221_STATUS_SUMMATION_ALERT);                 /* run the callback */
        }
    }
    if (((prev >> 5) & 0x01) != 0)                                                             /* check flag */
    {
        if (handle->receive_callback != NULL)                                                  /* check null */
        {
            handle->receive_callback((uint8_t)INA3221_STATUS_WARNING_ALERT_CHANNEL_1);         /* run the callback */
        }
    }
    if (((prev >> 4) & 0x01) != 0)                                                             /* check flag */
    {
        if (handle->receive_callback != NULL)                                                  /* check null */
        {
            handle->receive_callback((uint8_t)INA3221_STATUS_WARNING_ALERT_CHANNEL_2);         /* run the callback */
        }
    }
    if (((prev >> 3) & 0x01) != 0)                                                             /* check flag */
    {
        if (handle->receive_callback != NULL)                                                  /* check null */
        {
            handle->receive_callback((uint8_t)INA3221_STATUS_WARNING_ALERT_CHANNEL_3);         /* run the callback */
        }
    }
    if (((prev >> 2) & 0x01) != 0)                                                             /* check flag */
    {
        if (handle->receive_callback != NULL)                                                  /* check null */
        {
            handle->receive_callback((uint8_t)INA3221_STATUS_POWER_VALID_ALERT);               /* run the callback */
        }
    }
    if (((prev >> 1) & 0x01) == 0)                                                             /* check flag */
    {
        if (handle->receive_callback != NULL)                                                  /* check null */
        {
            handle->receive_callback((uint8_t)INA3221_STATUS_TIMING_CONTROL_ALERT);            /* run the callback */
        }
    }
    
    return 0;                                                                                  /* success return 0 */
}

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
uint8_t ina3221_init(ina3221_handle_t *handle)
{
    uint8_t res;
    uint16_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                                         /* check debug_print */
    {
        return 3;                                                                            /* return error */
    }
    if (handle->iic_init == NULL)                                                            /* check iic_init */
    {
        handle->debug_print("ina3221: iic_init is null.\n");                                 /* iic_init is null */
        
        return 3;                                                                            /* return error */
    }
    if (handle->iic_deinit == NULL)                                                          /* check iic_deinit */
    {
        handle->debug_print("ina3221: iic_deinit is null.\n");                               /* iic_deinit is null */
        
        return 3;                                                                            /* return error */
    }
    if (handle->iic_read == NULL)                                                            /* check iic_read */
    {
        handle->debug_print("ina3221: iic_read is null.\n");                                 /* iic_read is null */
        
        return 3;                                                                            /* return error */
    }
    if (handle->iic_write == NULL)                                                           /* check iic_write */
    {
        handle->debug_print("ina3221: iic_write is null.\n");                                /* iic_write is null */
        
        return 3;                                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                                            /* check delay_ms */
    {
        handle->debug_print("ina3221: delay_ms is null.\n");                                 /* delay_ms is null */
        
        return 3;                                                                            /* return error */
    }
    if (handle->receive_callback == NULL)                                                    /* check receive_callback */
    {
        handle->debug_print("ina3221: receive_callback is null.\n");                         /* receive_callback is null */
        
        return 3;                                                                            /* return error */
    }
    
    if (handle->iic_init() != 0)                                                             /* iic init */
    {
        handle->debug_print("ina3221: iic init failed.\n");                                  /* iic init failed */
        
        return 1;                                                                            /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_MANUFACTURER_ID, (uint16_t *)&prev);        /* read manufacturer id */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ina3221: read manufacturer id register failed.\n");             /* read manufacturer id register failed */
        (void)handle->iic_deinit();                                                          /* iic deinit */
        
        return 4;                                                                            /* return error */
    }
    if (prev != 0x5449U)                                                                     /* check id */
    {
        handle->debug_print("ina3221: manufacturer id is invalid.\n");                       /* manufacturer id is invalid */
        (void)handle->iic_deinit();                                                          /* iic deinit */
        
        return 4;                                                                            /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_DIE_ID, (uint16_t *)&prev);                 /* read die id */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ina3221: read die id register failed.\n");                      /* read die id register failed */
        (void)handle->iic_deinit();                                                          /* iic deinit */
        
        return 4;                                                                            /* return error */
    }
    if (prev != 0x3220U)                                                                     /* check id */
    {
        handle->debug_print("ina3221: die id is invalid.\n");                                /* die id is invalid */
        (void)handle->iic_deinit();                                                          /* iic deinit */
        
        return 4;                                                                            /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);                   /* read conf */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ina3221: read conf register failed.\n");                        /* read conf register failed */
        (void)handle->iic_deinit();                                                          /* iic deinit */
       
        return 5;                                                                            /* return error */
    }
    prev |= 1 << 15;                                                                         /* set reset bit */
    res = a_ina3221_iic_write(handle, INA3221_REG_CONF, prev);                               /* write conf */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ina3221: write conf register failed.\n");                       /* write conf register failed */
        (void)handle->iic_deinit();                                                          /* iic deinit */
        
        return 5;                                                                            /* return error */
    }
    handle->delay_ms(10);                                                                    /* delay 10 ms */
    res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);                   /* read conf */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ina3221: read conf register failed.\n");                        /* read conf register failed */
        (void)handle->iic_deinit();                                                          /* iic deinit */
        
        return 5;                                                                            /* return error */
    }
    if ((prev & (1 << 15)) != 0)                                                             /* check the result */
    {
        handle->debug_print("ina3221: soft reset failed.\n");                                /* soft reset failed */
        (void)handle->iic_deinit();                                                          /* iic deinit */
        
        return 5;                                                                            /* return error */
    }
    
    handle->inited = 1;                                                                      /* flag inited */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t ina3221_deinit(ina3221_handle_t *handle)
{
    uint8_t res;
    uint16_t prev;
   
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_ina3221_iic_read(handle, INA3221_REG_CONF, (uint16_t *)&prev);     /* read config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina3221: read conf register failed.\n");          /* read conf register failed */
       
        return 4;                                                              /* return error */
    }
    prev &= ~(0x07);                                                           /* clear mode */
    res = a_ina3221_iic_write(handle, INA3221_REG_CONF, (uint16_t )prev);      /* write config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina3221: write conf register failed.\n");         /* write conf register failed */
       
        return 4;                                                              /* return error */
    }
    res = handle->iic_deinit();                                                /* iic deinit */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ina3221: iic deinit failed.\n");                  /* iic deinit failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t ina3221_set_reg(ina3221_handle_t *handle, uint8_t reg, uint16_t data)
{
    uint8_t res;
    
    if (handle == NULL)                                           /* check handle */
    {
        return 2;                                                 /* return error */
    }
    if (handle->inited != 1)                                      /* check handle initialization */
    {
        return 3;                                                 /* return error */
    }
    
    res = a_ina3221_iic_write(handle, reg, data);                 /* write data */
    if (res != 0)                                                 /* check result */
    {
        handle->debug_print("ina3221: set reg failed.\n");        /* set reg failed */
       
        return 1;                                                 /* return error */
    }
    
    return 0;                                                     /* success return 0 */
}

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
uint8_t ina3221_get_reg(ina3221_handle_t *handle, uint8_t reg, uint16_t *data)
{
    uint8_t res;
    
    if (handle == NULL)                                           /* check handle */
    {
        return 2;                                                 /* return error */
    }
    if (handle->inited != 1)                                      /* check handle initialization */
    {
        return 3;                                                 /* return error */
    }
    
    res = a_ina3221_iic_read(handle, reg, data);                  /* read data */
    if (res != 0)                                                 /* check result */
    {
        handle->debug_print("ina3221: get reg failed.\n");        /* get reg failed */
       
        return 1;                                                 /* return error */
    }
    
    return 0;                                                     /* success return 0 */
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to an ina3221 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ina3221_info(ina3221_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(ina3221_info_t));                        /* initialize ina3221 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
