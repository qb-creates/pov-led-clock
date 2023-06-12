/*
 * esc-driver-attiny861a
 *
 * Author: Quentin Baker
 *
 * Configures a PWM output that can be used to drive an ESC (Electronic Speed Controller).
 * Input PA7 will start and stop the motor.
 * PWM Frequency = fck / (prescaller * 255).
 * 16000000Mhz / (128 * 255) = 490 Hz.
 * 
 * Using external 16Mhz crystal.
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdbool.h>

#define STOP_MOTOR_VALUE 90
#define TARGET_SPEED_VALUE 150

void configurePWMPeripheral();
void configureInput(volatile uint8_t *ddr, uint8_t pin, bool enablePullup);
bool getButtonDown(volatile uint8_t *inputRegister, int pin);

int pwmCompareValue = STOP_MOTOR_VALUE;
bool buttonPressed = false;
bool startMotor = false;

ISR(TIMER1_OVF_vect)
{
  OCR1B = pwmCompareValue;
}

int main(void)
{
  configureInput(&DDRA, PIN7, true);
  configurePWMPeripheral();
  sei();

  while (true)
  {
    if (getButtonDown(&PINA, PINA7))
    {
      startMotor = !startMotor;
      pwmCompareValue = startMotor ? TARGET_SPEED_VALUE : STOP_MOTOR_VALUE;
    }
  }
}

/**
 * @brief Configures a pin as an input.
 * 
 * @param ddr Data Direction Register that the input pin is associated with.
 * @param pin The pin number that we want to configure as an input.
 * @param enablePullup If true, the pull up resistor will be enabled for the pin.
 */
void configureInput(volatile uint8_t *ddr, uint8_t pin, bool enablePullup)
{
  *ddr &= ~_BV(pin);

  // Get the Data Register associated with the Data Direction Register 
  volatile uint8_t *port = ++ddr;

  if (enablePullup)
  {
    *port |= _BV(pin);
  }
  else
  {
    *port &= ~_BV(pin);
  }
}

/**
 * @brief Configures Timer/Couter 1B to operate in PWM mode with a frequency of 490 Hz. Enables Timer 1 Overflow interrupt.
 * Configures PB3 as an output for the PWM signal.
 */
void configurePWMPeripheral()
{
  DDRB |= _BV(PB3);

  // Enable PWM mode based on comparator OCR1B. Clear OC1B on match and set OC1B when TCNT1 = 0x000;
  TCCR1A |= _BV(COM1B1) | _BV(PWM1B);

  // Selects a Prescaler of 128.
  TCCR1B |= _BV(CS13);

  // Enable Overflow interrupt for Timer 1.
  TIMSK = 1 << TOIE1;

  OCR1B = pwmCompareValue;
}

/**
 * @brief Get the down/pressed state of the specified button.
 *
 * @param inputRegister The input register address of the pin we want to check.
 * @param pin The pin the button is connected to.
 * @return Will return true when the button is pressed down. Will return false until the button has been released and pressed again.
 */
bool getButtonDown(volatile uint8_t *inputRegister, int pin)
{
  bool isPinLow = !(*inputRegister & _BV(pin));
  bool isPinHigh = *inputRegister & _BV(pin);

  if (isPinLow && !buttonPressed)
  {
    _delay_ms(300);
    buttonPressed = true;
    return true;
  }
  else if (isPinHigh && buttonPressed)
  {
    _delay_ms(300);
    buttonPressed = false;
  }
  return false;
}