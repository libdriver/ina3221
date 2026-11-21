[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver INA3221

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ina3221/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The INA3221 is a three-channel, high-side current and bus voltage monitor with an I2C- and SMBUS-compatible interface. The INA3221 monitors both shunt voltage drops and bus supply voltages, in addition to having programmable conversion times and averaging modes for these signals. The INA3221 offers both critical and warning alerts to detect multiple programmable out-of-range conditions for each channel. The INA3221 senses current on buses that can vary from 0V to 26V. The device is powered from a single 2.7V to 5.5V supply, and draws 350μA (typ) of supply current. The INA3221 is specified over the operating temperature range of –40°C to +125°C. The I2C- and SMBUS-compatible interface features four programmable addresses.

LibDriver INA3221 is a full-featured driver for INA3221, launched by LibDriver.It provides continuous reading, single reading, current acquisition, voltage acquisition and additional features. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver INA3221 source files.

/interface includes LibDriver INA3221 IIC platform independent template.

/test includes LibDriver INA3221 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver INA3221 sample code.

/doc includes LibDriver INA3221 offline document.

/datasheet includes INA3221 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC platform independent template and finish your platform IIC driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_ina3221_basic.h"

uint8_t res;
uint32_t i;

/* basic init */
res = ina3221_basic_init(INA3221_ADDRESS_0, 0.1f);
if (res != 0)
{
    return 1;
}

/* delay 1000ms */
ina3221_interface_delay_ms(1000);

/* loop */
for (i = 0; i < 3; i++)
{
    float mV;
    float mA;

    /* read data */
    res = ina3221_basic_read(INA3221_CHANNEL_1, &mV, &mA);
    if (res != 0)
    {
        (void)ina3221_basic_deinit();

        return 1;
    }

    /* output */
    ina3221_interface_debug_print("ina3221: %d/%d.\n", i + 1, 3);
    ina3221_interface_debug_print("ina3221: channel1.\n");
    ina3221_interface_debug_print("ina3221: bus voltage is %0.3fmV.\n", mV);
    ina3221_interface_debug_print("ina3221: current is %0.3fmA.\n", mA);

    /* read data */
    res = ina3221_basic_read(INA3221_CHANNEL_2, &mV, &mA);
    if (res != 0)
    {
        (void)ina3221_basic_deinit();

        return 1;
    }

    /* output */
    ina3221_interface_debug_print("ina3221: channel2.\n");
    ina3221_interface_debug_print("ina3221: bus voltage is %0.3fmV.\n", mV);
    ina3221_interface_debug_print("ina3221: current is %0.3fmA.\n", mA);

    /* read data */
    res = ina3221_basic_read(INA3221_CHANNEL_3, &mV, &mA);
    if (res != 0)
    {
        (void)ina3221_basic_deinit();

        return 1;
    }

    /* output */
    ina3221_interface_debug_print("ina3221: channel3.\n");
    ina3221_interface_debug_print("ina3221: bus voltage is %0.3fmV.\n", mV);
    ina3221_interface_debug_print("ina3221: current is %0.3fmA.\n", mA);

    /* delay 1000ms */
    ina3221_interface_delay_ms(1000);
}

/* deinit */
(void)ina3221_basic_deinit();

return 0;
```

#### example shot

```C
#include "driver_ina3221_shot.h"

uint8_t res;
uint32_t i;

/* shot init */
res = ina3221_shot_init(INA3221_ADDRESS_0, 0.1f);
if (res != 0)
{
    return 1;
}

/* delay 1000ms */
ina3221_interface_delay_ms(1000);

/* loop */
for (i = 0; i < 3; i++)
{
    float mV;
    float mA;

    /* read data */
    res = ina3221_shot_read(INA3221_CHANNEL_1, &mV, &mA);
    if (res != 0)
    {
        (void)ina3221_basic_deinit();

        return 1;
    }

    /* output */
    ina3221_interface_debug_print("ina3221: %d/%d.\n", i + 1, 3);
    ina3221_interface_debug_print("ina3221: channel1.\n");
    ina3221_interface_debug_print("ina3221: bus voltage is %0.3fmV.\n", mV);
    ina3221_interface_debug_print("ina3221: current is %0.3fmA.\n", mA);

    /* read data */
    res = ina3221_shot_read(INA3221_CHANNEL_2, &mV, &mA);
    if (res != 0)
    {
        (void)ina3221_basic_deinit();

        return 1;
    }

    /* output */
    ina3221_interface_debug_print("ina3221: channel2.\n");
    ina3221_interface_debug_print("ina3221: bus voltage is %0.3fmV.\n", mV);
    ina3221_interface_debug_print("ina3221: current is %0.3fmA.\n", mA);

    /* read data */
    res = ina3221_shot_read(INA3221_CHANNEL_3, &mV, &mA);
    if (res != 0)
    {
        (void)ina3221_basic_deinit();

        return 1;
    }

    /* output */
    ina3221_interface_debug_print("ina3221: channel3.\n");
    ina3221_interface_debug_print("ina3221: bus voltage is %0.3fmV.\n", mV);
    ina3221_interface_debug_print("ina3221: current is %0.3fmA.\n", mA);

    /* delay 1000ms */
    ina3221_interface_delay_ms(1000);
}

/* shot deinit */
(void)ina3221_shot_deinit();

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/ina3221/index.html](https://www.libdriver.com/docs/ina3221/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.