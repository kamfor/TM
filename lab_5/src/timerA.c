#include "../include/timerA.h"

void timerA_init(void)
{
  /*
   * Sourced by CCR0, Up to CCR0 mode, div by 8
   * default TA0CCTL0, TA0CCTL1 settings are fine
   */
   TA0CTL |= TASSEL_2 | ID_0 | MC_1 | TACLR;


   TACCR0 = 1152; // 1600 Hz
   TACCR1 = 1000;  // > 400 000 Hz brightness

   TA0CCTL0 |= CCIE;
   TA0CCTL1 |= CCIE;
}
