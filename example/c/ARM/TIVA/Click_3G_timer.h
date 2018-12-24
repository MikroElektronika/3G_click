/*

Use mikroE Timer Calculator to generate proper timer configuration
and timer ISR.

https://www.mikroe.com/timer-calculator

*/
#include "Click_3G_types.h"

#define __3G_TIMER__

static void c3g_configTimer()
{
    SYSCTL_RCGCTIMER_R0_bit = 1;
    TIMER_CTL_TAEN_bit = 0;
    TIMER0_CFG   = 4;
    TIMER0_TAMR |= 2;
    TIMER0_TAPR  = 1;
    TIMER0_TAILR = 60000;
    NVIC_IntEnable(IVT_INT_TIMER0A_16_32_bit);
    TIMER_IMR_TATOIM_bit = 1;
    TIMER_CTL_TAEN_bit   = 1;
    EnableInterrupts();
}

void Timer_interrupt() iv IVT_INT_TIMER0A_16_32_bit 
{
    c3g_tick();
    TIMER_ICR_TATOCINT_bit = 1;
}
