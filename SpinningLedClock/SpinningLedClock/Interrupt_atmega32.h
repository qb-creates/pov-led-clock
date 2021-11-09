/*
 * Interrupts.c
 *
 * Created: 5/24/2013 12:42:42 AM
 *  Author: Quentin
 */ 


//This program shows how to set up interrupts with the atmega128m mini development board.
//It also shows how to setup input and outputs on the DDRs and it shows how to tell if a
//pin is high or low.


#include <util/delay.h>
#include <avr/io.h>
#include <avr/signal.h>
#include <stdlib.h>

void Interrupt_initialize(void)
{
	GICR = (0 << INT1) | (1 << INT0); //Sets it up so that INT3 and INT0 are activate
	MCUCR= 0b00000011; //Sets up INT3:0 to be activated whenever a logic low is detected
	sei();  //Enables interrupts
}