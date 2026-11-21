### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. INA3221

#### 3.1 Command Instruction

1. Show ina3221 chip and driver information.

   ```shell
   ina3221 (-i | --information)
   ```

2. Show ina3221 help.

   ```shell
   ina3221 (-h | --help)
   ```

3. Show ina3221 pin connections of the current board.

   ```shell
   ina3221 (-p | --port)
   ```

4. Run ina3221 register test.

   ```shell
   ina3221 (-t reg | --test=reg) [--addr=<0 | 1 | 2 | 3>]
   ```

5. Run ina3221 read test, num is the test times, r is the sample resistance.

   ```shell
   ina3221 (-t read | --test=read) [--addr=<0 | 1 | 2 | 3>] [--resistance=<r>] [--times=<num>]
   ```

6. Run ina3221 read function, num is the test times, r is the sample resistance.

   ```shell
   ina3221 (-e read | --example=read) [--addr=<0 | 1 | 2 | 3>] [--resistance=<r>] [--times=<num>]
   ```

7. Run ina3221 shot function, num is test times, r is the sample resistance.

   ```shell
   ina3221 (-e shot | --example=shot) [--addr=<0 | 1 | 2 | 3>] [--resistance=<r>] [--times=<num>]
   ```
   
#### 3.2 Command Example

```shell
ina3221 -i

ina3221: chip is Texas Instruments INA3221.
ina3221: manufacturer is Texas Instruments.
ina3221: interface is IIC.
ina3221: driver version is 1.0.
ina3221: min supply voltage is 2.7V.
ina3221: max supply voltage is 5.5V.
ina3221: max current is 0.45mA.
ina3221: max temperature is 125.0C.
ina3221: min temperature is -40.0C.
```

```shell
ina3221 -p

ina3221: SCL connected to GPIOB PIN8.
ina3221: SDA connected to GPIOB PIN9.
```

```shell
ina3221 -t reg --addr=0

ina3221: chip is Texas Instruments INA3221.
ina3221: manufacturer is Texas Instruments.
ina3221: interface is IIC.
ina3221: driver version is 1.0.
ina3221: min supply voltage is 2.7V.
ina3221: max supply voltage is 5.5V.
ina3221: max current is 0.45mA.
ina3221: max temperature is 125.0C.
ina3221: min temperature is -40.0C.
ina3221: start register test.
ina3221: ina3221_set_addr_pin/ina3221_get_addr_pin test.
ina3221: set addr pin 0.
ina3221: check addr pin ok.
ina3221: set addr pin 1.
ina3221: check addr pin ok.
ina3221: set addr pin 2.
ina3221: check addr pin ok.
ina3221: set addr pin 3.
ina3221: check addr pin ok.
ina3221: ina3221_set_resistance/ina3221_get_resistance test.
ina3221: set resistance 0.068000.
ina3221: check resistance 0.068000.
ina3221: ina3221_set_channel/ina3221_get_channel test.
ina3221: channel 1 disable.
ina3221: check channel ok.
ina3221: channel 1 enable.
ina3221: check channel ok.
ina3221: channel 2 disable.
ina3221: check channel ok.
ina3221: channel 2 enable.
ina3221: check channel ok.
ina3221: channel 3 disable.
ina3221: check channel ok.
ina3221: channel 3 enable.
ina3221: check channel ok.
ina3221: ina3221_set_adc_average/ina3221_get_adc_average test.
ina3221: set adc average 1 sample.
ina3221: check adc average ok.
ina3221: set adc average 4 samples.
ina3221: check adc average ok.
ina3221: set adc average 16 samples.
ina3221: check adc average ok.
ina3221: set adc average 64 samples.
ina3221: check adc average ok.
ina3221: set adc average 128 samples.
ina3221: check adc average ok.
ina3221: set adc average 256 samples.
ina3221: check adc average ok.
ina3221: set adc average 512 samples.
ina3221: check adc average ok.
ina3221: set adc average 1024 samples.
ina3221: check adc average ok.
ina3221: ina3221_set_bus_voltage_conversion_time/ina3221_get_bus_voltage_conversion_time test.
ina3221: set bus voltage conversion time 140 us.
ina3221: check bus voltage conversion time ok.
ina3221: set bus voltage conversion time 204 us.
ina3221: check bus voltage conversion time ok.
ina3221: set bus voltage conversion time 332 us.
ina3221: check bus voltage conversion time ok.
ina3221: set bus voltage conversion time 588 us.
ina3221: check bus voltage conversion time ok.
ina3221: set bus voltage conversion time 1.1 ms.
ina3221: check bus voltage conversion time ok.
ina3221: set bus voltage conversion time 2.116 ms.
ina3221: check bus voltage conversion time ok.
ina3221: set bus voltage conversion time 4.156 ms.
ina3221: check bus voltage conversion time ok.
ina3221: set bus voltage conversion time 8.244 ms.
ina3221: check bus voltage conversion time ok.
ina3221: ina3221_set_shunt_voltage_conversion_time/ina3221_get_shunt_voltage_conversion_time test.
ina3221: set shunt voltage conversion time 140 us.
ina3221: check shunt voltage conversion time ok.
ina3221: set shunt voltage conversion time 204 us.
ina3221: check shunt voltage conversion time ok.
ina3221: set shunt voltage conversion time 332 us.
ina3221: check shunt voltage conversion time ok.
ina3221: set shunt voltage conversion time 588 us.
ina3221: check shunt voltage conversion time ok.
ina3221: set shunt voltage conversion time 1.1 ms.
ina3221: check shunt voltage conversion time ok.
ina3221: set shunt voltage conversion time 2.116 ms.
ina3221: check shunt voltage conversion time ok.
ina3221: set shunt voltage conversion time 4.156 ms.
ina3221: check shunt voltage conversion time ok.
ina3221: set shunt voltage conversion time 8.244 ms.
ina3221: check shunt voltage conversion time ok.
ina3221: ina3221_set_mode/ina3221_get_mode test.
ina3221: set mode power down.
ina3221: check mode ok.
ina3221: set mode shunt voltage triggered.
ina3221: check mode ok.
ina3221: set mode bus voltage triggered.
ina3221: check mode ok.
ina3221: set mode shunt and bus voltage triggered.
ina3221: check mode ok.
ina3221: set mode shunt voltage continuous.
ina3221: check mode ok.
ina3221: set mode bus voltage continuous.
ina3221: check mode ok.
ina3221: set mode shunt and bus voltage continuous.
ina3221: check mode ok.
ina3221: ina3221_set_critical_alert_limit/ina3221_get_critical_alert_limit test.
ina3221: set channel 1 critical alert limit 0x0E58.
ina3221: check channel 1 critical alert limit 0x0E58 ok.
ina3221: set channel 2 critical alert limit 0xDA48.
ina3221: check channel 2 critical alert limit 0xDA48 ok.
ina3221: set channel 3 critical alert limit 0xC340.
ina3221: check channel 3 critical alert limit 0xC340 ok.
ina3221: ina3221_set_warning_alert_limit/ina3221_get_warning_alert_limit test.
ina3221: set channel 1 warning alert limit 0x6838.
ina3221: check channel 1 warning alert limit 0x6838 ok.
ina3221: set channel 2 warning alert limit 0x9A28.
ina3221: check channel 2 warning alert limit 0x9A28 ok.
ina3221: set channel 3 warning alert limit 0xDE40.
ina3221: check channel 3 warning alert limit 0xDE40 ok.
ina3221: ina3221_set_shunt_voltage_sum_limit/ina3221_get_shunt_voltage_sum_limit test.
ina3221: set shunt voltage sum limit 0x9EFE.
ina3221: check shunt voltage sum limit 0x9EFE ok.
ina3221: ina3221_set_power_valid_upper_limit/ina3221_get_power_valid_upper_limit test.
ina3221: set power valid upper limit 0x1898.
ina3221: check power valid upper limit 0x1898 ok.
ina3221: ina3221_set_power_valid_lower_limit/ina3221_get_power_valid_lower_limit test.
ina3221: set power valid lower limit 0x7E20.
ina3221: check power valid lower limit 0x7E20 ok.
ina3221: ina3221_set_channel_summation/ina3221_get_channel_summation test.
ina3221: channel 1 summation disable.
ina3221: check channel 1 summation ok.
ina3221: channel 1 summation enable.
ina3221: check channel 1 summation ok.
ina3221: channel 2 summation disable.
ina3221: check channel 2 summation ok.
ina3221: channel 2 summation enable.
ina3221: check channel 2 summation ok.
ina3221: channel 3 summation disable.
ina3221: check channel 3 summation ok.
ina3221: channel 3 summation enable.
ina3221: check channel 3 summation ok.
ina3221: ina3221_set_warning_alert_latch/ina3221_get_warning_alert_latch test.
ina3221: disable warning alert latch.
ina3221: check warning alert latch ok.
ina3221: enable warning alert latch.
ina3221: check warning alert latch ok.
ina3221: ina3221_set_critical_alert_latch/ina3221_get_critical_alert_latch test.
ina3221: disable critical alert latch.
ina3221: check critical alert latch ok.
ina3221: enable critical alert latch.
ina3221: check critical alert latch ok.
ina3221: ina3221_shunt_voltage_limit_convert_to_register/ina3221_shunt_voltage_limit_convert_to_data test.
ina3221: check shunt voltage limit 71.48 -> 71.44 ok.
ina3221: ina3221_shunt_voltage_sum_limit_convert_to_register/ina3221_shunt_voltage_sum_limit_convert_to_data test.
ina3221: check shunt voltage sum limit 287.47 -> 287.44 ok.
ina3221: ina3221_power_limit_convert_to_register/ina3221_power_limit_convert_to_data test.
ina3221: check power limit 26207.40 -> 26200.00 ok.
ina3221: ina3221_soft_reset test.
ina3221: soft reset check ok.
ina3221: finish register test.
```

```shell
ina3221 -t read --addr=0 --resistance=0.1 --times=3

ina3221: chip is Texas Instruments INA3221.
ina3221: manufacturer is Texas Instruments.
ina3221: interface is IIC.
ina3221: driver version is 1.0.
ina3221: min supply voltage is 2.7V.
ina3221: max supply voltage is 5.5V.
ina3221: max current is 0.45mA.
ina3221: max temperature is 125.0C.
ina3221: min temperature is -40.0C.
ina3221: start read test.
ina3221: single read test.
ina3221: shunt voltage channel1 is 0.000000 mV.
ina3221: shunt voltage channel2 is 0.000000 mV.
ina3221: shunt voltage channel3 is 0.000000 mV.
ina3221: bus voltage channel1 is 0.000000 mV.
ina3221: bus voltage channel2 is 0.000000 mV.
ina3221: bus voltage channel3 is 0.000000 mV.
ina3221: current channel1 is 0.000000 mA.
ina3221: current channel2 is 0.000000 mA.
ina3221: current channel3 is 0.000000 mA.
ina3221: shunt voltage sum is 0.000000 mV.
ina3221: shunt voltage channel1 is 0.000000 mV.
ina3221: shunt voltage channel2 is 0.000000 mV.
ina3221: shunt voltage channel3 is 0.000000 mV.
ina3221: bus voltage channel1 is 0.000000 mV.
ina3221: bus voltage channel2 is 0.000000 mV.
ina3221: bus voltage channel3 is 0.000000 mV.
ina3221: current channel1 is 0.000000 mA.
ina3221: current channel2 is 0.000000 mA.
ina3221: current channel3 is 0.000000 mA.
ina3221: shunt voltage sum is 0.000000 mV.
ina3221: shunt voltage channel1 is 0.000000 mV.
ina3221: shunt voltage channel2 is 0.000000 mV.
ina3221: shunt voltage channel3 is 0.000000 mV.
ina3221: bus voltage channel1 is 0.000000 mV.
ina3221: bus voltage channel2 is 0.000000 mV.
ina3221: bus voltage channel3 is 0.000000 mV.
ina3221: current channel1 is 0.000000 mA.
ina3221: current channel2 is 0.000000 mA.
ina3221: current channel3 is 0.000000 mA.
ina3221: shunt voltage sum is 0.000000 mV.
ina3221: continuous read test.
ina3221: shunt voltage channel1 is 0.000000 mV.
ina3221: shunt voltage channel2 is 0.000000 mV.
ina3221: shunt voltage channel3 is 0.000000 mV.
ina3221: bus voltage channel1 is 0.000000 mV.
ina3221: bus voltage channel2 is 0.000000 mV.
ina3221: bus voltage channel3 is 0.000000 mV.
ina3221: current channel1 is 0.000000 mA.
ina3221: current channel2 is 0.000000 mA.
ina3221: current channel3 is 0.000000 mA.
ina3221: shunt voltage sum is 0.000000 mV.
ina3221: shunt voltage channel1 is 0.000000 mV.
ina3221: shunt voltage channel2 is 0.000000 mV.
ina3221: shunt voltage channel3 is 0.000000 mV.
ina3221: bus voltage channel1 is 0.000000 mV.
ina3221: bus voltage channel2 is 0.000000 mV.
ina3221: bus voltage channel3 is 0.000000 mV.
ina3221: current channel1 is 0.000000 mA.
ina3221: current channel2 is 0.000000 mA.
ina3221: current channel3 is 0.000000 mA.
ina3221: shunt voltage sum is 0.000000 mV.
ina3221: shunt voltage channel1 is 0.000000 mV.
ina3221: shunt voltage channel2 is 0.000000 mV.
ina3221: shunt voltage channel3 is 0.000000 mV.
ina3221: bus voltage channel1 is 0.000000 mV.
ina3221: bus voltage channel2 is 0.000000 mV.
ina3221: bus voltage channel3 is 0.000000 mV.
ina3221: current channel1 is 0.000000 mA.
ina3221: current channel2 is 0.000000 mA.
ina3221: current channel3 is 0.000000 mA.
ina3221: shunt voltage sum is 0.000000 mV.
ina3221: finish read test.
```

```shell
ina3221 -e read --addr=0 --resistance=0.1 --times=3

ina3221: 1/3.
ina3221: channel1.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: channel2.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: channel3.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: 2/3.
ina3221: channel1.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: channel2.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: channel3.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: 3/3.
ina3221: channel1.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: channel2.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: channel3.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
```

```shell
ina3221 -e shot --addr=0 --resistance=0.1 --times=3

ina3221: 1/3.
ina3221: channel1.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: channel2.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: channel3.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: 2/3.
ina3221: channel1.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: channel2.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: channel3.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: 3/3.
ina3221: channel1.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: channel2.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
ina3221: channel3.
ina3221: bus voltage is 0.000mV.
ina3221: current is 0.000mA.
```

```shell
ina3221 -h

Usage:
  ina3221(-i | --information)
  ina3221(-h | --help)
  ina3221(-p | --port
  ina3221(-t reg | --test=reg) [--addr=<0 | 1 | 2 | 3>]
  ina3221(-t read | --test=read) [--addr=<0 | 1 | 2 | 3>] [--resistance=<r>] [--times=<num>]
  ina3221(-e read | --example=read) [--addr=<0 | 1 | 2 | 3>] [--resistance=<r>] [--times=<num>]
  ina3221(-e shot | --example=shot) [--addr=<0 | 1 | 2 | 3>] [--resistance=<r>] [--times=<num>]

Options:
      --addr=<0 | 1 | 2 | 3>     Set the addr pin.([default: 0])
  -e <read | shot>, --example=<read | shot>
                                 Run the driver example.
  -h, --help                     Show the help.
  -i, --information              Show the chip information.
  -p, --port                     Display the pin connections of the current board.
      --resistance=<r>           Set the sample resistance.([default: 0.1])
  -t <reg | read>, --test=<reg | read>
                                 Run the driver test.
      --times=<num>              Set the running times.([default: 3])
```

