/*
 * pov-clock-atmega32
 *
 * Author: Quentin Baker
 *
 * Description:
 *   This file contains methods that will project the clock image and add seconds to the clock.
 *   The clock is rotating at about 1500 rpm. One rotation takes about 40ms (± 1.6ms). The time 
 *   for one rotation will be divided by 60. This will give us 60 steps of 666.67us. Each section
 *   of the clock image will be projected during that 666.67us timeframe.  The 666us will be 
 *   divided by 6. A section of the clock image will be projectected every 111.11us. For example, 
 *   the number 3 on the clock will be projected during the 666us window. Each slice of the number
 *   3 will be projected every 111.11us.
 *
 *   The drawClock function was measured to take about 12ms to complete. This is not including the 
 *   _delay_us() calls in the method. Using a delay of 81us for the _delay_us() calls gives us a total 
 *   delay time of (81us * 6  * 60 steps = 29.16ms). 29.16ms summed with the 12ms it takes to execute 
 *   the other lines of code in the drawClock function givs us a total rotation time of 41.16ms.
 */

#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include "povClock.h"
#include "imageData.h"
#include "spiUtility.h"

uint8_t secondsOutlinePosition = 0;
uint8_t minuteHandPosition = 0;
uint8_t hourHandPosition = 25;

/**
 * @brief Initializes the clock. Call this method in order to enable spi communication
 * with the LED drivers.
 */
void initializeClock(void)
{
    spiMasterModeInit();
}

/**
 * @brief Will add one second to the clock. This function will update the seconds outline, minute hand, and hour hand positions
 * on the projected clock image. 
 */
void addSecondToClock(void)
{
    // Move the seconds outline to the next position on the clock.
    ++secondsOutlinePosition;

    if (secondsOutlinePosition == 60)
    {
        // Move the minute hand to the next position on the clock.
        ++minuteHandPosition;
        
        if (minuteHandPosition % 12 == 0)
        {
            // Gradually move the hour hand to the next hour marker as the minute hand gets closer to making a complete trip around the clock.
            // Will move the hour hand when the minute hand is at positions 12, 24, 36, 48, and 0.
            ++hourHandPosition;
        }

        if (minuteHandPosition == 59)
        {
            minuteHandPosition = 0;
        }
        
        if (hourHandPosition == 59)
        {
            hourHandPosition = 0;
        }

        secondsOutlinePosition = 0;
    }
}

/**
 * @brief Project the image of the clock.
 */
void projectClockImage(void)
{
    // Go through each row of the clock image data array and project the image data in that row
    for (int row = 0; row < 60; row++)
    {
        for (int i = 0; i < 6; i++)
        {
            // Store our clock image data
            uint8_t byte0 = clockImageData[row][i];
            uint8_t byte1 = clockImageData[row][i + 6];
            uint8_t byte2 = clockImageData[row][i + 12];
            uint8_t byte3 = clockImageData[row][i + 18];

            // Include the seconds outline image data if the seconds position is >= to the row position we are projecting.
            if (secondsOutlinePosition >= row)
            {
                byte3 |= secondsOutlineImageData[i][3];
            }

            // Include the Minute hand image data if the minute hand position is equal to the row position we are projecting.
            if (minuteHandPosition == row)
            {
                byte0 |= minuteHandImageData[i][0];
                byte1 |= minuteHandImageData[i][1];
                byte2 |= minuteHandImageData[i][2];
            }

            // Include the Hour hand image data if the hour hand position is equal to the row position we are projecting.
            if (hourHandPosition == row)
            {
                byte0 |= hourHandImageData[i][0];
                byte1 |= hourHandImageData[i][1];
            }

            // Transmit the data to the LED drivers
            transmitData(byte3, true);
            transmitData(byte2, false);
            transmitData(byte1, true);
            transmitData(byte0, false);
            latchData();
            _delay_us(81); 
        }
    }
}