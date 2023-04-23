#include "spiUtility.h"

/**
 * @brief Configures the spi peripheral in master mode.
 * 
 */
void configureSPIMasterMode()
{
  /* Set MOSI, SCK, and Latch as output.*/
  DDRB |= _BV(PB4) | _BV(PB5) | _BV(PB7);

  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR |= _BV(SPE) | _BV(MSTR) | _BV(CPOL);
}

/**
 * @brief Will transmit a single byte of data.
 *
 * @param data The byte that will be transmitted.
 */
void transmitData(char data)
{
    /* Start transmission */
    SPDR = data;

    /* Wait for transmission complete */
    while (!(SPSR & _BV(SPIF)))
    {
    }
}

/**
 * @brief Transmit an array of bytes.
 *
 * @param data The array of bytes that should be transmitted.
 * @param count The number of bytes in the array.
 */
void transmitChars(char data[], int count)
{
    for (int i = 0; i < count; i++)
    {
        /* Start transmission */
        SPDR = data[i];

        /* Wait for transmission complete */
        while (!(SPSR & _BV(SPIF)))
        {
        }
    }
}