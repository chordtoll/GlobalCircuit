/*
 * File:   SPI.h
 * Authors: Lauren Turnbow & Ryan Price
 *
 * Created on August 16, 2018, 7:30 PM
 */
#include <proc/p32mx360f512l.h>
#include "SPI.h"
#include "ADC.h"

//Initalize SPI1
void InitSPI1(void)
{
    //SCK is output RF6
    //SDI is input RF7
    //SDO is output RF8

    TRISFbits.TRISF6 = 0; // SCK1 is output RD10 (RF6)
    TRISFbits.TRISF7 = 1;  // SDI1 is input RC04 (RF7)
    TRISFbits.TRISF8 = 0;  // SDO1 is output RD00 (RF8)
    TRISBbits.TRISB2 = 0;  // CHIP SELECT1 IS O/P output RD09 (RB2)

    TRISGbits.TRISG9 = 0;  // CHIP SELECT2 IS O/P output RD09 (RG9)

    CS1 = 1;
    CS2 = 1;

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

    //continue operation in IDLE mode
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

    //Write to buffer for transmission
    SPI1BUF = data;

    //Wait for transfer to be completed
    while (!SPI1STATbits.SPIRBF);

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
