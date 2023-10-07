/*
 * esc-driver-attiny861a
 *
 * Author: Quentin Baker
 * MCU: AtTiny861a
 * Clock: External 16MHz crystal
 *
 * Configures a PWM output that can be used to drive an ESC (Electronic Speed Controller).
 * A SPST normally open push button connected to PINA7 will be used to start and stop the motor. 
 * Button debouncing is accomplished in software by using a 300ms delay after detecting the 
 * first pulse before checking again. 
 *
 * PWM Frequency = fck / (prescaller * 255).
 * 16000000Mhz / (128 * 255) = 490 Hz.
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>

#define STOP_MOTOR_COMPARE_VALUE 127  // Duty cycle of 50%. Time high will be 1ms
#define START_MOTOR_COMPARE_VALUE 150 // Duty cycle of 59%. Time high will be 1.18ms

void configurePWM();
void configureButtonInput();
bool getButtonDown();

int pwmCompareValue = STOP_MOTOR_COMPARE_VALUE;
bool buttonPressed = false;
bool startMotor = false;

ISR(TIMER1_OVF_vect)
{
    OCR1B = pwmCompareValue;
}

int main(void)
{
    configureButtonInput();
    configurePWM();
    sei();
    
    while (true)
    {
        if (getButtonDown())
        {
            startMotor = !startMotor;
            pwmCompareValue = startMotor ? START_MOTOR_COMPARE_VALUE : STOP_MOTOR_COMPARE_VALUE;
        }
    }
}

/**
 * @brief Configures PINA7 as input.
 */
void configureButtonInput()
{
    // Configures PINA7 as an input.
    DDRA &= ~_BV(PA7);

    // Enables the pull up resistor for PINA7
    PORTA |= _BV(PA7);
}

/**
 * @brief Configures Timer 1B to operate in PWM mode with a frequency of 490 Hz.
 */
void configurePWM()
{
    // Configure PB3 as an output.
    DDRB |= _BV(PB3);

    // Enable PWM mode based on comparator OCR1B. Clear OC1B on match and set OC1B when TCNT1 = 0x000;
    TCCR1A |= _BV(COM1B1) | _BV(PWM1B);

    // Selects a Prescaler of 128.
    TCCR1B |= _BV(CS13);

    // Enable Overflow interrupt for Timer 1.
    TIMSK = 1 << TOIE1;

    // Sets the duty cycle.
    OCR1B = pwmCompareValue;
}

/**
 * @brief Will return the down/pressed state of the button located on PINA7.
 * 
 * @return Will return true the first time the button is pressed. Will return false until the button has been released and pressed again.
 */
bool getButtonDown()
{
    bool isPinHigh = PINA & _BV(PINA7);

    if (!isPinHigh && !buttonPressed)
    {
        buttonPressed = true;
        _delay_ms(300);

        return true;
    }
    else if (isPinHigh && buttonPressed)
    {
        buttonPressed = false;
        _delay_ms(300);
    }

    return false;
}