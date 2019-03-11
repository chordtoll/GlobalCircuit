/*
 * File:   SPI.h
 * Authors: Lauren Turnbow & Ryan Price
 *
 * Created on August 16, 2018, 7:30 PM
 */
#include <proc/p32mx360f512l.h>
#include "SPI.h"

uint16_t adc_samples[6];


//Initalize SPI1
void InitSPI1(void)
{
    //SCK is output RF6
    //SDI is input RF7
    //SDO is output RF8
    //CHIP SELECT IS O/P output RB2

    //Set CS high (idle state)
    //CS1 = 1;

    TRISFbits.TRISF6 = 0; // SCK1 is output RD10 (RF6)
    TRISFbits.TRISF7 = 1;  // SDI1 is input RC04 (RF7)
    TRISFbits.TRISF8 = 0;  // SDO1 is output RD00 (RF8)
    TRISBbits.TRISB2 = 0;  // CHIP SELECT1 IS O/P output RD09 (RB2)

    //SPI interrupts disabled
    IEC0bits.SPI1EIE = 0;

    //Disable receive & transmit
    IEC0bits.SPI1RXIE = 0;
    IEC0bits.SPI1TXIE = 0;

    //Turn off SPI module
    SPI1CONbits.ON = 0;

    //Clear the receive buffer
    SPI1BUF = 0;

    //FSCK = 10MHz
    //SPI1BRG = 3;
    SPI1BRG = 19;
    
    //Clear overflow flag
    SPI1STATbits.SPIROV = 0;

    /* SPI1CON settings */

    //Framed SPI support is disabled
    SPI1CONbits.FRMEN = 0;

    //Continue operation in IDLE mode
    SPI1CONbits.SIDL = 0;

    //SDO1 pin is controlled by the module
    SPI1CONbits.DISSDO = 0;

    //32 bit mode
    SPI1CONbits.MODE16 = 0;
    SPI1CONbits.MODE32 = 1;

    //Idle state for clock is high, active state is low
    SPI1CONbits.CKP = 0;

    //Output data changes on transition from idle to active
    SPI1CONbits.CKE = 0;

    //Not in slave mode
    SPI1CONbits.SSEN = 0;

    //Master mode
    SPI1CONbits.MSTEN = 1;

    //Input data sampled at the end of data output time //Comment did not match implementation, fixed
    SPI1CONbits.SMP = 1;

    //Turn module on
    SPI1CONbits.ON = 1;
}

//Write data to SPI, return buffer data
double WriteReadSPI1(unsigned short data)
{
    //Set the chip select low
    //CS1 = 0;

    //Write to buffer for transmission
    SPI1BUF = data;

    //Wait for transfer to be completed
    while (!SPI1STATbits.SPIRBF);

    //Set the chip select back high
    //CS1 = 1;

    //Return the received value
    return SPI1BUF;
}

//
unsigned int ReadSPI1()
{
    //Check for Receive buffer full status bit of status register
    if (SPI1STATbits.SPIRBF)
    {
        //Clear overflow flag bit. Bit is set high if a transmission was received and not read
        SPI1STATbits.SPIROV = 0;

        //Choose size of data to read
        if (SPI1CONbits.MODE32)
        {
            return ( SPI1BUF ); //Return word read
        }
        else
        {
            return (SPI1BUF & 0xff); //return byte read
        }
    }

    //RBF bit is not set return error*/
    return -1;
}

void WriteSPI1(unsigned int data_out)
{
   //Write word
   if (SPI1CONbits.MODE32)
   {
        SPI1BUF = data_out;
   }
   //Write byte
    else
    {
        SPI1BUF = data_out & 0xff; // byte write
    }
}

void InitSPI2(void)
{
    // SCK is output RG6
    // SDI is input RG7
    // SDO is output RG8
    // CHIP SELECT IS O/P output RG9

    TRISGbits.TRISG6 = 0; // SCK2 is output RG6
    TRISGbits.TRISG7 = 1;  // SDI1 is input RG7
    TRISGbits.TRISG8 = 0;  // SDO1 is output RG8
    TRISGbits.TRISG9 = 0;  // CHIP SELECT1 IS O/P output RG2

    //Set CS high (idle state)
    CS2 = 1;

    // SPI interrupts disabled
    IEC1bits.SPI2EIE = 0;
    IEC1bits.SPI2RXIE = 0;
    IEC1bits.SPI2TXIE = 0;

    //Turn off SPI module
    SPI2CONbits.ON = 0;

    //Clear the receive buffer
    SPI2BUF = 0;

    //FSCK = 10MHz
    SPI2BRG = 3;

    //Clear overflow flag
    SPI2STATbits.SPIROV = 0;

    /* SPI1CON settings */

    //Framed SPI support is disabled
    SPI2CONbits.FRMEN = 0;

    //Continue operation in IDLE mode
    SPI2CONbits.SIDL = 0;

    //SDO2 pin is controlled by the module
    SPI2CONbits.DISSDO = 0;

    //32 bit mode
    SPI2CONbits.MODE16 = 0;
    SPI2CONbits.MODE32 = 1;

    //Idle state for clock is high, active state is low
    SPI2CONbits.CKP = 0;

    //Output data changes on transition from idle to active
    SPI2CONbits.CKE = 0;

     //Not in slave mode
    SPI2CONbits.SSEN = 0;

    //Master mode
    SPI2CONbits.MSTEN = 1;

    //Input data sampled at the end of data output time
    SPI2CONbits.SMP = 0;

    //Turn module on
    SPI2CONbits.ON = 1;
}

double WriteReadSPI2(unsigned short i)
{
    //Set the chip select low
    //CS2 = 0;

    //Write to buffer for transmission
    SPI2BUF = i;

    //Wait for transfer to be completed
    while (!SPI2STATbits.SPIRBF);

    //Set the chip select back high
    //CS2 = 1;

    //Return the received value
    return SPI2BUF;
}

unsigned int ReadSPI2()
{
    //Check for Receive buffer full status bit of status register
    if (SPI2STATbits.SPIRBF)
    {
        //Clear overflow flag bit. Bit is set high if a transmission was received and not read
        SPI2STATbits.SPIROV = 0;

        //Choose size of data to read
        if (SPI2CONbits.MODE32)
        {
            return ( SPI2BUF ); //Return word read
        }
        else
        {
            return (SPI2BUF & 0xff); //return byte read
        }
    }

    //RBF bit is not set return error*/
    return -1;
}

void WriteSPI2(unsigned int data_out)
{
   //Write word
   if (SPI2CONbits.MODE32)
   {
        SPI2BUF = data_out;
   }
   //Write byte
   else
   {
       SPI2BUF = data_out & 0xff; // byte write
   }
}

void Delay ( void )
{
    int temp;
    for( temp = 0; temp < 10000; temp++ );
}


