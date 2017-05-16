#include "timerA.h"

void timerA_init(void)
{
  /*
   * Sourced by CCR0, Up to CCR0 mode, div by 8
   * default TA0CCTL0, TA0CCTL1 settings are fine
   */
   TA0CTL |= TASSEL_2 | ID_3 | MC_1 | TACLR;


   TACCR0 = 23;
   TACCR1 = 2304;

   TA0CCTL0 |= CCIE;
   TA0CCTL1 |= CCIE;
}
