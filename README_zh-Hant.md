[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver INA3221

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ina3221/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

INA3221是一款三通道高壓側電流和匯流排電壓監測器，具有I2C和SMBUS相容介面。 INA3221除了具有可程式設計的轉換時間和這些訊號的平均模式外，還監測分流電壓降和匯流排電源電壓。 INA3221提供關鍵和警告警報，以檢測每個通道的多個可程式設計超範圍情况。 INA3221感測匯流排上的電流，電流範圍為0V至26V。該設備由單個2.7V至5.5V電源供電，並消耗350μa（典型值）的電源電流。 INA3221的工作溫度範圍為-40°C至+125°C。I2C和SMBUS相容介面具有四個可程式設計地址。

LibDriver INA3221是LibDriver推出的INA3221的全功能驅動，該驅動提供連續模式讀取、單次模式讀取，電流採集和電壓採集等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver INA3221的源文件。

/interface目錄包含了LibDriver INA3221與平台無關的IIC總線模板。

/test目錄包含了LibDriver INA3221驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver INA3221編程範例。

/doc目錄包含了LibDriver INA3221離線文檔。

/datasheet目錄包含了INA3221數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/ina3221/index.html](https://www.libdriver.com/docs/ina3221/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。

