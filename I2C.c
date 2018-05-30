#include "I2C.h"
#include <proc/p32mx360f512l.h>


void I2cConfig(void)
{
    I2C1BRG = 0x0058; //Set the bit rate to about 400kHz.
    //Turn on the I2C module by setting the ON bit (I2CxCON<15>) to '1'.
    I2C1CONbits.ON = 1;
}

char I2cSend(char addr, char* data, int size)
{
    char ack = 0;
    //Start communication
    I2cStart();
    //Send the device address byte to the slave with a write indication.
    ack |= I2cWrite(addr << 1);
    //Send out data
    while(size--)
    {
        ack |= I2cWrite(*data);
        ++data;
    }
    //Stop Communication
    I2cStop();
    return ack;
}

char I2cEnum(char addr)
{
    char ack = 0;
    I2cStart();
    ack |= I2cWrite(addr << 1);
    ack &= I2cWrite(0);
    ack &= I2cWrite(0);
    I2cStop();
    return ack;
}

char I2cReceive(char addr, char* data, int size)
{
    char ack = 0;
    //Send start bit
    I2cStart();
    //Send address with high LSB (read)
    ack |= I2cWrite((addr << 1) + 1);
    //Start reception of data
    if(I2C1STATbits.RBF)
    I2cRead(); //Read a dummy value
    while(size--) //runs until all are read
    {
        //Send a restart bit.
        I2cRestart();
        //Write read byte to data array.
        *data = I2cRead();
        //Increment data pointer.
        ++data;
        //If not the last read.
        if(size)
            I2cAck(); //Send acknowledge bit.
    }
    //Send stop bit.
    I2cStop();
    return ack;
}

char I2cRead(void)
{
    I2C1CONbits.RCEN = 1;
    while(I2C1CONbits.RCEN == 1);
    return I2C1RCV;
}
char I2cWrite(char data)
{
    I2C1TRN = data;
    while(I2C1STATbits.TRSTAT == 1);
    return I2C1STATbits.ACKSTAT;
}

void I2cStart(void)
{
    I2C1CONbits.SEN = 1;
    while(I2C1CONbits.SEN == 1);
}

void I2cStop(void)
{
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN == 1);
}

void I2cRestart(void)
{
    I2C1CONbits.RSEN = 1;
    while(I2C1CONbits.RSEN == 1);
}

void I2cAck(void)
{
    I2C1CONbits.ACKDT = 0;
    I2C1CONbits.ACKEN = 1;
    while(I2C1CONbits.ACKEN == 1);
}
void I2cNAck(void)
{
    I2C1CONbits.ACKDT = 1;
    I2C1CONbits.ACKEN = 1;
    while(I2C1CONbits.ACKEN == 1);
}

