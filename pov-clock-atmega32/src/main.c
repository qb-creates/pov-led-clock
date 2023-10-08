/*
 * pov-clock-atmega32
 *
 * Author: Quentin Baker
 *
 * Description:
 *   This program will project the image of an anolog clock using the idea of POV (Persistence of Vision).
 *   The circuit is mounted to a 3 phase brushless motor and will project the image of the clock everytime 
 *   the motor completes a full rotation. An IR Detector connected to External Interrupt 0 is used to notify 
 *   the circuit that a full rotation of the motor has occured. This will let the circuit know that it is back 
 *   at its home position. A 2 second CTC timer is used to add one second to the clock on each CTC compare 
 *   interrupt. This interrupt will be triggered every 1 second.
 *
 * MCU Details:
 *   - MCU: ATmega32
 *   - Oscillator: External 16MHz crystal
 * 
 * CTC Waveform Frequency Equation
 *   - Waveform Frequency = fck / (2 * prescaler * (1 + OCRnA)).
 * 
 * Variables
 *   - Waveform Frequency = 0.5Hz = 2 seconds
 *   - fck = 8000000Mhz
 *   - prescaler = 256
 *   - OCRnA = ?
 * 
 * Rearranged formula to solve for OCRnA:
 *   - OCRnA = (fck / (2 * prescaler * Waveform Frequency)) - 1
 * 
 * Solve for OCRnA
 *   - OCRnA = (8000000Mhz / (2 * 256 * 0.5Hz)) - 1 
 *   - OCRnA = 31249
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>
#include "povClock.h"

#define ROTATION_COMPLETE INT0_vect
#define SECONDS_TIMER TIMER1_COMPA_vect
#define OCR1A_Value 31249

void configureRotationInterupt(void);
void configureTimer(void);

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
    configureRotationInterupt();
    configureTimer();
    sei();

    while (true)
    {
        if (rotationCompleteTrigger)
        {
            projectClockImage();
            rotationCompleteTrigger = false;
        }

        if (secondsTrigger)
        {
            addSecondToClock();
            secondsTrigger = false;
        }
    }
}

/**
 * @brief Enables external interrupt request 0 (INT0). Interrupt will be triggered on each
 * full rotation of the motor.
 */
void configureRotationInterupt(void)
{
    // External Interrupt Request 0 is enabled.
    GICR |= _BV(INT0);

    // Generate an interrupt request on the falling edge of INT0.
    MCUCR |= _BV(ISC01);
}

/**
 * @brief Configures Timer1 to operate in Clear Timer on Compare (CTC) mode with compare output interrupt enabled.
 * Compare output interrupt will be triggered every 1 second.
 */
void configureTimer(void)
{
    // Selects CTC mode. Selects a Prescaler of 256.
    TCCR1B |= _BV(CS12) | _BV(WGM12);

    // Enable Compare Output interrupt.
    TIMSK |= _BV(OCIE1A);

    OCR1A = OCR1A_Value;
}