#include "system.h"

void _clocks_init(void)
{
  /*
   * SMCLK - XT2 source, div by 4
   * MCLK  - DCO source, default 1MHz, no div
   *
   */

  BCSCTL1  &= ~XT2OFF;
  BCSCTL2 |= SELS | DIVS_2;

  /* Let oscillator  stabilize */
  _delay_cycles(10000);
  do {
    IFG1 &= ~OFIFG;
    _delay_cycles(10000);
  } while (IFG1 & OFIFG);

}


void system_init(void)
{
  _clocks_init(void);
  timerA_init(void);
}
