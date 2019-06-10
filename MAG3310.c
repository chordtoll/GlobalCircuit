#include "I2C.h"
#include "MAG3310.h"

//Initializes magnetometer module
uint8_t InitMagneto(uint8_t addr) {
    uint8_t ack=0;
    StartI2C();                                //start I2C communication
    ack |= WriteI2C(addr << 1);                //enable automatic magnetic sensor resets
    ack |= WriteI2C(MAG_REG_CR2);
    ack |= WriteI2C(MAG_CR2_AMR);
    StopI2C();                                 //restart I2C communication
    StartI2C();
    ack |= WriteI2C(addr << 1);                //configure for 0.08Hz output rate
    ack |= WriteI2C(MAG_REG_CR1);
    ack |= WriteI2C(MAG_CR1_DR7|MAG_CR1_OS3);
    StopI2C();                                 //stop I2C communication
    return ack;                                //return acknowledge status
}

//Begins a magnetometer conversion
uint8_t TriggerMagneto(uint8_t addr) {
    uint8_t ack=0;
    StartI2C();                                //start I2C communication
    ack |= WriteI2C(addr << 1);                //Trigger a measurement at 0.08Hz
    ack |= WriteI2C(MAG_REG_CR1);
    ack |= WriteI2C(MAG_CR1_TIM|MAG_CR1_DR7|MAG_CR1_OS3);
    StopI2C();                                 //stop I2C communication
    return ack;                                //return acknowledge status
}

//Checks if magnetometer conversion complete
uint8_t CheckMagneto(uint8_t addr) {
    uint8_t val=0;
    uint8_t ack=0;
    StartI2C();                      //start I2C communication
    ack |= WriteI2C(addr << 1);      //request data from DR_STATUS register
    ack |= WriteI2C(MAG_REG_DRS);
    RestartI2C();                    //restart condition
    ack |= WriteI2C((addr << 1)+1);  //read in data from DR_STATUS register
    val=ReadI2C();
    NAckI2C();                       //send a NAck 
    StopI2C();                       //stop I2C communication
    return (val&0x04 );              //return flag for Z-axis new data available
}

//Reads values from magnetometer
uint8_t ReadMagneto(uint8_t addr, uint16_t* x, uint16_t* y, uint16_t* z) {
    uint8_t ack=0;
    StartI2C();                     //start I2C communication
    ack |= WriteI2C(addr << 1);     //request data from data registers
    ack |= WriteI2C(MAG_REG_VXM);
    RestartI2C();                   //restart condition
    ack |= WriteI2C((addr << 1)+1); //read in MSB of X data
    *x=(ReadI2C()&0xFF)<<8;
    AckI2C();        
    *x|=(ReadI2C()&0xFF);           //read in LSB of X data
    AckI2C();
    *y=(ReadI2C()&0xFF)<<8;         //read in MSB of Y data
    AckI2C();
    *y|=(ReadI2C()&0xFF);           //read in LSB of Y data
    AckI2C();
    *z=(ReadI2C()&0xFF)<<8;         //read in MSB of Z data
    AckI2C();
    *z|=(ReadI2C()&0xFF);           //read in LSB of Z data
    NAckI2C();                      //send a NAck
    StopI2C();                      //stop I2C communication
    return ack;                     //return acknowledge status
}

uint8_t ReadMagTemp(uint8_t addr, uint8_t *temp)
{
    uint8_t ack=0;
    StartI2C();                      //start I2C communication
    ack |= WriteI2C(addr << 1);      //request data from DIE_TEMP register
    ack |= WriteI2C(MAG_REG_TMP);
    RestartI2C();                    //restart condition
    ack |= WriteI2C((addr << 1)+1);  //read in data from DIE_TEMP register
    *temp=ReadI2C();
    NAckI2C();                       //send a NAck
    StopI2C();                       //stop I2C communication
    return ack;                      //return acknowledge status
}
