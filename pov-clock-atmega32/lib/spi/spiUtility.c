#include "spiUtility.h"

/**
 * @brief Initializes the spi peripheral in master mode.
 *
 */
void spiMasterModeInit(void)
{
    /* Set MOSI, SCK, and Latch as output.*/
    DDRB |= _BV(PB4) | _BV(PB5) | _BV(PB7);

    /* Enable SPI, Master, set clock frequency fck/4 */
    SPCR |= _BV(SPE) | _BV(MSTR) | _BV(CPOL);
}

/**
 * @brief Will transmit a single byte of data.
 *
 * @param data The byte that will be transmitted.
 * @param msb Transmit the most significant bit first. true/false
 */
void transmitData(char data, bool msb)
{
    if (msb)
    {
        SPCR &= ~_BV(DORD);
    }
    else
    {
        SPCR |= _BV(DORD);
    }

    /* Start transmission */
    SPDR = data;

    /* Wait for transmission complete */
    loop_until_bit_is_set(SPSR, SPIF);
}

/**
 * @brief Will latch the data to the led drivers.
 *
 */
void latchData(void)
{
    PORTB |= _BV(PB4);
    PORTB &= ~(_BV(PB4));
}