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
#include "spiUtility.h"

struct Frame
{
  int byte1;
  int byte2;
  int byte3;
  int byte4;
} volatile frames[6];

void configureInterupts();
void configureTimer();

volatile bool startAnimation = false;
volatile int Seconds_count = 59;
volatile int Minute_count = 59;
volatile int Hour_count = 59;
volatile int Hour_set1 = 0;
volatile int Hour_set2 = 0;
volatile int Hour_set3 = 0;
volatile int Hour_set4 = 0;

ISR(INT0_vect)
{
  if (!startAnimation)
  {
    startAnimation = true;
  }
}

ISR(TIMER1_COMPA_vect)
{
  --Seconds_count;
  if (Seconds_count < 0)
  {
    Seconds_count = 59;
    --Minute_count;
  }

  if (Minute_count < 47 && Minute_count >= 35 && Hour_set1 == 0)
  {
    Hour_count--;
    Hour_set1 = 1;
    Hour_set2 = 0;
    Hour_set3 = 0;
    Hour_set4 = 0;
  }

  if (Minute_count < 35 && Minute_count >= 23 && Hour_set2 == 0)
  {
    Hour_count--;
    Hour_set2 = 1;
    Hour_set3 = 0;
    Hour_set4 = 0;
  }

  if (Minute_count < 23 && Minute_count >= 12 && Hour_set3 == 0)
  {
    Hour_count--;
    Hour_set3 = 1;
    Hour_set4 = 0;
  }

  if (Minute_count < 12 && Minute_count >= 0 && Hour_set4 == 0)
  {
    Hour_count--;
    Hour_set4 = 1;
  }

  if (Minute_count < 0)
  {
    Minute_count = 59;
    Hour_count--;
    Hour_set1 = 0;
    Hour_set2 = 0;
    Hour_set3 = 0;
    Hour_set4 = 0;
  }

  if (Hour_count < 0)
  {
    Hour_count = 59;
  }
}

void LED_Set()
{
  /**This is where the instances of 666us are separated by six for animation design.
   each additional frame adds on to how long the lights in that particular byte stay on
   or if they are on at*/
  for (int i = 0; i < 6; i++)
  { 
    SPCR &= ~_BV(DORD);
    transmitData(frames[i].byte4);
    SPCR |= _BV(DORD);
    transmitData(frames[i].byte3);
    SPCR &= ~_BV(DORD);
    transmitData(frames[i].byte2);
    SPCR |= _BV(DORD);
    transmitData(frames[i].byte1);

    // Latch Date
    PORTB |= _BV(PB4);
    PORTB &= !(_BV(PB4));

    _delay_us(76.5);
  }
}

void LED_On(int Frame1_Byte3_num, int Frame2_Byte3_num, int Frame3_Byte3_num, int Frame4_Byte3_num, int Frame5_Byte3_num, int Frame6_Byte3_num,
            int Frame1_Byte4_num, int Frame2_Byte4_num, int Frame3_Byte4_num, int Frame4_Byte4_num, int Frame5_Byte4_num, int Frame6_Byte4_num, int Hours_num, int Minutes_num, int Seconds_num)
{
  for (int i = 0; i < 6; i++)
  {
    frames[i].byte1 = 0x00;
    frames[i].byte2 = 0x00;
    frames[i].byte3 = 0x00;
    frames[i].byte4 = 0x00;
  }

  frames[0].byte1 = 0x80;
  frames[1].byte1 = 0x80;
  frames[2].byte1 = 0x80;
  frames[3].byte1 = 0x80;
  frames[4].byte1 = 0x80;
  frames[5].byte1 = 0x80;

  frames[0].byte3 = Frame1_Byte3_num;
  frames[1].byte3 = Frame2_Byte3_num;
  frames[2].byte3 = Frame3_Byte3_num;
  frames[3].byte3 = Frame4_Byte3_num;
  frames[4].byte3 = Frame5_Byte3_num;
  frames[5].byte3 = Frame6_Byte3_num;

  frames[0].byte4 = Frame1_Byte4_num;
  frames[1].byte4 = Frame2_Byte4_num;
  frames[2].byte4 = Frame3_Byte4_num;
  frames[3].byte4 = Frame4_Byte4_num;
  frames[4].byte4 = Frame5_Byte4_num;
  frames[5].byte4 = Frame6_Byte4_num;

  if (Hour_count == Hours_num)
  { // Minute Hand If statement that makes sure the Minute hand is visible when the seconds light has reached its location
    frames[2].byte1 |= 255;
    frames[3].byte1 |= 255;

    frames[0].byte2 |= 0b00001100;
    frames[1].byte2 |= 0b00001110;
    frames[2].byte2 |= 0b11111111;
    frames[3].byte2 |= 0b11111111;
    frames[4].byte2 |= 0b00001110;
    frames[5].byte2 |= 0b00001100;
  }
  if (Minute_count == Minutes_num)
  { // Minute Hand If statement that makes sure the Minute hand is visible when the seconds light has reached its location
    frames[2].byte1 |= 255;
    frames[3].byte1 |= 255;
    frames[2].byte2 |= 0b11111111;
    frames[3].byte2 |= 0b11111111;

    frames[0].byte3 |= 0b11000000;
    frames[1].byte3 |= 0b11100000;
    frames[2].byte3 |= 0b11110000;
    frames[3].byte3 |= 0b11110000;
    frames[4].byte3 |= 0b11100000;
    frames[5].byte3 |= 0b11000000;
  }

  if (Seconds_count <= Seconds_num)
  { // Minute Hand If statement that makes sure the Minute hand is visible when the seconds light has reached its location

    frames[0].byte4 |= 0b00000001;
    frames[1].byte4 |= 0b00000001;
    frames[2].byte4 |= 0b00000001;
    frames[3].byte4 |= 0b00000001;
    frames[4].byte4 |= 0b00000001;
    frames[5].byte4 |= 0b00000001;
  }
  LED_Set();
}

int main(void)
{
  configureInterupts();
  configureSPIMasterMode();
  configureTimer();
  sei();

  int jj = 1;
  int ii = 0;

  while (ii < 8)
  {
    transmitData(0x00);
    transmitData(0x00);
    transmitData(0x00);
    transmitData(jj);
    PORTB |= _BV(PB4);
    PORTB &= !(_BV(PB4));
    _delay_ms(40);
    jj = jj << 1;
    ++ii;
  }

  jj = 128;
  ii = 0;

  while (ii < 8)
  {
    transmitData(0x00);
    transmitData(0x00);
    transmitData(jj);
    transmitData(0x00);
    PORTB |= _BV(PB4);
    PORTB &= !(_BV(PB4));
    _delay_ms(40);
    jj = jj >> 1;
    ++ii;
  }

  while (true)
  {
    if (startAnimation)
    {
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 59, 59);
      /**	This portion starts the animation for the Twelve on the clock.This begins the Seconds counter at the top of the clock.Because this is half way of the total 40ms, I started the counter
      here so that the seconds animation would start at	the top of the clock as opposed to the bottom where the IR transmitter is located.
      *************************************************************************************************************************************************************************************************/
      LED_On(0b00001100, 0b00001110, 0b00001111, 0b00001101, 0b00001100, 0b00001100, 0b00001100, 0b00001110, 0b00000110, 0b11000110, 0b11111110, 0b00111000, 23, 58, 58);
      /**This portion Ends the animation for the Twelve on the clock.**********************************************************************************************************************************/
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 57, 57);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 56, 56);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 55, 55);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 254, 254, 0, 0, 19, 54, 54); // Notch one
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 53, 53);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 52, 52);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 51, 51);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 50, 50);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 254, 254, 0, 0, 14, 49, 49); // Notch two
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 48, 48);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 47, 47);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 46, 46);
      /**This portion Begins the animation for the Three on the clock.*********************************************************************************************************************************/
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0b00111100, 0b00111110, 0b00000110, 0b00000110, 10, 45, 45);
      LED_On(0, 0, 0, 0, 0, 0, 0b00000110, 0b00000110, 0b00111000, 0b00111000, 0b00000110, 0b00000110, 9, 44, 44);
      LED_On(0, 0, 0, 0, 0, 0, 0b00000110, 0b00000110, 0b00111110, 0b00111100, 0, 0, 8, 43, 43);
      /**This portion Ends the animation for the Three on the clock.***********************************************************************************************************************************/
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 42, 42);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 41, 41);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 40, 40);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 254, 254, 0, 0, 4, 39, 39); // Notch three
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 38, 38);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 37, 37);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 36, 36);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 35);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 254, 254, 0, 0, 59, 34, 34); // Notch four
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 58, 33, 33);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, 32, 32);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 31, 31);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 55, 30, 30);
      /**This portion Starts the animation for the Six on the clock.*******************************************************************************************************************************/
      LED_On(0, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00111000, 0b01111100, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 54, 29, 29);
      LED_On(0b00000111, 0b00000011, 0, 0, 0, 0, 0b11111100, 0b11111100, 0, 0, 0, 0, 53, 28, 28);
      /**This portion Finishes the animation for the Six on the clock.****************************************************************************************************************************/
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 52, 27, 27);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 51, 26, 26);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 25, 25);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 254, 254, 0, 0, 49, 24, 24); // Notch five
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 23, 23);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, 22, 22);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 46, 21, 21);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, 20, 20);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 254, 254, 0, 0, 44, 19, 19); // Notch six
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, 18, 18);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 17, 17);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, 16, 16);
      /**This portion starts the animation for the Nine on the clock.**********************************************************************************************************************************/
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0b00111100, 0b01111100, 0b11000000, 0b11000000, 40, 15, 15);
      LED_On(0, 0, 0, 0, 0, 0, 0b11000000, 0b11000000, 0b11111100, 0b11111110, 0b11000110, 0b11000110, 39, 14, 14);
      LED_On(0, 0, 0, 0, 0, 0, 0b11000110, 0b11000110, 0b11111100, 0b00111000, 0, 0, 38, 13, 13);
      /**This portion Ends the animation for the Nine on the clock.************************************************************************************************************************************/
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 37, 12, 12);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36, 11, 11);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 10, 10);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 254, 254, 0, 0, 34, 9, 9); // Notch Seven
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 33, 8, 8);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 7, 7);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 6, 6);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 5, 5);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 254, 254, 0, 0, 29, 4, 4); // Notch Eight
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 3, 3);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 27, 2, 2);
      LED_On(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26, 1, 1);
      /**	This portion Finishes the animation for the Twelve on the clock.
      *************************************************************************************************************************************************************************************************/
      LED_On(0, 0b00001100, 0b00001111, 0b00001111, 0b00001111, 0b00001100, 0, 0b00000100, 0b11111100, 0b11111110, 0b11111110, 0, 25, 0, 0);
      char data[4] = {128, 0, 0, 128};
      transmitChars(data, 4);
      PORTB |= _BV(PB4);
      PORTB &= !(_BV(PB4));
      startAnimation = false;
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
  OCR1A = 31249;
}