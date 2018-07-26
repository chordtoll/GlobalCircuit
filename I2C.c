#include "I2C.h"
#include <proc/p32mx360f512l.h>


void InitI2C(void)
{
    I2C1BRG = 0x0184; //Set the bit rate to about 100kHz.
    //Turn on the I2C module by setting the ON bit (I2CxCON<15>) to '1'.
    I2C1CONbits.ON = 1;
}

char SendI2C(char addr, char* data, int size)
{
    char ack = 0;
    //Start communication
    StartI2C();
    //Send the device address byte to the slave with a write indication.
    ack |= WriteI2C(addr << 1);
    //Send out data
    while(size--)
    {
        ack |= WriteI2C(*data);
        ++data;
    }
    //Stop Communication
    StopI2C();
    return ack;
}

char EnumI2C(char addr)
{
    char ack = 0;
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack &= WriteI2C(0);
    ack &= WriteI2C(0);
    StopI2C();
    return ack;
}

char ReceiveI2C(char addr, char* data, int size)
{
    char ack = 0;
    //Send start bit
    StartI2C();
    //Send address with high LSB (read)
    ack |= WriteI2C((addr << 1) + 1);
    //Start reception of data
    if(I2C1STATbits.RBF)
    ReadI2C(); //Read a dummy value
    while(size--) //runs until all are read
    {
        //Send a restart bit.
        RestartI2C();
        //Write read byte to data array.
        *data = ReadI2C();
        //Increment data pointer.
        ++data;
        //If not the last read.
        if(size)
            AckI2C(); //Send acknowledge bit.
    }
    //Send stop bit.
    StopI2C();
    return ack;
}

char ReadI2C(void)
{
    I2C1CONbits.RCEN = 1;
    while(I2C1CONbits.RCEN == 1);
    return I2C1RCV;
}
char WriteI2C(char data)
{
    I2C1TRN = data;
    while(I2C1STATbits.TRSTAT == 1);
    return I2C1STATbits.ACKSTAT;
}

void StartI2C(void)
{
    I2C1CONbits.SEN = 1;
    while(I2C1CONbits.SEN == 1);
}

void StopI2C(void)
{
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN == 1);
}

void RestartI2C(void)
{
    I2C1CONbits.RSEN = 1;
    while(I2C1CONbits.RSEN == 1);
}

void AckI2C(void)
{
    I2C1CONbits.ACKDT = 0;
    I2C1CONbits.ACKEN = 1;
    while(I2C1CONbits.ACKEN == 1);
}
void NAckI2C(void)
{
    I2C1CONbits.ACKDT = 1;
    I2C1CONbits.ACKEN = 1;
    while(I2C1CONbits.ACKEN == 1);
}

