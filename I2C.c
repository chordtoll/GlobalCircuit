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
    uint8_t ack = 0;            //acknowledge bit
    StartI2C();                 //start the I2C communication
    ack |= WriteI2C(addr << 1); //write the address passed in and store acknowledge status
    ack &= WriteI2C(0);         //write a 0 and store acknowledge status
    ack &= WriteI2C(0);
    StopI2C();                  //stop the I2C communication
    return ack;                 //return acknowledge status
}

uint8_t ReadI2C(void)            
{
    I2C1CONbits.RCEN = 1;         //enable recieve mode
    while(I2C1CONbits.RCEN == 1); //wait full read
    return I2C1RCV;               //return the recieved data
}
uint8_t WriteI2C(uint8_t data)
{
    I2C1TRN = data;                  //write data
    while(I2C1STATbits.TRSTAT == 1); //wait until data is transmitted fully
    return I2C1STATbits.ACKSTAT;     //return acknowledge status
}

void StartI2C(void)
{
    I2C1CONbits.SEN = 1;         //initiate start condition
    while(I2C1CONbits.SEN == 1); //wait until startup is complete
}

void StopI2C(void)
{
    I2C1CONbits.PEN = 1;         //initiate stop condition
    while(I2C1CONbits.PEN == 1); //wait until fully stopped
}

void RestartI2C(void)
{
    I2C1CONbits.RSEN = 1;         //initiate restart condition
    while(I2C1CONbits.RSEN == 1); //wait until fully restarted
}

void AckI2C(void)
{
    I2C1CONbits.ACKDT = 0;         //!ACK is sent when user initiates acknowledge sequence
    I2C1CONbits.ACKEN = 1;         //Initiate acknowledge sequence
    while(I2C1CONbits.ACKEN == 1); //wait for the acknowledge sequence to complete
}
void NAckI2C(void)
{
    I2C1CONbits.ACKDT = 1;         //NACK is sent when user initiates acknowledge sequence
    I2C1CONbits.ACKEN = 1;         //initiate acknowledge sequence
    while(I2C1CONbits.ACKEN == 1); //wait for the acknowledge sequence to complete
}

