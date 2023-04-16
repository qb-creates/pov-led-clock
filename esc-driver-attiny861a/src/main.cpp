/*
 * esc-driver-attiny861a
 *
 * Author: Quentin Baker
 *
 * Configures a PWM output that can be used to drive an ESC (Electronic Speed Controller).
 * Input PA0 will start and stop the motor.
 * PWM Frequency = fck / (prescaller * 255).
 * 16000000Mhz / (128 * 255) = 490 Hz.
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

void configureInputs();
void configurePWMPeripheral();
bool getButtonState(volatile uint8_t *inputRegister, int pin);

int pwmCompareValue = 90;
bool buttonArmed = true;
bool start = false;

ISR(TIMER1_OVF_vect)
{
  OCR1B = pwmCompareValue;
}

int main(void)
{
  configureInputs();
  configurePWMPeripheral();
  sei();
  
  while (true)
  {
    bool pressed = getButtonState(&PINA, PIN7);

    if (pressed)
    {
      start = !start;
      pwmCompareValue = start ? 150 : 90;
    }
  }
}

void configureInputs()
{
  // Configure PA0 as an input.
  DDRA = ~(_BV(PA0) | _BV(PA1));

  // Enable Pullup resistors for PA0 and PA1.
  PORTA = 0xFF;
}

void configurePWMPeripheral()
{
  // Configure PB3 as an output
  DDRB |= _BV(PB3);

  // Enable PWM mode based on comparator OCR1B. Clear OC1B on match and set OC1B when TCNT1 = 0x000;
  TCCR1A |= _BV(COM1B1) | _BV(PWM1B);

  // Selects a Prescaler of 128.
  TCCR1B |= _BV(CS13);

  // Enable Overflow interupt for Timer 1.
  TIMSK = 1 << TOIE1;

  OCR1B = pwmCompareValue;
}

bool getButtonState(volatile uint8_t *inputRegister, int pin)
{
  int inputData = *inputRegister;

  // Button is pressed if the bit is logic low.
  if (!(inputData & _BV(pin)) && !buttonArmed)
  {
    // Delay to account for debounce.
    _delay_ms(300);
    buttonArmed = true;
    return true;
  }
  else if ((inputData & _BV(pin)) && buttonArmed)
  {
    // Delay to account for debounce.
    _delay_ms(300);
    buttonArmed = false;
  }
  return false;
}