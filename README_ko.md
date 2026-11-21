[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver INA3221

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ina3221/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

INA3221은 I2C 및 SMBus 호환 인터페이스를 갖춘 3채널 하이사이드(high-side) 전류 및 버스 전압 모니터입니다. INA3221은 션트 전압 강하와 버스 공급 전압을 모두 모니터링하며, 이러한 신호에 대해 프로그래밍 가능한 변환 시간 및 평균화 모드를 제공합니다. 또한 각 채널별로 설정 가능한 다양한 범위 이탈(out-of-range) 조건을 감지하여 위험(critical) 및 경고(warning) 알림을 제공합니다. INA3221은 0V에서 26V 범위의 버스에서 전류를 감지할 수 있습니다. 이 장치는 2.7V~5.5V의 단일 전원으로 구동되며, 350μA(일반값)의 공급 전류를 소비합니다. INA3221은 -40°C에서 +125°C의 동작 온도 범위에서 사양을 보장합니다. I2C 및 SMBus 호환 인터페이스는 4개의 프로그래밍 가능한 주소를 지원합니다.

LibDriver INA3221은 LibDriver에서 출시한 INA3221용 종합 드라이버입니다. 이 드라이버는 연속 읽기, 단일 읽기, 전류 및 전압 측정, 그리고 기타 다양한 기능을 제공합니다. LibDriver는 MISRA 표준을 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver INA3221의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver INA3221용 플랫폼 독립적인 IIC버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver INA3221드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver INA3221프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver INA3221오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 INA3221데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인 IIC버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/ina3221/index.html](https://www.libdriver.com/docs/ina3221/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.