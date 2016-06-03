#include "3G_click_lib.h"
#include "resources.h"
#include <built_in.h>

char TFT_DataPort at GPIO_PORT_32_39;
sbit TFT_RST at GPIO_PIN42_bit;
sbit TFT_RS at GPIO_PIN40_bit;
sbit TFT_CS at GPIO_PIN41_bit;
sbit TFT_RD at GPIO_PIN46_bit;
sbit TFT_WR at GPIO_PIN47_bit;
sbit TFT_BLED at GPIO_PIN58_bit;

sbit GSM_PWR at GPIO_PIN1_bit;
sbit GSM_CTS at GPIO_PIN28_bit;
sbit GSM_RTS at GPIO_PIN3_bit;

void Auxiliary_Code(void);
void Set_Index(unsigned short index);
void Write_Command(unsigned short cmd);
void Write_Data(unsigned int _data);

bool answer_call = false;
bool hangup_call = false;
bool call_id = false;


void system_init( void );

void system_init()
{
    GPIO_Digital_Output(&GPIO_PORT_00_07,_GPIO_PINMASK_1);
    GPIO_Digital_Output(&GPIO_PORT_24_31,_GPIO_PINMASK_4);
    GPIO_Digital_Input(&GPIO_PORT_00_07,_GPIO_PINMASK_3  );
    GPIO_Digital_Input(&GPIO_PORT_08_15, _GPIO_PINMASK_0);
    GPIO_Digital_Input(&GPIO_PORT_08_15, _GPIO_PINMASK_1);
    
    Delay_ms( 100 );
    // UART bus for monitoring
    UART1_Init( 57600 );
    Delay_ms( 200 );
    UART1_Write_Text("Uart initialized");
    /// UART bus for GSM
    UART2_Init( 9600 );
    Delay_ms( 200 );

    // set up the UART interrupt
    UART2_FCR |= ( 1 << FIFO_EN ) | ( 1 << RCVR_RST );
    UART2_INT_ENABLE |= ( 1 << RX_AVL_EN );
    IRQ_CTRL &= ~( 1 << GLOBAL_INTERRUPT_MASK );

}



void display_init()
{
    TFT_Init_ILI9341_8bit( 320, 240 );
    TFT_BLED = 1;
    TFT_Set_Pen( CL_AQUA, 1 );
    TFT_Set_Brush( 1,CL_AQUA,0,0,0,0 );
    TFT_Set_Font( TFT_defaultFont, CL_BLACK, FO_HORIZONTAL );
    TFT_Fill_Screen( CL_AQUA );
    TFT_Set_Pen( CL_BLACK, 1 );
    TFT_Line(  20,  46, 300,  46 );
    TFT_Line(  20,  70, 300,  70 );
    TFT_Line(  20, 196, 300, 196 );;
    TFT_Line(  20, 220, 300, 220 );
    TFT_Set_Pen( CL_AQUA, 1 );
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
        
        if( Button(&GPIO_PORT_08_15, 0, 200, 1) )
            answer_call = true;
            
        if( Button(&GPIO_PORT_08_15, 2, 200, 1) )
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
        
        if( call_id )
        {
            gsm2_call_id();
            call_id = false;
        
        }
    }
}




void UART_RX_ISR() iv IVT_UART2_IRQ ics ICS_AUTO
{

        char tmp = UART2_Read();
        at_adapter_rx( tmp );
        UART1_Write( tmp );
}