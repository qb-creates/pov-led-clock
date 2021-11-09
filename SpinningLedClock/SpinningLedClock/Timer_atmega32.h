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
	OCR1A = 15606;//15625
}

void PWM_8bit_initialize()
{

    	//This chooses your Compare output mode, your pre-scaler and what you want your OC0 to do.  The OC0 pin has to be set as an output to get a reading.
	//Right now it is set up for PWM mode with a 50% duty cycle. OC0 is set up to clear on compare match. It is also set up with a clk/32 pre-scaler.
	TCCR0 = (0 << WGM01) | (0 << WGM00) | (0 << CS02) | (1 << CS01) | (1 << CS00) | (0 << COM01) | (0 << COM00); 
	TIFR = (0 << OCF0);
	OCR0 = 18;   //This value is determined by what you want your duty cycle and average voltage
				  // to be.  It is set to 127 for a %50 duty cycle because OC0 toggles
				  // every time there is a compare match between TCNT0 and OCR0.

}