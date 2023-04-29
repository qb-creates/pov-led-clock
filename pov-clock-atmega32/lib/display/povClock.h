#ifndef POVCLOCK_H
#define POVCLOCK_H
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "spiUtility.h"

void initializeClock();
void drawClock(void);
void updateHandPositions(void);

#endif