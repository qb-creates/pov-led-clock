/*
 * pov-clock-atmega32
 *
 * Author: Quentin Baker
 *
 * The clock is rotating at about 1500 rpm. One rotation takes about 40ms (± 1.6ms). The time for one rotation will be divided by 60.
 * This will give us 60 steps of 666.67us. Each animation will be drawn in during that 666.67us timeframe.
 * There are 60 instances of 666.67us to represent the 40ms full rotation.  In places where complex
 * animations like numbers need to be drawn, the 666us will be divided by 6 to have room to play with animation times.
 * This value became 111.111us. 
 * 
 * Using the ATmega32's internal 8Mhz oscillator.
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdbool.h>
#include "povClock.h"

#define ROTATION_COMPLETE INT0_vect
#define SECONDS_TIMER TIMER1_COMPA_vect

void configureInterupts();
void configureTimer();

volatile bool rotationCompleteTrigger = false;
volatile bool secondsTrigger = false;

ISR(ROTATION_COMPLETE)
{
  if (!rotationCompleteTrigger)
  {
    rotationCompleteTrigger = true;
  }
}

ISR(SECONDS_TIMER)
{
  if (!secondsTrigger)
  {
    secondsTrigger = true;
  }
}

int main(void)
{
  initializeClock();
  configureInterupts();
  configureTimer();

  while (true)
  {
    if (rotationCompleteTrigger)
    {
      drawClock();
      rotationCompleteTrigger = false;
    }

    if (secondsTrigger)
    {
      updateHandPositions();
      secondsTrigger = false;
    }
  }
}

void configureInterupts()
{
  GICR |= _BV(INT0);   // External Interrupt Request 0 is enabled
  MCUCR |= _BV(ISC01); // The falling edge of INT0 generates an interrupt request
  sei();               
}

void configureTimer()
{
  // Selects CTC mode. Selects a Prescaler of 256.
  TCCR1B |= _BV(CS12) | _BV(WGM12);

  // Enable Compare Output interrupt.
  TIMSK |= _BV(OCIE1A);

  // We want our compare output interrupt to be trigger every 1 second.
  OCR1A = (F_CPU / (2 * 256 * .5)) - 1;
}