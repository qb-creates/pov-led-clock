#ifndef SPIUTILITY_H
#define SPIUTILITY_H

#include <stdbool.h>

void spiMasterModeInit(void);
void transmitData(char data, bool msb);
void latchData(void);

#endif