
#include <msp430.h>
#include "system.h"
#include "watchdog.h"

extern volatile uint16_t timer_100ms_ticks;
extern volatile uint16_t timer_ms_ticks;

int main(void)
{
  watchdog_disable();
  system_init();
  
  
   __enable_interrupt();
   //  __low_power_mod_1();
   //  __bis_SR_register(GIE);
}



// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A0 (void)
{
  
}

// Timer_A3 Interrupt Vector (TAIV) handler
#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A1(void)
{
  switch( TAIV )
  {
  case  2: CCR1 += 1000;
           break;
 }
}

