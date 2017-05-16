#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <stdint.h>

struct Time_struct
{
  uint16_t hour;
  uint16_t min;
  uint16_t sec;
  uint16_t ms;
} _time;


volatile uint16_t timer_100ms_ticks;
volatile uint16_t timer_ms_ticks;


void update_time(void);
struct Time_struct *get_time(void);


#endif /* STOPWATCH_H */
