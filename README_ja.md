[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MULTI_BUTTON

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/multi_button/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

押しボタンまたは単にボタンは、機械またはプロセスの一部の側面を制御するための単純なスイッチ機構です。 ボタンは通常、プラスチックまたは金属などの硬い素材で作られています。 通常、表面は平らであるか、人間の指や手に適合する形状になっており、簡単に押したり押したりできるようになります。 ほとんどの場合、ボタンはバイアスされたスイッチですが、バイアスされていないボタンの多くは (その物理的性質により)、押されていない状態に戻るためにスプリングを必要とします。

LibDriver MULTI_BUTTON は、LibDriver によって起動されるボタンの全機能ドライバーです。 マルチボタンまたはマトリックスボタンのプレス、リリース、シングルクリック、ダブルクリック、トリプルクリック、リピートクリック、短押し、長押しなどの機能を提供します。 LibDriver は MISRA に準拠しています。

LibDriver MULTI_BUTTON はマルチボタンとマトリックスボタンをサポートしており、単一ボタンを使用する場合は、より効率的な検出方法を使用できます。

シングルボタンドライバーは参照できます[https://github.com/libdriver/button](https://github.com/libdriver/button)。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver MULTI_BUTTONのソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver MULTI_BUTTON用のプラットフォームに依存しないGPIOバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver MULTI_BUTTONドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver MULTI_BUTTONプログラミング例が含まれています。

/ docディレクトリには、LibDriver MULTI_BUTTONオフラインドキュメントが含まれています。

/ datasheetディレクトリには、MULTI_BUTTONデータシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないGPIOバステンプレートを参照して、指定したプラットフォームのGPIOバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

#### example basic

```C
#include "driver_multi_button_basic.h"

uint8_t res;
uint32_t timeout;

static void a_receive_callback(uint16_t row, uint16_t col, multi_button_t *data)
{
    switch (data->status)
    {
        case MULTI_BUTTON_STATUS_PRESS :
        {
            break;
        }
        case MULTI_BUTTON_STATUS_RELEASE :
        {
            break;
        }
        case MULTI_BUTTON_STATUS_SINGLE_CLICK :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq single click.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_DOUBLE_CLICK :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq double click.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_TRIPLE_CLICK :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq triple click.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_REPEAT_CLICK :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq repeat click with %d times.\n", row, col, data->times);
            
            break;
        }
        case MULTI_BUTTON_STATUS_SHORT_PRESS_START :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq short press start.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_SHORT_PRESS_END :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq short press end.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_LONG_PRESS_START :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq long press start.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_LONG_PRESS_HOLD :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq long press hold.\n", row, col);
            
            break;
        }
        case MULTI_BUTTON_STATUS_LONG_PRESS_END :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq long press end.\n", row, col);
            
            break;
        }
        default :
        {
            multi_button_interface_debug_print("multi_button: row %d col %d irq unknown status.\n", row, col);
            
            break;
        }
    }
}

/* basic init */
res = multi_button_basic_init(4, 4, a_receive_callback);
if (res != 0)
{
    return 1;
}

/* 100s timeout */
timeout = 10000;

/* check timeout */
while (timeout != 0)
{
    /* timeout -- */
    timeout--;

    /* delay 5ms */
    multi_button_interface_delay_ms(5);

    /* process */
    (void)multi_button_basic_process();
    
    
    ....
}

...

/* basic deinit */
(void)multi_button_basic_deinit();

return 0;
```

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/multi_button/index.html](https://www.libdriver.com/docs/multi_button/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。