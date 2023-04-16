//This one particularly shows how to set up Pulse Width Modulation with Phase Correct
//The registers used are TCCR0, OCR0, and ASSR
//A value can be read from TCNT0
//With the PWM the counter counts from 0 to 0xFF regardless and it counts back down
//Frequency of PWM signal = Fclk or board/(pre-scaler*510) This is the time it takes
//to count from up to down.
//Right now the pre-scaler is 1024 and the FPWM is = 30.63Hz = 32.64ms     
//This also houses the random number generator               

#include <util/delay.h>
#include <avr/io.h>
#include <avr/signal.h>
#include <stdlib.h>
int num = 79;
int a = 276;
int b = 71;

int random_num ()
{
	num = ((a*num + b)%TCNT0);
	return num;
}


void PWM_16bit_initialize()
{
//	This chooses what mode you want to operate in, your pre-scaler and what you want your OCR1A, OCR1B, and OCR1C to do.
	TCCR1A =  (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) | (0 << WGM11) | (0 << WGM10);
	TCCR1B =  (1 << ICNC1) | (0 << ICES1) | (0 << WGM13) | (0 << WGM12)| (1 << CS12) | (0 << CS11) | (1 << CS10);
	TIMSK = (1 << TICIE1) | (1 << OCIE1A); // interrupt for input capture enabled
	OCR1A = 7812.5;
}

void PWM_8bit_initialize()
{
    // Sets up a counter that will trigger a compare output every 1ms.
	TCCR0 = (1 << WGM01) | (1 << CS01) | (1 << CS00); 
	OCR0 = 125;   
	TIMSK = 1 << OCIE0;
}