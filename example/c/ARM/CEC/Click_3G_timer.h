/*

Use mikroE Timer Calculator to generate proper timer configuration
and timer ISR.

https://www.mikroe.com/timer-calculator

*/
#include "Click_3G_types.h"

#define __3G_TIMER__

static void c3g_configTimer()
{


    // Configure Timer
}

void Timer_interrupt()
{
    c3g_tick();
    // Reset Flag
}