#include "gsm_engine.h"

void gsm2_api_init( void );

void gsm2_callback( char *response );

void gsm2_cb_ring( char *response );

void gsm2_cb_no_carrier( char *response );

void gsm2_cb_call_id( char *response );

void gsm2_call_answer( void );

void gsm2_call_hangup( void );

void gsm2_call_id( void );