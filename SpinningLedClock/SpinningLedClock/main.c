/*
 * POV_LED_Display.c
 *
 * Created: 6/15/2015 8:13:12 PM
 *  Author: Quentin
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL // frequency of the atmega128a clock used to make delay work properly.
#endif
#include <util/delay.h>
#include <avr/io.h>
#include <avr/signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <avr/iom32.h>
#include "Ports_lib.h"
#include "Interrupt_atmega32.h"
#include "SPI_atmega32.h"
#include "Timer_atmega32.h"
#define Timer_Counter TCNT1
void TransmitData();
void LED_Set();
volatile long int Delay_Count = 0;
volatile int Seconds_count = 59;
volatile int Minute_count = 59;
volatile int Hour_count = 59;

int Delay1 = 0;

int Frame_Byte1 = 0x01;
int Frame_Byte2 = 0x00;
int Frame_Byte3 = 0x00;
int Frame_Byte4 = 0x01;

int Frame1_Byte1 = 0x01;
int Frame1_Byte2 = 0;
int Frame1_Byte3 = 0;
int Frame1_Byte4 = 0;

int Frame2_Byte1 = 0x01;
int Frame2_Byte2 = 0;
int Frame2_Byte3 = 0;
int Frame2_Byte4 = 0;

int Frame3_Byte1 = 0x01;
int Frame3_Byte2 = 0;
int Frame3_Byte3 = 0;
int Frame3_Byte4 = 0;

int Frame4_Byte1 = 0x01;
int Frame4_Byte2 = 0;
int Frame4_Byte3 = 0;
int Frame4_Byte4 = 0;

int Frame5_Byte1 = 0x01;
int Frame5_Byte2 = 0;
int Frame5_Byte3 = 0;
int Frame5_Byte4 = 0;

int Frame6_Byte1 = 0x01;
int Frame6_Byte2 = 0;
int Frame6_Byte3 = 0;
int Frame6_Byte4 = 0;

int Hour_set1 = 0;
int Hour_set2 = 0;
int Hour_set3 = 0;
int Hour_set4 = 0;
volatile int Start_Clock = 0;
volatile int start_animation = 0;
volatile int animation_done = 1;

long int Previous_Period = 0;
long int Previous_PulseWidth_High = 0;
long int Previous_PulseWidth_Low = 0;
long int Start_Time = 0;
long int Stop_Time = 0;
volatile unsigned long int IR_Value = 0;
int Edge_Toggle = 0b10000101;			//Sets the 16bit timer to 1024 pre-scaler, input capture edge select for falling edge and to be set up in normal mode
int Rising_Edge = 0b11000101;			//Sets the 16bit timer to 1024 pre-scaler, input capture edge select for Rising edge and to be set up in normal mode
int Falling_Edge = 0b10000101;			//Sets the 16bit timer to 1024 pre-scaler, input capture edge select for falling edge and to be set up in normal mode
int Usart_storage = 0;
int IR_Count = 0;
int ii = 0;
int jj = 128;

void Hour_IF()
{
	if(Hour_count == 59 || Hour_count == 54 || Hour_count == 49 || Hour_count == 44 || Hour_count == 39 || Hour_count == 34 ||
	Hour_count == 29 || Hour_count == 24 || Hour_count == 19 || Hour_count == 14 || Hour_count == 9 || Hour_count == 4) {
		
		if(Minute_count < 47 && Minute_count >= 35){ Hour_set1 = 1;}
		if(Minute_count < 35 && Minute_count >= 23){ Hour_set2 = 1;}
		if(Minute_count < 23 && Minute_count >= 12){ Hour_set3 = 1;}
		if(Minute_count < 12 && Minute_count >= 0){ Hour_set4 = 1;}
	}
}

//Interrupt vector INT0 which is located on PIND0
//Testing interrupts and IRs
ISR(INT0_vect)
{
	start_animation = 1;
}

ISR(TIMER1_COMPA_vect)
{
		Timer_Counter = 0;
		if(Start_Clock == 1)
		{
			--Seconds_count;
			if(Seconds_count < 0){Seconds_count = 59; --Minute_count; }
			if(Minute_count < 47 && Minute_count >= 35 && Hour_set1 == 0){--Hour_count; Hour_set1 = 1;  Hour_set2 = 0; Hour_set3 = 0; Hour_set4 = 0;}
			if(Minute_count < 35 && Minute_count >= 23 && Hour_set2 == 0){--Hour_count; Hour_set2 = 1;  Hour_set3 = 0; Hour_set4 = 0;}
			if(Minute_count < 23 && Minute_count >= 12 && Hour_set3 == 0){--Hour_count; Hour_set3 = 1;  Hour_set4 = 0;}
			if(Minute_count < 12 && Minute_count >= 0 && Hour_set4 == 0){--Hour_count; Hour_set4 = 1;}
			if(Minute_count < 0){Minute_count = 59; --Hour_count; Hour_set1 = 0; Hour_set2 = 0; Hour_set3 = 0; Hour_set4 = 0;}
			
			if (Hour_count < 0){Hour_count = 59;}
		}
}


ISR(TIMER1_CAPT_vect)
{
	
	if(Edge_Toggle == Falling_Edge)
	{
		//The Calculations for the previous Period and Pulse widths are done first. The new start and stop Timestamps are then saved. It is one big cycle.
		Previous_Period = ICR1 - Start_Time;									//The Previous_Period for the previous time snapshots is calculated by subtracting the previous start time (Start_Time) from the current start time (ICR1)
		Previous_PulseWidth_Low = Stop_Time - Start_Time;						//The Previous_PulseWidth_Low for the previous time snapshots is calculated by subtracting the Previous Stop and Start Times
		Previous_PulseWidth_High = Previous_Period - Previous_PulseWidth_Low;	//The Previous_PulseWidth_ for the previous time snapshots is calculated by subtracting the Previous period and Pulse Width Time Low.
		Start_Time = ICR1;														//Sets the Previous Start_Time
		Edge_Toggle = Rising_Edge;												// Sets the Edge_Toggle to the value of the Rising_Edge.
		
		// Algorithm to find the Hex value of the Timestamps gathered. It is in the Falling Edge If statement so
		// it is performed after the calculations are made and so that the bits are only set one time.
		// A bit is set to High if the Rise time is between 3 and 5 and if the Falling time is between 10 and 15.
		if (Previous_PulseWidth_Low <= 15 && (Previous_PulseWidth_High >= 23) && (Previous_PulseWidth_High <= 35))
		{
			// Shifts the bits over 1 and then places a 1 in the LSB
			IR_Value = IR_Value	<< 1;
			IR_Value = IR_Value	| 1;
			
			//A bit is set to Low if the Rise time is between 3 and 5 and if the Falling time is between 3 and 5.
		} else if(Previous_PulseWidth_Low <= 15 && Previous_PulseWidth_High <= 15 && Previous_PulseWidth_Low >= 7 && Previous_PulseWidth_High >= 7)
		{
			// Shifts the bits over 1 and then places a 0 in the LSB
			IR_Value = IR_Value << 1;
		}
		else {}//If neither of the criteria are met, then nothing is done.
		
	}
	else
	{
		Stop_Time = ICR1; //Sets the Previous Stop_Time
		Edge_Toggle = Falling_Edge; // Sets the Edge_Toggle to the value of the Falling_edge.
	}
	
	
	TCCR1B = Edge_Toggle; //Toggles the Interrupt Edge
	TIFR = (1 << ICF1); // clears the input capture interrupt so a change of edge toggle does not trigger an interrupt

	if(IR_Value	 == 0x10EFA05F && Start_Clock == 0) {--Minute_count; if(Minute_count < 0){Minute_count = 59;} IR_Value = 0;}
	if(IR_Value	 == 0x10EF00FF && Start_Clock == 0) {++Minute_count; if(Minute_count == 60){Minute_count = 0;} IR_Value = 0;}
	if(IR_Value	 == 0x10EF10EF && Start_Clock == 0) {Hour_count = Hour_count - 5; if(Hour_count < 0){Hour_count = 59;}  IR_Value = 0; Hour_IF();}		
	if(IR_Value	 == 0x10EF807F && Start_Clock == 0) {Hour_count = Hour_count + 5; if(Hour_count > 60){Hour_count = 4;}  IR_Value = 0; Hour_IF();}	
	
	if(IR_Value	 == 0x10EFD827) 
	{
		if(Start_Clock == 0)
		{
			Start_Clock = 1;
			Timer_Counter = 0;
			Seconds_count = 59;
		} else {Start_Clock = 0;}
			IR_Value = 0;
			
		if(Minute_count < 47 && Minute_count > 35){ Hour_count = Hour_count - 1; Hour_set1 = 1;  Hour_set2 = 0; Hour_set3 = 0; Hour_set4 = 0;}
		if(Minute_count < 35 && Minute_count > 23){ Hour_count = Hour_count - 2; Hour_set1 = 1;  Hour_set2 = 1; Hour_set3 = 0; Hour_set4 = 0;}
		if(Minute_count < 23 && Minute_count > 12){ Hour_count = Hour_count - 3; Hour_set1 = 1;  Hour_set2 = 1; Hour_set3 = 1; Hour_set4 = 0;}
		if(Minute_count < 12 && Minute_count >= 0){ Hour_count = Hour_count - 4; Hour_set1 = 1;  Hour_set2 = 1; Hour_set3 = 1; Hour_set4 = 1;}
	}	
	++IR_Count;
}

void TransmitData(int Byte_1,int Byte_2,int Byte_3,int Byte_4)
{
	SPCR = (1 << SPE) | (1 << DORD) | (1 << MSTR) | (0 << CPHA) | (1 << CPOL) | (0 << SPR1) | (0 << SPR0);
	SPI_MasterTransmit(Byte_4);
	SPCR = (1 << SPE) | (0 << DORD) | (1 << MSTR) | (0 << CPHA) | (1 << CPOL) | (0 << SPR1) | (0 << SPR0);
	SPI_MasterTransmit(Byte_3);
	SPI_MasterTransmit(Byte_2);
	SPCR = (1 << SPE) | (1 << DORD) | (1 << MSTR) | (0 << CPHA) | (1 << CPOL) | (0 << SPR1) | (0 << SPR0);
	SPI_MasterTransmit(Byte_1);
	latch();
}

void LED_Set()
{
	/**This is where the instances of 666us are separated by six for animation design.
	 each additional frame adds on to how long the lights in that particular byte stay on
	 or if they are on at*/
	for(Delay1 = 0; Delay1 == 0; Delay1++)
	{
	TransmitData(Frame1_Byte1, Frame1_Byte2, Frame1_Byte3, Frame1_Byte4 ) ;
	if(IR_Count == 0){_delay_us(69.7);} else  {_delay_us(55); IR_Count = 0;}
		
	TransmitData(Frame2_Byte1, Frame2_Byte2, Frame2_Byte3, Frame2_Byte4 ) ;
	if(IR_Count == 0){_delay_us(69.7);} else  {_delay_us(55); IR_Count = 0;}
		
	TransmitData(Frame3_Byte1, Frame3_Byte2, Frame3_Byte3, Frame3_Byte4 ) ;
	if(IR_Count == 0){_delay_us(69.7);} else  {_delay_us(55); IR_Count = 0;}
		
	TransmitData(Frame4_Byte1, Frame4_Byte2, Frame4_Byte3, Frame4_Byte4 ) ;
	if(IR_Count == 0){_delay_us(69.7);} else  {_delay_us(55); IR_Count = 0;}
		
	TransmitData(Frame5_Byte1, Frame5_Byte2, Frame5_Byte3, Frame5_Byte4 ) ;
	if(IR_Count == 0){_delay_us(69.7);} else  {_delay_us(55); IR_Count = 0;}
		
	TransmitData(Frame6_Byte1, Frame6_Byte2, Frame6_Byte3, Frame6_Byte4 ) ;
	if(IR_Count == 0){_delay_us(69.7);} else  {_delay_us(55); IR_Count = 0;}
	}
	
	Frame1_Byte1 = 0x80;
	Frame2_Byte1 = 0x80;
	Frame3_Byte1 = 0x80;
	Frame4_Byte1 = 0x80;
	Frame5_Byte1 = 0x80;
	Frame6_Byte1 = 0x80;
	
	Frame1_Byte2 = 0;
	Frame2_Byte2 = 0;
	Frame3_Byte2 = 0;
	Frame4_Byte2 = 0;
	Frame5_Byte2 = 0;
	Frame6_Byte2 = 0;
	
	Frame1_Byte3 = 0;
	Frame2_Byte3 = 0;
	Frame3_Byte3 = 0;
	Frame4_Byte3 = 0;
	Frame5_Byte3 = 0;
	Frame6_Byte3 = 0;
	
	Frame1_Byte4 = 0;
	Frame2_Byte4 = 0;
	Frame3_Byte4 = 0;
	Frame4_Byte4 = 0;
	Frame5_Byte4 = 0;
	Frame6_Byte4 = 0;
	
}
void LED_On(int Frame1_Byte3_num, int Frame2_Byte3_num, int Frame3_Byte3_num, int Frame4_Byte3_num, int Frame5_Byte3_num, int Frame6_Byte3_num,
					int Frame1_Byte4_num, int Frame2_Byte4_num, int Frame3_Byte4_num, int Frame4_Byte4_num, int Frame5_Byte4_num, int Frame6_Byte4_num, int Hours_num, int Minutes_num, int Seconds_num)
{
	if (Hour_count == Hours_num && Minute_count == Minutes_num && Seconds_count <= Seconds_num){ //Minute Hand If statement that makes sure the Minute hand is visible when the seconds light has reached its location
				Frame3_Byte1 = 255; Frame4_Byte1 = 255;
				Frame1_Byte2 = 0b00001100; Frame2_Byte2 = 0b00001110; Frame3_Byte2 = 0b11111111; Frame4_Byte2 = 0b11111111; Frame5_Byte2 = 0b00001110; Frame6_Byte2 = 0b00001100;
			
				Frame1_Byte3 = 0b11000000 | Frame1_Byte3_num; Frame2_Byte3 = 0b11100000 | Frame2_Byte3_num; Frame3_Byte3 = 0b11110000 | Frame3_Byte3_num; Frame4_Byte3 = 0b11110000 | Frame4_Byte3_num; Frame5_Byte3 = 0b11100000 | Frame5_Byte3_num; Frame6_Byte3 = 0b11000000 | Frame6_Byte3_num;
				Frame1_Byte4 = 0b00000001 | Frame1_Byte4_num; Frame2_Byte4 = 0b00000001 | Frame2_Byte4_num; Frame3_Byte4 = 0b00000001 | Frame3_Byte4_num; Frame4_Byte4 = 0b00000001 | Frame4_Byte4_num; Frame5_Byte4 = 0b00000001 | Frame5_Byte4_num; Frame6_Byte4 = 0b00000001 | Frame6_Byte4_num; LED_Set();
				} 
					else if (Hour_count == Hours_num && Minute_count == Minutes_num && Seconds_count >= Seconds_num){ //Minute Hand If statement that makes sure the Minute hand is visible even when the seconds light hasn't reached it's location yet
						Frame3_Byte1 = 255; Frame4_Byte1 = 255;
						Frame1_Byte2 = 0b00001100; Frame2_Byte2 = 0b00001110; Frame3_Byte2 = 0b11111111; Frame4_Byte2 = 0b11111111; Frame5_Byte2 = 0b00001110; Frame6_Byte2 = 0b00001100;
						
						Frame1_Byte3 = 0b11000000 | Frame1_Byte3_num; Frame2_Byte3 = 0b11100000 | Frame2_Byte3_num; Frame3_Byte3 = 0b11110000 | Frame3_Byte3_num; Frame4_Byte3 = 0b11110000 | Frame4_Byte3_num; Frame5_Byte3 = 0b11100000 | Frame5_Byte3_num; Frame6_Byte3 = 0b11000000 | Frame6_Byte3_num;
						Frame1_Byte4 = 0b00000000 | Frame1_Byte4_num; Frame2_Byte4 = 0b00000000 | Frame2_Byte4_num; Frame3_Byte4 = 0b00000000 | Frame3_Byte4_num; Frame4_Byte4 = 0b00000000 | Frame4_Byte4_num; Frame5_Byte4 = 0b00000000 | Frame5_Byte4_num; Frame6_Byte4 = 0b00000000 | Frame6_Byte4_num; LED_Set();
				
						} else if(Hour_count == Hours_num && Minute_count != Minutes_num && Seconds_count <= Seconds_num){ // If statement that will show the seconds light at this location when the minutes hand is not at this location
								Frame3_Byte1 = 255;  Frame4_Byte1 = 255;
								Frame1_Byte2 = 0b00001100; Frame2_Byte2 = 0b00001110; Frame3_Byte2 = 0b11111111; Frame4_Byte2 = 0b11111111; Frame5_Byte2 = 0b00001110; Frame6_Byte2 = 0b00001100;
							
									Frame1_Byte3 = Frame1_Byte3_num; Frame2_Byte3 = Frame2_Byte3_num; Frame3_Byte3 = Frame3_Byte3_num; Frame4_Byte3 = Frame4_Byte3_num; Frame5_Byte3 = Frame5_Byte3_num; Frame6_Byte3 = Frame6_Byte3_num;
									Frame1_Byte4 = 0b00000001 | Frame1_Byte4_num; Frame2_Byte4 = 0b00000001 | Frame2_Byte4_num; Frame3_Byte4 = 0b00000001 | Frame3_Byte4_num; Frame4_Byte4 = 0b00000001 | Frame4_Byte4_num; Frame5_Byte4 = 0b00000001 | Frame5_Byte4_num; Frame6_Byte4 = 0b00000001 | Frame6_Byte4_num; LED_Set();
							
							} else if(Hour_count == Hours_num && Minute_count != Minutes_num && Seconds_count >= Seconds_num){ //If statement that will show neither the minutes hand or the seconds lights at this location.
										Frame3_Byte1 = 255;  Frame4_Byte1 = 255;
										Frame1_Byte2 = 0b00001100; Frame2_Byte2 = 0b00001110; Frame3_Byte2 = 0b11111111; Frame4_Byte2 = 0b11111111; Frame5_Byte2 = 0b00001110; Frame6_Byte2 = 0b00001100;
			
										Frame1_Byte3 = Frame1_Byte3_num; Frame2_Byte3 = Frame2_Byte3_num; Frame3_Byte3 = Frame3_Byte3_num; Frame4_Byte3 = Frame4_Byte3_num; Frame5_Byte3 = Frame5_Byte3_num; Frame6_Byte3 = Frame6_Byte3_num;
										Frame1_Byte4 = Frame1_Byte4_num; Frame2_Byte4 = Frame2_Byte4_num; Frame3_Byte4 = Frame3_Byte4_num; Frame4_Byte4 = Frame4_Byte4_num; Frame5_Byte4 = Frame5_Byte4_num; Frame6_Byte4 = Frame6_Byte4_num; LED_Set();
			
								} else if (Hour_count != Hours_num && Minute_count == Minutes_num && Seconds_count <= Seconds_num){ //Minute Hand If statement that makes sure the Minute hand is visible when the seconds light has reached its location
											Frame3_Byte1 = 0b11111111; Frame4_Byte1 = 0b11111111;
											Frame1_Byte2 = 0b00000000; Frame2_Byte2 = 0b00000000; Frame3_Byte2 = 0b11111111; Frame4_Byte2 = 0b11111111; Frame5_Byte2 = 0b00000000; Frame6_Byte2 = 0b00000000;
			
											Frame1_Byte3 = 0b11000000 | Frame1_Byte3_num; Frame2_Byte3 = 0b11100000 | Frame2_Byte3_num; Frame3_Byte3 = 0b11110000 | Frame3_Byte3_num; Frame4_Byte3 = 0b11110000 | Frame4_Byte3_num; Frame5_Byte3 = 0b11100000 | Frame5_Byte3_num; Frame6_Byte3 = 0b11000000 | Frame6_Byte3_num;
											Frame1_Byte4 = 0b00000001 | Frame1_Byte4_num; Frame2_Byte4 = 0b00000001 | Frame2_Byte4_num; Frame3_Byte4 = 0b00000001 | Frame3_Byte4_num; Frame4_Byte4 = 0b00000001 | Frame4_Byte4_num; Frame5_Byte4 = 0b00000001 | Frame5_Byte4_num; Frame6_Byte4 = 0b00000001 | Frame6_Byte4_num; LED_Set();
									} else if (Hour_count != Hours_num && Minute_count == Minutes_num && Seconds_count >= Seconds_num){ //Minute Hand If statement that makes sure the Minute hand is visible even when the seconds light hasn't reached it's location yet
												Frame3_Byte1 = 0b11111111; Frame4_Byte1 = 0b11111111;
												Frame1_Byte2 = 0b00000000; Frame2_Byte2 = 0b00000000; Frame3_Byte2 = 0b11111111; Frame4_Byte2 = 0b11111111; Frame5_Byte2 = 0b00000000; Frame6_Byte2 = 0b00000000;
							
												Frame1_Byte3 = 0b11000000 | Frame1_Byte3_num; Frame2_Byte3 = 0b11100000 | Frame2_Byte3_num; Frame3_Byte3 = 0b11110000 | Frame3_Byte3_num; Frame4_Byte3 = 0b11110000 | Frame4_Byte3_num; Frame5_Byte3 = 0b11100000 | Frame5_Byte3_num; Frame6_Byte3 = 0b11000000 | Frame6_Byte3_num;
												Frame1_Byte4 = 0b00000000 | Frame1_Byte4_num; Frame2_Byte4 = 0b00000000 | Frame2_Byte4_num; Frame3_Byte4 = 0b00000000 | Frame3_Byte4_num; Frame4_Byte4 = 0b00000000 | Frame4_Byte4_num; Frame5_Byte4 = 0b00000000 | Frame5_Byte4_num; Frame6_Byte4 = 0b00000000 | Frame6_Byte4_num; LED_Set();
							
											} else if(Hour_count != Hours_num && Minute_count != Minutes_num && Seconds_count <= Seconds_num) { // If statement that will show the seconds light at this location when the minutes hand is not at this location
														Frame1_Byte3 = Frame1_Byte3_num; Frame2_Byte3 = Frame2_Byte3_num; Frame3_Byte3 = Frame3_Byte3_num; Frame4_Byte3 = Frame4_Byte3_num; Frame5_Byte3 = Frame5_Byte3_num; Frame6_Byte3 = Frame6_Byte3_num;
														Frame1_Byte4 = 0b00000001 | Frame1_Byte4_num; Frame2_Byte4 = 0b00000001 | Frame2_Byte4_num; Frame3_Byte4 = 0b00000001 | Frame3_Byte4_num; Frame4_Byte4 = 0b00000001 | Frame4_Byte4_num; Frame5_Byte4 = 0b00000001 | Frame5_Byte4_num; Frame6_Byte4 = 0b00000001 | Frame6_Byte4_num; LED_Set();
												} else { //If statement that will show neither the minutes hand or the seconds lights at this location.
														Frame1_Byte3 = Frame1_Byte3_num; Frame2_Byte3 = Frame2_Byte3_num; Frame3_Byte3 = Frame3_Byte3_num; Frame4_Byte3 = Frame4_Byte3_num; Frame5_Byte3 = Frame5_Byte3_num; Frame6_Byte3 = Frame6_Byte3_num;
														Frame1_Byte4 = Frame1_Byte4_num; Frame2_Byte4 = Frame2_Byte4_num; Frame3_Byte4 = Frame3_Byte4_num; Frame4_Byte4 = Frame4_Byte4_num; Frame5_Byte4 = Frame5_Byte4_num; Frame6_Byte4 = Frame6_Byte4_num; LED_Set();
														}
}


int main(void)
{
	MCUCSR = (1<<JTD);  //Disables JTAG so that PORTC can operate as an output or input
	MCUCSR = (1<<JTD);  //Disables JTAG so that PORTC can operate as an output or input
	
	Interrupt_initialize();
	SPI_MasterInit();
	PWM_16bit_initialize();
	
	while(ii < 8)
	{
		TransmitData(jj,0,0,0); 
		_delay_ms(50);
		jj = jj >> 1;
		++ii;
	}
	jj = 128;
	ii = 0;
	
	while(ii < 8)
	{
		TransmitData(0,jj,0,0);
		_delay_ms(50);
		jj = jj >> 1;
		++ii;
	}
	jj = 128;
	ii = 0;
	
	while(ii < 8)
	{
		TransmitData(0,0,jj,0);
		_delay_ms(50);
		jj = jj >> 1;
		++ii;
	}
	jj = 128;
	ii = 0;
	
	while(ii < 8)
	{
		TransmitData(0,0,0,jj);
		_delay_ms(50);
		jj = jj >> 1;
		++ii;
	}
	Start_Clock = 1;
	Timer_Counter = 0;
	Seconds_count = 59;
	while(1)
	{	
		if(start_animation == 1)
		{
		start_animation = 0;
		/**The full rotation of lights equal to 40ms.  This entire portion of code divides the time into 60 steps to represent 60 seconds and 60 minutes.
		Therefore, the 40ms was divided by 60 to 666us.  There is 60 instances of 666us to represent the 40ms full rotation.  In places where complex 
		animations like numbers need to be formed, the 666us were dived by 6 to have room to play with animation times.  This value became 111us. */
	
		LED_On(0,0,0,0,0,0,
			0,0,0,0,0,0,24,59,59);
													
		/**	This portion starts the animation for the Twelve on the clock.This begins the Seconds counter at the top of the clock.Because this is half way of the total 40ms, I started the counter 
		here so that the seconds animation would start at	the top of the clock as opposed to the bottom where the IR transmitter is located.
		*************************************************************************************************************************************************************************************************/
		LED_On(0b00001100,0b00001110,0b00001111,0b00001101,0b00001100,0b00001100,
						0b00001100,0b00001110,0b00000110,0b11000110,0b11111110,0b00111000,23,58,58);
		/**This portion Ends the animation for the Twelve on the clock.**********************************************************************************************************************************/
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,22,57,57);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,21,56,56);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,20,55,55);
		
		LED_On(0,0,0,0,0,0,0,0,254,254,0,0,19,54,54);//Notch one
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,18,53,53);

		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,17,52,52);

		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,16,51,51);

		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,15,50,50);
		
		LED_On(0,0,0,0,0,0,0,0,254,254,0,0,14,49,49);//Notch two
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,13,48,48);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,12,47,47);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,11,46,46);
					
		/**This portion Begins the animation for the Three on the clock.*********************************************************************************************************************************/
		LED_On(0,0,0,0,0,0,0,0,0b00111100,0b00111110,0b00000110,0b00000110,10,45,45);
	
		LED_On(0,0,0,0,0,0,0b00000110,0b00000110,0b00111000,0b00111000,0b00000110,0b00000110,9,44,44);
	
		LED_On(0,0,0,0,0,0,0b00000110,0b00000110,0b00111110,0b00111100,0,0,8,43,43);
		/**This portion Ends the animation for the Three on the clock.***********************************************************************************************************************************/	

		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,7,42,42);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,6,41,41);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,5,40,40);
		
		LED_On(0,0,0,0,0,0,0,0,254,254,0,0,4,39,39);//Notch three

		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,3,38,38);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,2,37,37);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,1,36,36);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,0,35,35);
		
		LED_On(0,0,0,0,0,0,0,0,254,254,0,0,59,34,34);//Notch four

		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,58,33,33);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,57,32,32);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,56,31,31);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,55,30,30);			
		
		/**This portion Starts the animation for the Six on the clock.*******************************************************************************************************************************/
		LED_On(0,0b00001100,0b00001100,0b00001100,0b00001100,0b00001100,
				0b00111000,0b01111100,0b01100110,0b01100110,0b01100110,0b01100110,54,29,29);
		
		LED_On(0b00000111, 0b00000011 ,0,0,0,0,0b11111100 ,0b11111100 ,0,0,0,0,53,28,28);
		/**This portion Finishes the animation for the Six on the clock.****************************************************************************************************************************/
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,52,27,27);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,51,26,26);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,50,25,25);
		
		LED_On(0,0,0,0,0,0,0,0,254,254,0,0,49,24,24);//Notch five
	
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,48,23,23);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,47,22,22);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,46,21,21);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,45,20,20);
		
		LED_On(0,0,0,0,0,0,0,0,254,254,0,0,44,19,19);//Notch six

		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,43,18,18);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,42,17,17);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,41,16,16);
		
		/**This portion starts the animation for the Nine on the clock.**********************************************************************************************************************************/	
		LED_On(0,0,0,0,0,0,0,0,0b00111100,0b01111100,0b11000000,0b11000000,40,15,15);
			
		LED_On(0,0,0,0,0,0,0b11000000,0b11000000,0b11111100,0b11111110,0b11000110,0b11000110,39,14,14);	
		
		LED_On(0,0,0,0,0,0,0b11000110,0b11000110,0b11111100,0b00111000,0,0,38,13,13);
		/**This portion Ends the animation for the Nine on the clock.************************************************************************************************************************************/

		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,37,12,12);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,36,11,11);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,35,10,10);
		
		LED_On(0,0,0,0,0,0,0,0,254,254,0,0,34,9,9);// Notch sevin

		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,33,8,8);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,32,7,7);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,31,6,6);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,30,5,5);
		
		LED_On(0,0,0,0,0,0,0,0,254,254,0,0,29,4,4);//Notch eight
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,28,3,3);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,27,2,2);
		
		LED_On(0,0,0,0,0,0,0,0,0,0,0,0,26,1,1);
		
		/**	This portion Finishes the animation for the Twelve on the clock.
		*************************************************************************************************************************************************************************************************/
		LED_On(0,0b00001100,0b00001111,0b00001111,0b00001111,0b00001100,
				0,0b00000100,0b11111100,0b11111110,0b11111110,0,25,0,0);
		/**This portion Ends the animation for the Twelve on the clock.**********************************************************************************************************************************/
		TransmitData(128,0,0,1);
		}
	}
	
}

