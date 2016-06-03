![MikroE](http://www.mikroe.com/img/designs/beta/logo_small.png)

![3G Click](http://cdn.mikroe.com/img/click/3g/3g-click.png)

---
[Product Page](http://www.mikroe.com/click/3g-ea/)

[Manual Page](http://docs.mikroe.com/3g_click)

[Learn Page](http://learn.mikroe.com/2g-sunset-near/)

---

### General Description

3G click (for Europe and Australia) is a mikroBUSâ„¢ add-on board with Quectelâ€™s UG95 3G UMTS/HSPA module. The board has a rich set of components to simplify development: an SMA connector, earphone/microphone jack, MicroUSB port, as well as a SIM card slot. The UG95 module specifies data rates of up to 7.2Mbps (HSPA), and 384Kbps (UMTS). The network protocol stack includes support for TCP, UDP, PPP, MMS, FTP, SMTP, SMTPS, HTTP, HTTPS, PING, and SSL. This version operates on European frequency bands (900/2100@UMTS, 900/1800@GSM). An American version of the same board is available separately. 3G click communicates with the target MCU through the mikroBUSâ„¢ UART interface (RX, TX), with additional functionality provided by STAT, PWRKEY, RTS, R1, and CTS pins. The board is designed to use either a 3.3V or a 5V power supply. Note: Antenna sold separately.

---

### Example

#### Configuration
* MCU:             STM32F107VC
* Dev.Board:       EasyMx Pro v7
* Oscillator:      72 Mhz internal
* Ext. Modules:    Accel 3 click
* SW:              MikroC PRO for ARM 4.7.4

```
#include <stdint.h>

/*      Functions
 ****************************/
#include "3G_click_lib.h"
#include "resources.h"

// TFT module connections
unsigned int TFT_DataPort at GPIOE_ODR;
sbit TFT_RST at GPIOE_ODR.B8;
sbit TFT_RS at GPIOE_ODR.B12;
sbit TFT_CS at GPIOE_ODR.B15;
sbit TFT_RD at GPIOE_ODR.B10;
sbit TFT_WR at GPIOE_ODR.B11;
sbit TFT_BLED at GPIOE_ODR.B9;

sbit GSM_PWR at GPIOC_ODR.B2;
sbit GSM_CTS at GPIOD_ODR.B13;
sbit GSM_RTS at GPIOD_IDR.B10;

bool answer_call = false;
bool hangup_call = false;

void system_init( void );
void system_init( void )
{
    GPIO_Digital_Output( &GPIOC_ODR, _GPIO_PINMASK_2 );
    GPIO_Digital_Output( &GPIOD_ODR, _GPIO_PINMASK_13 );
    GPIO_Digital_Input( &GPIOD_IDR, _GPIO_PINMASK_10 );
    Delay_ms( 100 );

    // UART bus for monitoring
    UART1_Init( 57600 );
    Delay_ms( 200 );

    /// UART bus for GSM
    UART3_Init_Advanced( 9600, _UART_8_BIT_DATA,
                               _UART_NOPARITY,
                               _UART_ONE_STOPBIT,
                               &_GPIO_MODULE_USART3_PD89 );
    Delay_ms( 200 );

    RXNEIE_USART3_CR1_bit = 1;
    NVIC_IntEnable( IVT_INT_USART3 );
    EnableInterrupts();
}

void main()
{
    system_init();
    display_init(); 
    click_3g_api_init();

    while( 1 )
    {
        click_3g_process();

        if( Button( &GPIOC_IDR, 9, 80, 1 ) )
            answer_call = true;

        if( Button( &GPIOC_IDR, 8, 80, 1 ) )
            hangup_call = true;

        if( answer_call )
        {
            click_3g_call_answer();
            answer_call = false;
        }

        if( hangup_call )
        {
            click_3g_call_hangup();
            hangup_call = false;
        }
    }
}
```
