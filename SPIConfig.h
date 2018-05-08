/*******************************************************************************
 *
 * Functions to initialize SPI
 *
 ******************************************************************************/
#ifndef _SPIConfig_H_
#define	_SPIConfig_H_

// Chip select for SPI will need to be changed for pic32mx360
#define CS LATDbits.LATD9
void initSPI(void)
{
    //CS = 1;                     // Set CS high (idle state)
    IEC0bits.SPI1EIE = 0;       // SPI interrupts disabled
    IEC0bits.SPI1RXIE = 0;
    IEC0bits.SPI1TXIE = 0;
    SPI1CONbits.ON = 0;         // Turn off SPI module
    SPI1BUF = 0;                // Clear the receive buffer
    SPI1BRG = 3;                // FSCK = 2.5MHz
    SPI1STATbits.SPIROV = 0;    // Clear overflow flag


    /* SPI1CON settings */
    SPI1CONbits.FRMEN = 0;      // Framed SPI support is disabled
    SPI1CONbits.SIDL = 0;       // Continue operation in IDLE mode
    SPI1CONbits.DISSDO = 0;     // SDO1 pin is controlled by the module
    SPI1CONbits.MODE16 = 0;     // 16 bit mode
    SPI1CONbits.MODE32 = 1;
    SPI1CONbits.CKP = 0;        // Idle state for clock is high, active state is low
    SPI1CONbits.CKE = 0;        // Output data changes on transition from idle to active
    SPI1CONbits.SSEN = 0;       // Not in slave mode
    SPI1CONbits.MSTEN = 1;      // Master mode
    SPI1CONbits.SMP = 0;        // Input data sampled at the end of data output time
    SPI1CONbits.ON = 1;         // Turn module on
}

double WriteReadSPI(unsigned short i)
{
    //CS = 0;                         // Set the chip select low
    SPI1BUF = i;                    // Write to buffer for transmission
    while (!SPI1STATbits.SPIRBF);   // Wait for transfer to be completed
    //CS = 1;                         // Set the chip select back high
    return SPI1BUF;                 // Return the received value
}

unsigned int ReadSPI()
{
    /* Check for Receive buffer full status bit of status register*/
    if (SPI1STATbits.SPIRBF)
    {
        SPI1STATbits.SPIROV = 0;

        if (SPI1CONbits.MODE32)
            return ( SPI1BUF ); /* return word read */
        else
            return (SPI1BUF & 0xff); /* return byte read */
    }
    return -1; /* RBF bit is not set return error*/
}

void WriteSPI(unsigned int data_out)
{

   if (SPI1CONbits.MODE32) // word write
        SPI1BUF = data_out;
    else
        SPI1BUF = data_out & 0xff; // byte write

}

void Delay ( void )
{
    int temp;
    for( temp = 0; temp < 10000; temp++ );
}

#endif