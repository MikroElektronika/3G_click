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

void system_init()
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

void display_init()
{
    TFT_Init_ILI9341_8bit( 320, 240 );
    TFT_BLED = 1;
    TFT_Set_Pen( CL_WHITE, 1 );
    TFT_Set_Brush( 1,CL_WHITE,0,0,0,0 );
    TFT_Set_Font( TFT_defaultFont, CL_BLACK, FO_HORIZONTAL );
    TFT_Fill_Screen( CL_WHITE );
    TFT_Set_Pen( CL_BLACK, 1 );
    TFT_Line(  20,  46, 300,  46 );
    TFT_Line(  20,  70, 300,  70 );
    TFT_Line(  20, 196, 300, 196 );;
    TFT_Line(  20, 220, 300, 220 );
    TFT_Set_Pen( CL_WHITE, 1 );
    TFT_Set_Font( &HandelGothic_BT21x22_Regular, CL_RED, FO_HORIZONTAL );
    TFT_Write_Text( "3G Click", 130, 14 );
    TFT_Set_Font( &Tahoma15x16_Bold, CL_BLUE, FO_HORIZONTAL );
    TFT_Write_Text( "Quectel UG95", 125, 50 );
    TFT_Set_Font( &Verdana12x13_Regular, CL_BLACK, FO_HORIZONTAL );
    TFT_Write_Text("EasyMx PRO v7 for STM32", 19, 223);
    TFT_Set_Font( &Verdana12x13_Regular, CL_RED, FO_HORIZONTAL );
    TFT_Write_Text( "www.mikroe.com", 200, 223 );
    TFT_Set_Font( &Tahoma15x16_Bold, CL_BLUE, FO_HORIZONTAL );
    TFT_Write_Text( "HANG UP - C8", 44, 200 );
    TFT_Write_Text( "ANSWER - C9", 186, 200 );
    TFT_Set_Font( &HandelGothic_BT21x22_Regular, CL_BLACK, FO_HORIZONTAL );
}

void main()
{
    system_init();
    display_init();
    gsm2_api_init();

    while( 1 )
    {
        gsm_process();

        if( Button( &GPIOC_IDR, 9, 80, 1 ) )
            answer_call = true;

        if( Button( &GPIOC_IDR, 8, 80, 1 ) )
            hangup_call = true;

        if( answer_call )
        {
            gsm2_call_answer();
            answer_call = false;
        }

        if( hangup_call )
        {
            gsm2_call_hangup();
            hangup_call = false;
        }
    }
}

void LO_RX_ISR() iv IVT_INT_USART3 ics ICS_AUTO
{
    if( RXNE_USART3_SR_bit )
    {
        char tmp = USART3_DR;
        at_adapter_rx( tmp );
    }
}