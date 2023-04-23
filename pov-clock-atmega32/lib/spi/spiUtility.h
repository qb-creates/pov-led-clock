#ifndef SPIUTILITY_H
#define SPIUTILITY_H
#include <avr/io.h>

void configureSPIMasterMode();
void transmitData(char data);
void transmitChars(char data[], int count);

#endif