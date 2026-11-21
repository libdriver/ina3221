### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(ina3221 REQUIRED)
```
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
./ina3221 -i

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
./ina3221 -p

ina3221: SCL connected to GPIO3(BCM).
ina3221: SDA connected to GPIO2(BCM).
```

```shell
./ina3221 -t reg --addr=0

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
ina3221: set resistance 0.083000.
ina3221: check resistance 0.083000.
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
ina3221: set channel 1 critical alert limit 0x1E30.
ina3221: check channel 1 critical alert limit 0x1E30 ok.
ina3221: set channel 2 critical alert limit 0xC348.
ina3221: check channel 2 critical alert limit 0xC348 ok.
ina3221: set channel 3 critical alert limit 0x4398.
ina3221: check channel 3 critical alert limit 0x4398 ok.
ina3221: ina3221_set_warning_alert_limit/ina3221_get_warning_alert_limit test.
ina3221: set channel 1 warning alert limit 0xE288.
ina3221: check channel 1 warning alert limit 0xE288 ok.
ina3221: set channel 2 warning alert limit 0xE7F8.
ina3221: check channel 2 warning alert limit 0xE7F8 ok.
ina3221: set channel 3 warning alert limit 0xA250.
ina3221: check channel 3 warning alert limit 0xA250 ok.
ina3221: ina3221_set_shunt_voltage_sum_limit/ina3221_get_shunt_voltage_sum_limit test.
ina3221: set shunt voltage sum limit 0xB1D8.
ina3221: check shunt voltage sum limit 0xB1D8 ok.
ina3221: ina3221_set_power_valid_upper_limit/ina3221_get_power_valid_upper_limit test.
ina3221: set power valid upper limit 0x7948.
ina3221: check power valid upper limit 0x7948 ok.
ina3221: ina3221_set_power_valid_lower_limit/ina3221_get_power_valid_lower_limit test.
ina3221: set power valid lower limit 0x6668.
ina3221: check power valid lower limit 0x6668 ok.
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
ina3221: check shunt voltage limit 23.62 -> 23.60 ok.
ina3221: ina3221_shunt_voltage_sum_limit_convert_to_register/ina3221_shunt_voltage_sum_limit_convert_to_data test.
ina3221: check shunt voltage sum limit -499.73 -> -499.72 ok.
ina3221: ina3221_power_limit_convert_to_register/ina3221_power_limit_convert_to_data test.
ina3221: check power limit 869.00 -> 864.00 ok.
ina3221: ina3221_soft_reset test.
ina3221: soft reset check ok.
ina3221: finish register test.
```

```shell
./ina3221 -t read --addr=0 --resistance=0.1 --times=3

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
./ina3221 -e read --addr=0 --resistance=0.1 --times=3

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
./ina3221 -e shot --addr=0 --resistance=0.1 --times=3

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
./ina3221 -h

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
