#ifndef SPIUTILITY_H
#define SPIUTILITY_H
#include <avr/io.h>
#include <stdbool.h>

void spiMasterModeInit(void);
void transmitData(char data, bool msb);
void transmitChars(char data[], int count);
void latchData(void);

#endif