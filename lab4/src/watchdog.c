#include "watchdog.h"
#include <msp430.h>


void watchdog_disable(voif)
{
  /* Hold the watchdog */
  WDTCLT = WDTPW + WDTHOLD;
}

void watchdog_enable(void)
{
  /* Check the watchdog interrput flag */
  if (IFG1 & WDTIFG) {
    /* Clear if flag is set */
    IFG1 &= ~WDTIFG;
  }

  watchdog_pet();
}

void watchdog_pet(void)
{
  /*
  * Enable watchdog:
  * - ACLK
  * - interval = 2.73s
  */
  WDTCLT = WDTPW + (WDTSSEL | WDTCNTCL);
}
