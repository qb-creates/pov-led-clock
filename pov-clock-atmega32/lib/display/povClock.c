/*
 * pov-clock-atmega32
 *
 * Author: Quentin Baker
 *
 * Description:
 *   This file contains methods that will project the clock image and add seconds to the clock.
 *   The clock is rotating at about 1500 rpm. One rotation takes about 40ms (± 1.6ms). The time for one rotation will be divided by 60.
 *   This will give us 60 steps of 666.67us. Each section of the clock image will be projected during that 666.67us timeframe.
 *   In places where complex animations like numbers need to be drawn, the 666us will be divided by 6 to have room to play with animation times.
 *   This value became 111.111us.
 * 
 *   The drawClock function was measured to take about 12ms to complete. This is not including the _delay_us() calls in the method.
 *   Using a delay of 81us for the _delay_us() calls gives us a total delay time of (81us * 6  * 60 steps = 29.16ms).
 *   29.16ms summed with the 12ms it takes to execute the other portions of code in the drawClock function givs us a total time of 41.16ms.
 *
 */

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "povClock.h"
#include "pixelData.h"
#include "spiUtility.h"

uint8_t secondHandPosition = 0;
uint8_t minutesHandPosition = 0;
uint8_t hourHandPosition = 25;

/**
 * @brief Initializes the clock. Enables spi transmission.
 *
 */
void initializeClock(void)
{
    spiMasterModeInit();
    transmitData(0x80, false);
    latchData();
}

/**
 * @brief Will update the second, minute, and hour hand positions. Will add one to the
 * seconds hand. When seconds hand is at 60, the minutes hand will update. 
 */
void addSecondToClock(void)
{
    ++secondHandPosition;

    if (secondHandPosition == 60)
    {
        ++minutesHandPosition;
        secondHandPosition = 0;

        if (minutesHandPosition % 12 == 0)
        {
            ++hourHandPosition;
        }

        if (minutesHandPosition == 59)
        {
            minutesHandPosition = 0;
        }

        if (hourHandPosition == 59)
        {
            hourHandPosition = 0;
        }
    }
}

/**
 * @brief
 */
void projectClockImage(void)
{
    for (int row = 0; row < 60; row++)
    {
        // Reset our PixelData.
        struct PixelData pixelData[6] = {{0x80, 0, 0, 0}};

        // pixelData Byte3 will hold our Hour Markers Byte 1
        pixelData[0].byte3 = clockImageData[row][0];
        pixelData[1].byte3 = clockImageData[row][1];
        pixelData[2].byte3 = clockImageData[row][2];
        pixelData[3].byte3 = clockImageData[row][3];
        pixelData[4].byte3 = clockImageData[row][4];
        pixelData[5].byte3 = clockImageData[row][5];

        // pixelData Byte4 will hold our Hour Markers Byte 2
        pixelData[0].byte4 = clockImageData[row][6];
        pixelData[1].byte4 = clockImageData[row][7];
        pixelData[2].byte4 = clockImageData[row][8];
        pixelData[3].byte4 = clockImageData[row][9];
        pixelData[4].byte4 = clockImageData[row][10];
        pixelData[5].byte4 = clockImageData[row][11];

        if (hourHandPosition == row)
        {
            // Include the Hour hand pixel data if the hour hand position is equal to the row we are drawing.
            pixelData[2].byte1 |= 255;
            pixelData[3].byte1 |= 255;

            pixelData[0].byte2 |= 0b00001100;
            pixelData[1].byte2 |= 0b00001110;
            pixelData[2].byte2 |= 0b11111111;
            pixelData[3].byte2 |= 0b11111111;
            pixelData[4].byte2 |= 0b00001110;
            pixelData[5].byte2 |= 0b00001100;
        }

        if (minutesHandPosition == row)
        {
            // Include the Minute hand pixel data if the minute hand position is equal to the row we are drawing.
            pixelData[2].byte1 |= 255;
            pixelData[3].byte1 |= 255;
            pixelData[2].byte2 |= 0b11111111;
            pixelData[3].byte2 |= 0b11111111;

            pixelData[0].byte3 |= 0b11000000;
            pixelData[1].byte3 |= 0b11100000;
            pixelData[2].byte3 |= 0b11110000;
            pixelData[3].byte3 |= 0b11110000;
            pixelData[4].byte3 |= 0b11100000;
            pixelData[5].byte3 |= 0b11000000;
        }

        if (secondHandPosition >= row)
        {
            // Include the seconds outline pixel data if the seconds outline position is >= to the row we are drawing.
            pixelData[0].byte4 |= 0b00000001;
            pixelData[1].byte4 |= 0b00000001;
            pixelData[2].byte4 |= 0b00000001;
            pixelData[3].byte4 |= 0b00000001;
            pixelData[4].byte4 |= 0b00000001;
            pixelData[5].byte4 |= 0b00000001;
        }

        // We transmit the data stored the pixelData struct. Because the two led driver IC's are cascaded together,
        // we transmit byte4 first and work our way down to byte 1. This is done 6 times.
        for (int i = 0; i < 6; i++)
        {
            transmitData(pixelData[i].byte4, true);
            transmitData(pixelData[i].byte3, false);
            transmitData(pixelData[i].byte2, true);
            transmitData(pixelData[i].byte1, false);
            latchData();
            _delay_us(81);
        }
    }
}