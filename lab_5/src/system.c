#include "../include/system.h"



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

void _io_init(void)
{
    P1DIR = 0x00;
    P1IES = START_BUTTON | INTERTIME_BUTTON | RESET_BUTTON;
	P1IE =	START_BUTTON | INTERTIME_BUTTON | RESET_BUTTON;
	P2DIR |= 0xFF; //port 2 - wyjscie
    P3DIR |= 0xFF; //port 3 - wyjscie
}


void system_init(void)
{
  _clocks_init();
  timerA_init();
  _io_init();
}


