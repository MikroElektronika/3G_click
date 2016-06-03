#include "3g_click_lib.h"

void gsm2_api_init()
{
    gsm_engine_init( gsm2_callback );

    at_cmd_save( "RING", 1000, NULL, NULL, NULL, gsm2_cb_ring );
    at_cmd_save( "NO CARRIER", 1000, NULL, NULL, NULL, gsm2_cb_no_carrier );
    at_cmd_save( "+CLCC", 1000, NULL, NULL, NULL, gsm2_cb_call_id );
}

void gsm2_callback( char *response )
{
    UART1_Write_Text( response );
}

void gsm2_cb_ring( char *response )
{
    gsm2_call_id();
}

void gsm2_cb_no_carrier( char *response )
{
    TFT_Rectangle( 50, 90, 300, 180 );
}

void gsm2_cb_call_id( char *response )
{
    char *tmp_s;
    char *tmp_e;
    char tmp_num[ 20 ] = { 0 };

    tmp_s = strchr( response, '\"' );
    tmp_e = strchr( tmp_s + 1, '\"' );
    strncpy( tmp_num, tmp_s + 1, tmp_e - tmp_s - 1  );

    TFT_Rectangle( 50, 90, 300, 180 );
    TFT_Write_Text( tmp_num, 86, 110 );
}

void gsm2_call_answer()
{
    at_cmd( "ATA" );
}

void gsm2_call_hangup()
{
    at_cmd( "ATH" );
    TFT_Rectangle( 50, 90, 300, 180 );
}

void gsm2_call_id()
{
    at_cmd( "AT+CLCC" );
}