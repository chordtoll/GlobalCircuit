#include "I2C.h"
#include "MAG3310.h"

//Initializes magnetometer module
char InitMagneto(char addr) {
    char ack=0;
    I2cStart();
    ack |= I2cWrite(addr << 1);
    ack |= I2cWrite(MAG_REG_CR2);
    ack |= I2cWrite(MAG_CR2_AMR);
    I2cStop();
    I2cStart();
    ack |= I2cWrite(addr << 1);
    ack |= I2cWrite(MAG_REG_CR1);
    ack |= I2cWrite(MAG_CR1_DR7|MAG_CR1_OS3);
    I2cStop();
    return ack;
}

//Begins a magnetometer conversion
char TriggerMagneto(char addr) {
    char ack=0;
    I2cStart();
    ack |= I2cWrite(addr << 1);
    ack |= I2cWrite(MAG_REG_CR1);
    ack |= I2cWrite(MAG_CR1_TIM|MAG_CR1_DR7|MAG_CR1_OS3);
    I2cStop();
    return ack;
}

//Checks if magnetometer conversion complete
char CheckMagneto(char addr) {
    int val=0;
    char ack=0;
    I2cStart();
    ack |= I2cWrite(addr << 1);
    ack |= I2cWrite(MAG_REG_DRS);
    I2cRestart();
    ack |= I2cWrite((addr << 1)+1);
    val=I2cRead();
    I2cNAck();
    I2cStop();
    return (val&0x04 );
}

//Reads values from magnetometer
char ReadMagneto(char addr, short* x, short* y, short* z) {
    char ack=0;
    I2cStart();
    ack |= I2cWrite(addr << 1);
    ack |= I2cWrite(MAG_REG_VXM);
    I2cRestart();
    ack |= I2cWrite((addr << 1)+1);
    *x=I2cRead()<<8;
    I2cAck();
    *x|=I2cRead();
    I2cAck();
    *y=I2cRead()<<8;
    I2cAck();
    *y|=I2cRead();
    I2cAck();
    *z=I2cRead()<<8;
    I2cAck();
    *z|=I2cRead();
    I2cNAck();
    I2cStop();
    return ack;
}
