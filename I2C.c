#include "I2C.h"
#include <proc/p32mx360f512l.h>


void InitI2C(void)
{
    I2C1BRG = 0x0184; //Set the bit rate to about 100kHz.
    //Turn on the I2C module by setting the ON bit (I2CxCON<15>) to '1'.
    I2C1CONbits.ON = 1;
}

uint8_t EnumI2C(uint8_t addr)
{
    uint8_t ack = 0;
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack &= WriteI2C(0);
    ack &= WriteI2C(0);
    StopI2C();
    return ack;
}

uint8_t ReadI2C(void)
{
    I2C1CONbits.RCEN = 1;
    while(I2C1CONbits.RCEN == 1);
    return I2C1RCV;
}
uint8_t WriteI2C(uint8_t data)
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

