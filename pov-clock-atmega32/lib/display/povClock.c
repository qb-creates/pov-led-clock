/*
 * pov-clock-atmega32
 *
 * Author: Quentin Baker
 *
 * The drawClock function was measured to take about 12ms to complete not including the _delay_us() calls in the method.
 * using a delay of 80us gives us a total delay time of (80us * 6  * 60 steps = 28.8ms). 28.8ms summed with the 12ms it takes
 * to execute the other portions of code in the function givs us 40.8ms. That is the time it takes to complete one rotation.
 * 
 * Using the ATmega32's internal 8Mhz oscillator.
 */

#include "povClock.h"
#include "pixelData.h"

void updatePixels(const uint8_t hourNumberPixelData[][12]);

uint8_t secondHandPosition = 60;
uint8_t minutesHandPosition = 60;
uint8_t hourHandPosition = 25;

/**
 * @brief Initializes the clock. Enables spi transmission.s
 *
 */
void initializeClock()
{
    spiMasterModeInit();

    for (int i = 1; i < 8; i++)
    {
        char data[] = {i % 2, 0, 0, i % 2};

        transmitDataArray(data, 4, true);
        latchData();
        _delay_ms(200);
    }
}

/**
 * @brief Will update the second, minute, and hour hand positions. Will add one to the
 * seconds hand. When seconds hand is at 60, the minutes hnad will update.
 */
void updateHandPositions(void)
{
    --secondHandPosition;
    if (secondHandPosition == 0)
    {
        --minutesHandPosition;
        secondHandPosition = 60;

        if (minutesHandPosition % 12 == 0)
        {
            hourHandPosition--;
        }

        if (minutesHandPosition == 0)
        {
            minutesHandPosition = 59;
        }

        if (hourHandPosition == 0)
        {
            hourHandPosition = 59;
        }
    }
}

/**
 * @brief 
 */
void drawClock(void)
{
    int positionCheck = 60;

    for (int row = 0; row < 60; row++)
    {
        for (int i = 0; i < 6; i++)
        {
            clockPixelData[i].clockHandByte2 = 0x80;
            clockPixelData[i].clockHandByte1 = 0x00;
        }

        clockPixelData[0].hourNumberByte2 = asdfff[row][0];
        clockPixelData[1].hourNumberByte2 = asdfff[row][1];
        clockPixelData[2].hourNumberByte2 = asdfff[row][2];
        clockPixelData[3].hourNumberByte2 = asdfff[row][3];
        clockPixelData[4].hourNumberByte2 = asdfff[row][4];
        clockPixelData[5].hourNumberByte2 = asdfff[row][5];

        clockPixelData[0].hourNumberByte1 = asdfff[row][6];
        clockPixelData[1].hourNumberByte1 = asdfff[row][7];
        clockPixelData[2].hourNumberByte1 = asdfff[row][8];
        clockPixelData[3].hourNumberByte1 = asdfff[row][9];
        clockPixelData[4].hourNumberByte1 = asdfff[row][10];
        clockPixelData[5].hourNumberByte1 = asdfff[row][11];

        if (hourHandPosition == positionCheck)
        {
            clockPixelData[2].clockHandByte2 |= 255;
            clockPixelData[3].clockHandByte2 |= 255;

            clockPixelData[0].clockHandByte1 |= 0b00001100;
            clockPixelData[1].clockHandByte1 |= 0b00001110;
            clockPixelData[2].clockHandByte1 |= 0b11111111;
            clockPixelData[3].clockHandByte1 |= 0b11111111;
            clockPixelData[4].clockHandByte1 |= 0b00001110;
            clockPixelData[5].clockHandByte1 |= 0b00001100;
        }

        if (minutesHandPosition == positionCheck)
        {
            clockPixelData[2].clockHandByte2 |= 255;
            clockPixelData[3].clockHandByte2 |= 255;
            clockPixelData[2].clockHandByte1 |= 0b11111111;
            clockPixelData[3].clockHandByte1 |= 0b11111111;

            clockPixelData[0].hourNumberByte2 |= 0b11000000;
            clockPixelData[1].hourNumberByte2 |= 0b11100000;
            clockPixelData[2].hourNumberByte2 |= 0b11110000;
            clockPixelData[3].hourNumberByte2 |= 0b11110000;
            clockPixelData[4].hourNumberByte2 |= 0b11100000;
            clockPixelData[5].hourNumberByte2 |= 0b11000000;
        }

        if (secondHandPosition <= positionCheck)
        {
            // Adds the seconds outline to this hour number.
            clockPixelData[0].hourNumberByte1 |= 0b00000001;
            clockPixelData[1].hourNumberByte1 |= 0b00000001;
            clockPixelData[2].hourNumberByte1 |= 0b00000001;
            clockPixelData[3].hourNumberByte1 |= 0b00000001;
            clockPixelData[4].hourNumberByte1 |= 0b00000001;
            clockPixelData[5].hourNumberByte1 |= 0b00000001;
        }

        /**This is where the instances of 666us are separated by six for animation design.
        each additional frame adds on to how long the lights in that particular byte stay on
        or if they are on at*/
        for (int i = 0; i < 6; i++)
        {
            transmitData(clockPixelData[i].hourNumberByte1, true);
            transmitData(clockPixelData[i].hourNumberByte2, false);
            transmitData(clockPixelData[i].clockHandByte1, true);
            transmitData(clockPixelData[i].clockHandByte2, false);
            latchData();
            _delay_us(83);
        }

        positionCheck--;
    }

    // char data[4] = {128, 0, 0, 128};
    // transmitDataArray(data, 4, false);
    // latchData();
}