/*
 * Tetris.c
 *
 * Created: 6/14/2013 8:37:01 PM
 *  Author: Quentin
	
	SPI is initiated to operate the MAX 7219 chip.  CC is located on PinB.0
	MOSI is located on PinB.2 Clock is located on PinB.1
 */ 


#ifndef F_CPU
#define F_CPU 16000000UL // frequency of the atmega128a clock used to make delay work properly.
#endif
#include <util/delay.h>
#include <avr/signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDRB = (1 << PORTB4)|(1 << PORTB5) | (1 << PORTB7);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPSR = (0 << SPI2X);
	SPCR = (1 << SPE) | (0 << DORD) | (1 << MSTR) | (0 << CPHA) | (1 << CPOL) | (0 << SPR1) | (0 << SPR0);
}

void SPI_MasterTransmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}

void latch (void)
{
	//latches data into the seven segment driver.
	PB_4 = 1;
	PB_4 = 0;
}
void LED_Matrix_init (void)
{
	//Display test register.  It is set for normal operation
	SPI_MasterTransmit(0x0F);
	SPI_MasterTransmit(0x00);
	latch();
	
	//Shutdown register. It is set for normal operation
	SPI_MasterTransmit(0x0C);
	SPI_MasterTransmit(0x01);
	latch();
	
	//Decode mode register. It is set for no decode mode
	//Each segment can be addressed individually.
	SPI_MasterTransmit(0x09);
	SPI_MasterTransmit(0x00);
	latch();
	
	//Scan limit register. It is setup so that dig0-7 will display
	SPI_MasterTransmit(0x0B);
	SPI_MasterTransmit(0x07);
	latch();
}
