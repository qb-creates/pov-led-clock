/*
 * pov-clock-atmega32
 *
 * Author: Quentin Baker
 *
 * The full rotation time of lights equal to 28.62ms. This entire portion of code divides the time into 60
 * steps to represent 60 seconds and 60 minutes. Therefore, the 28.62ms was divided by 60 to 447us.
 * There is 60 instances of 447us to represent the 28.62ms full rotation.  In places where complex
 * animations like numbers need to be formed, the 666us were divided by 6 to have room to play with animation times.
 * This value became 79.5us.
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

bool rotationTrigger = false;
bool secondsTrigger = false;

ISR(ROTATION_COMPLETE)
{
  if (!rotationTrigger)
  {
    rotationTrigger = true;
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
    if (rotationTrigger)
    {
      drawClock();
      rotationTrigger = false;
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
  GICR |= _BV(INT0);   // Sets it up so that INT3 and INT0 are activate
  MCUCR |= _BV(ISC01); // Sets up INT3:0 to be activated whenever a logic low is detected
  sei();               // Enables interrupts
}

void configureTimer()
{
  // Selects CTC mode. Selects a Prescaler of 256.
  TCCR1B |= _BV(CS12) | _BV(WGM12);

  // Enable Compare Output interrupt.
  TIMSK |= _BV(OCIE1A);
  // OCR1A = 31249;
  OCR1A = 1000;
}