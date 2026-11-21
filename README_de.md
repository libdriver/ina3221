[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver INA3221
[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ina3221/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der INA3221 ist ein dreikanaliger High-Side-Monitor für Strom und Busspannung, der über eine I2C- und SMBus-kompatible Schnittstelle verfügt. Der INA3221 überwacht sowohl Spannungsabfälle an Shunt-Widerständen als auch Bus-Versorgungsspannungen; zudem bietet er programmierbare Wandlungszeiten sowie Mittelwertbildungsmodi für diese Signale. Der INA3221 stellt Warn- und Alarmsignale bereit, um für jeden Kanal verschiedene, frei programmierbare Grenzwertüberschreitungen zu erkennen. Er erfasst Ströme auf Busleitungen, deren Spannung zwischen 0 V und 26 V liegen kann. Das Bauteil wird mit einer Versorgungsspannung von 2,7 V bis 5,5 V betrieben und nimmt dabei einen typischen Versorgungsstrom von 350 μA auf. Der INA3221 ist für einen Betriebstemperaturbereich von –40 °C bis +125 °C spezifiziert. Die I2C- und SMBus-kompatible Schnittstelle unterstützt vier programmierbare Adressen.

LibDriver INA3221 ist ein von LibDriver bereitgestellter, voll ausgestatteter Treiber für den INA3221. Er ermöglicht unter anderem kontinuierliche Messungen, Einzelmessungen sowie die Erfassung von Strom- und Spannungswerten. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver INA3221-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver INA3221IIC.

/test enthält den Testcode des LibDriver INA3221-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver INA3221-Beispielcode.

/doc enthält das LibDriver INA3221-Offlinedokument.

/Datenblatt enthält INA3221-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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


### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/ina3221/index.html](https://www.libdriver.com/docs/ina3221/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.