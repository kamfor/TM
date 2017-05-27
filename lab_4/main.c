
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "include/system.h"
#include "include/watchdog.h"

#define TICKS_PER_CSEK 16
#define INTERTIMES_SIZE 10
#define DEBOUNCE_VAL 32

extern volatile uint16_t timer_10ms_ticks;
volatile uint8_t display_data[8] = {0xF0,0x70,0x70,0xF0,0x70,0xF0,0xF0,0xF0};
volatile uint8_t position;
volatile uint8_t csecs_counter;

volatile bool to_increment = false;
volatile uint8_t debounce_buttons = 0x00;
volatile uint8_t state_of_buttons = 0x00;
volatile uint8_t start_button_counter = DEBOUNCE_VAL;
volatile uint8_t intertime_button_counter = DEBOUNCE_VAL;
volatile uint8_t reset_button_counter = DEBOUNCE_VAL;



bool button_action = false;
bool running = false;
bool start_button_action = false;
bool intertime_overflow = false;



typedef volatile struct  {
	uint8_t cseconds;
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
} stopwatch_time;

stopwatch_time inter_time[INTERTIMES_SIZE];

static uint8_t intertime_index = 1;
static uint8_t intertime_display_index = 0;


void save_intertime(uint8_t);
void display_intertime(uint8_t);


int main(void)
{
  watchdog_disable();
  system_init();

   __enable_interrupt();
   while(1){
	   /* Konwersja licznikow na czas*/
	   if(inter_time[0].cseconds >= 100)
	   {
		   inter_time[0].cseconds -= 100;
		   inter_time[0].seconds++;

		   if(inter_time[0].seconds >= 60)
		   {
		   		   inter_time[0].seconds -= 60;
		   		   inter_time[0].minutes++;

		   		   if(inter_time[0].minutes >= 60)
		   		   {
		   			   	inter_time[0].minutes -= 60;
		   				inter_time[0].hours++;
		   				inter_time[0].hours = (inter_time[0].hours>10) ? 9 : inter_time[0].hours;
		   		   }
		   }


	   }

	   /* Wyswitlenie czasu na wywietlaczu */
	   if(running)
	   {
		   display_data[0] = (inter_time[0].cseconds % 10) | 0xF0;
		   	   display_data[1] = (inter_time[0].cseconds / 10)| 0xF0;
		   	   if (inter_time[0].cseconds == 0)
		   	   {
		   		   display_data[2] = (inter_time[0].seconds % 10) | 0x70;
		   		   display_data[3] = (inter_time[0].seconds / 10)| 0xF0;

		   		   if (inter_time[0].seconds == 0)
		   		   {
		   			   display_data[4] = (inter_time[0].minutes % 10) | 0x70;
		   			   display_data[5] = (inter_time[0].minutes / 10)| 0xF0;
		   			   if (inter_time[0].minutes == 0)
		   			   {
		   				   display_data[6] = (inter_time[0].hours % 10) | 0x70;
		   			   }
		   		   }
		   	   }
	   }

	   to_increment = (inter_time[0].cseconds < 100);

	   /* Obsluga trybow pracy*/
	   if(button_action)
	   	   {
		   	   /* Start - stop*/


	   		   	if (running)
	   		   	{
	   		   		if (state_of_buttons & START_BUTTON)
	   		   		{
	   		   			running = false;
	   		   			intertime_display_index = 0;
	   		   			display_data[7] = 0 | 0xF0;

	   		   		}

	   		   		/* Zapisywanie miedzyczasow*/
				   if ((state_of_buttons & INTERTIME_BUTTON) && !intertime_overflow)
				   {
					   save_intertime(intertime_index);
					   display_data[7] = intertime_index | 0x70;
					   if (++intertime_index >= INTERTIMES_SIZE)
					   {
						   intertime_overflow = true;
					   }
				   }



	   		   	}
	   		   	else
	   		   	{
	   		   		if (state_of_buttons & START_BUTTON)
	   		   		{
	   		   			running = true;
	   		   			display_intertime(0);

						display_data[7] = (intertime_index-1) | 0x70;

	   		   		}

	   		   		if (state_of_buttons & INTERTIME_BUTTON)
				    {

					   if (++intertime_display_index >= (intertime_index) )
					   {
						   intertime_display_index = 0;
					   }
					   display_intertime(intertime_display_index);
					   display_data[7] = intertime_display_index | 0xF0;
				   }
	   		   	}


	   		   /* Wyswoetlanie miedzyczasow*/


	   		   if(state_of_buttons & RESET_BUTTON)
	   		   {
	   			   running = false;
	   			   inter_time[0] = (stopwatch_time) {0, 0, 0, 0};
	   			   display_intertime(0);
	   			   display_data[7] = 0 | 0xF0;
	   			   intertime_index = 1;
	   			   intertime_overflow = false;
	   		   }
	   		   button_action = false;

	   	   }


	   if (!to_increment && !button_action) {
		   __low_power_mode_1();
	   }
   }
}



// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A0 (void)
{

    DISPLAY_DATA=display_data[position];
    DISPLAY_CONTROL&=~(1<<position); //wlaczenie danej pozycji
    position=(position + 1) & 0x07;
    if(running)
    	csecs_counter++;
    if(csecs_counter >= TICKS_PER_CSEK){
    	inter_time[0].cseconds++;
    	csecs_counter = 0;
    	to_increment = true;

    	__low_power_mode_off_on_exit();
    }

    TACCTL0&=~CCIFG;
}

// Timer_A3 Interrupt Vector (TAIV) handler
#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A1(void)
{
	  DISPLAY_CONTROL=0xFF; //wylaczenie pozycji
	  TACCTL1&=~CCIFG;

	  if (debounce_buttons) {
		  if (debounce_buttons & START_BUTTON) {
			  if ((state_of_buttons & START_BUTTON) == (CONTROL_INPUT & START_BUTTON)) {
				  start_button_counter--;
				  if (start_button_counter == 0) {
					  start_button_counter = DEBOUNCE_VAL;
					  button_action = true;
					  P1IES ^= START_BUTTON;
					  state_of_buttons ^= START_BUTTON;
					  debounce_buttons &= ~START_BUTTON;
				      P1IE |= START_BUTTON;
				  }
			  } else {
					start_button_counter = DEBOUNCE_VAL;
			  }
		  }

		  if (debounce_buttons & INTERTIME_BUTTON) {
			  if ((state_of_buttons & INTERTIME_BUTTON) == (CONTROL_INPUT & INTERTIME_BUTTON)) {
				  intertime_button_counter--;
				  if (intertime_button_counter == 0) {
					  intertime_button_counter = DEBOUNCE_VAL;
					  button_action = true;
					  P1IES ^= INTERTIME_BUTTON;
					  state_of_buttons ^= INTERTIME_BUTTON;
					  debounce_buttons &= ~INTERTIME_BUTTON;
					  P1IE |= INTERTIME_BUTTON;
					  }
			  } else {
				  intertime_button_counter = DEBOUNCE_VAL;
			  }

		  }
		  if (debounce_buttons & RESET_BUTTON) {
			  if ((state_of_buttons & RESET_BUTTON) == (CONTROL_INPUT & RESET_BUTTON)) {
				  reset_button_counter--;
				  if (reset_button_counter == 0) {
					  reset_button_counter = DEBOUNCE_VAL;
					  button_action = true;
					  P1IES ^= RESET_BUTTON;
					  state_of_buttons ^= RESET_BUTTON;
					  debounce_buttons &= ~RESET_BUTTON;
					  P1IE |= RESET_BUTTON;
				  }
			  } else {
				  reset_button_counter = DEBOUNCE_VAL;
			  }
		  }
		  __low_power_mode_off_on_exit();
	  }

}

// Timer_A3 Interrupt Vector (TAIV) handler
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	if (P1IFG & INTERTIME_BUTTON) {
			debounce_buttons |= INTERTIME_BUTTON;
			intertime_button_counter = DEBOUNCE_VAL;
			P1IE &= ~INTERTIME_BUTTON;
			P1IFG &= ~INTERTIME_BUTTON;

		}

	if (P1IFG & START_BUTTON) {
		debounce_buttons |= START_BUTTON;
		start_button_counter = DEBOUNCE_VAL;
		P1IE &= ~START_BUTTON;
		P1IFG &= ~START_BUTTON;

	}

	 if (P1IFG & RESET_BUTTON) {
		debounce_buttons |= RESET_BUTTON;
		start_button_counter = DEBOUNCE_VAL;
		P1IE &= ~RESET_BUTTON;
		P1IFG &= ~RESET_BUTTON;

	}

}



void save_intertime(uint8_t index_num)
{
	inter_time[index_num] = inter_time[0];
}

void display_intertime(uint8_t index_num)
{
	display_data[0] = (inter_time[index_num].cseconds % 10) | 0xF0;
	display_data[1] = (inter_time[index_num].cseconds / 10)| 0xF0;
	display_data[2] = (inter_time[index_num].seconds % 10) | 0x70;
	display_data[3] = (inter_time[index_num].seconds / 10)| 0xF0;
	display_data[4] = (inter_time[index_num].minutes % 10) | 0x70;
	display_data[5] = (inter_time[index_num].minutes / 10)| 0xF0;
	display_data[6] = (inter_time[index_num].hours % 10) | 0x70;
}

