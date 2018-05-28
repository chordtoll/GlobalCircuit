#include "I2C.h"
#include "MAG3310.h"

char mag_reset(char addr) {
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

char mag_start(char addr) {
    char ack=0;
    I2cStart();
    ack |= I2cWrite(addr << 1);
    ack |= I2cWrite(MAG_REG_CR1);
    ack |= I2cWrite(MAG_CR1_TIM|MAG_CR1_DR7|MAG_CR1_OS3);
    I2cStop();
    return ack;
}

char mag_check(char addr) {
    int val=0;
    char ack=0;
    I2cStart();
    ack |= I2cWrite(addr << 1);
    ack |= I2cWrite(MAG_REG_DRS);
    I2cStop();
    I2cStart();
    ack |= I2cWrite((addr << 1)+1);
    val=I2cRead();
    I2cNAck();
    I2cStop();
    return (val&0x40);
}

char mag_read(char addr, short* x, short* y, short* z) {
    char ack=0;
    I2cStart();
    ack |= I2cWrite(addr << 1);
    ack |= I2cWrite(MAG_REG_VXM);
    I2cStop();
    I2cStart();
    ack |= I2cWrite((addr << 1)+1);
    x=I2cRead()<<8;
    I2cAck();
    x|=I2cRead();
    I2cAck();
    y=I2cRead()<<8;
    I2cAck();
    y|=I2cRead();
    I2cAck();
    z=I2cRead()<<8;
    I2cAck();
    z|=I2cRead();
    I2cNAck();
    I2cStop();
    return ack;
}