#include "povClock.h"

void LED_ON(const uint8_t Frame1_Byte3_num[]);

struct Frame
{
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;
    uint8_t byte4;
} frames[6];

const uint8_t emptySpaceBytes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const uint8_t hourLineBytes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0xFE, 0xFE, 0, 0};
const uint8_t hourThreeBytes[3][12] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0b00111100, 0b00111110, 0b00000110, 0b00000110},
    {0, 0, 0, 0, 0, 0, 0b00000110, 0b00000110, 0b00111000, 0b00111000, 0b00000110, 0b00000110},
    {0, 0, 0, 0, 0, 0, 0b00000110, 0b00000110, 0b00111110, 0b00111100, 0, 0}};

const uint8_t hourSixBytes[2][12] = {
    {0, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00111000, 0b01111100, 0b01100110, 0b01100110, 0b01100110, 0b01100110},
    {0b00000111, 0b00000011, 0, 0, 0, 0, 0b11111100, 0b11111100, 0, 0, 0, 0}};

const uint8_t hourNineBytes[3][12] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0b00111100, 0b01111100, 0b11000000, 0b11000000},
    {0, 0, 0, 0, 0, 0, 0b11000000, 0b11000000, 0b11111100, 0b11111110, 0b11000110, 0b11000110},
    {0, 0, 0, 0, 0, 0, 0b11000110, 0b11000110, 0b11111100, 0b00111000, 0, 0}};

const uint8_t hourTwelveBytes[3][12] = {
    {0b00001100, 0b00001110, 0b00001111, 0b00001101, 0b00001100, 0b00001100, 0b00001100, 0b00001110, 0b00000110, 0b11000110, 0b11111110, 0b00111000},
    {0, 0b00001100, 0b00001111, 0b00001111, 0b00001111, 0b00001100, 0, 0b00000100, 0b11111100, 0b11111110, 0b11111110, 0}};

uint8_t secondHandPosition = 59;
uint8_t minutesHandPosition = 59;
uint8_t hourHandPosition = 24;

uint8_t Hours_num = 59;

/**
 * @brief Initializes the clock. Enables spi transmission.
 * 
 */
void initializeClock()
{
    spiMasterModeInit();

    for (int i = 1; i < 8; i++)
    {
        char data[] = {i % 2, 0, 0, i % 2};

        transmitChars(data, 4);
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
    if (secondHandPosition < 0)
    {
        --minutesHandPosition;
        secondHandPosition = 59;

        if (minutesHandPosition % 12 == 0)
        {
            hourHandPosition--;
        }

        if (minutesHandPosition < 0)
        {
            minutesHandPosition = 59;
        }

        if (hourHandPosition < 0)
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
    Hours_num = 59;
    LED_On(emptySpaceBytes);
    LED_On(hourTwelveBytes[0]);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(hourLineBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(hourLineBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(hourThreeBytes[0]);
    LED_On(hourThreeBytes[1]);
    LED_On(hourThreeBytes[2]);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(hourLineBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(hourLineBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(hourSixBytes[0]);
    LED_On(hourSixBytes[1]);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(hourLineBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(hourLineBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(hourNineBytes[0]);
    LED_On(hourNineBytes[1]);
    LED_On(hourNineBytes[2]);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(hourLineBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(hourLineBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(emptySpaceBytes);
    LED_On(hourTwelveBytes[1]);
    char data[4] = {128, 0, 0, 128};
    transmitChars(data, 4);
    latchData();
}

void LED_Set()
{
    /**This is where the instances of 666us are separated by six for animation design.
     each additional frame adds on to how long the lights in that particular byte stay on
     or if they are on at*/
    for (int i = 0; i < 6; i++)
    {
        transmitData(frames[i].byte4, true);
        transmitData(frames[i].byte3, false);
        transmitData(frames[i].byte2, true);
        transmitData(frames[i].byte1, false);
        latchData();
        _delay_us(76.5);
    }
}

void LED_On(const uint8_t Frame1_Byte3_num[])
{
    for (int i = 0; i < 6; i++)
    {
        frames[i].byte1 = 0x00;
        frames[i].byte2 = 0x00;
        frames[i].byte3 = 0x00;
        frames[i].byte4 = 0x00;
    }

    frames[0].byte1 = 0x80;
    frames[1].byte1 = 0x80;
    frames[2].byte1 = 0x80;
    frames[3].byte1 = 0x80;
    frames[4].byte1 = 0x80;
    frames[5].byte1 = 0x80;

    frames[0].byte3 = Frame1_Byte3_num[0];
    frames[1].byte3 = Frame1_Byte3_num[1];
    frames[2].byte3 = Frame1_Byte3_num[2];
    frames[3].byte3 = Frame1_Byte3_num[3];
    frames[4].byte3 = Frame1_Byte3_num[4];
    frames[5].byte3 = Frame1_Byte3_num[5];

    frames[0].byte4 = Frame1_Byte3_num[6];
    frames[1].byte4 = Frame1_Byte3_num[7];
    frames[2].byte4 = Frame1_Byte3_num[8];
    frames[3].byte4 = Frame1_Byte3_num[9];
    frames[4].byte4 = Frame1_Byte3_num[10];
    frames[5].byte4 = Frame1_Byte3_num[11];

    if (hourHandPosition == Hours_num)
    {
        frames[2].byte1 |= 255;
        frames[3].byte1 |= 255;

        frames[0].byte2 |= 0b00001100;
        frames[1].byte2 |= 0b00001110;
        frames[2].byte2 |= 0b11111111;
        frames[3].byte2 |= 0b11111111;
        frames[4].byte2 |= 0b00001110;
        frames[5].byte2 |= 0b00001100;
    }
    if (minutesHandPosition == Hours_num)
    { 
        frames[2].byte1 |= 255;
        frames[3].byte1 |= 255;
        frames[2].byte2 |= 0b11111111;
        frames[3].byte2 |= 0b11111111;

        frames[0].byte3 |= 0b11000000;
        frames[1].byte3 |= 0b11100000;
        frames[2].byte3 |= 0b11110000;
        frames[3].byte3 |= 0b11110000;
        frames[4].byte3 |= 0b11100000;
        frames[5].byte3 |= 0b11000000;
    }

    if (secondHandPosition <= Hours_num)
    { 
        frames[0].byte4 |= 0b00000001;
        frames[1].byte4 |= 0b00000001;
        frames[2].byte4 |= 0b00000001;
        frames[3].byte4 |= 0b00000001;
        frames[4].byte4 |= 0b00000001;
        frames[5].byte4 |= 0b00000001;
    }

    LED_Set();
    Hours_num--;
}