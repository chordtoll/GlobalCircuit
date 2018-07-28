#include "I2C.h"
#include "MAG3310.h"

//Initializes magnetometer module
char InitMagneto(char addr) {
    char ack=0;
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack |= WriteI2C(MAG_REG_CR2);
    ack |= WriteI2C(MAG_CR2_AMR);
    StopI2C();
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack |= WriteI2C(MAG_REG_CR1);
    ack |= WriteI2C(MAG_CR1_DR7|MAG_CR1_OS3);
    StopI2C();
    return ack;
}

//Begins a magnetometer conversion
char TriggerMagneto(char addr) {
    char ack=0;
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack |= WriteI2C(MAG_REG_CR1);
    ack |= WriteI2C(MAG_CR1_TIM|MAG_CR1_DR7|MAG_CR1_OS3);
    StopI2C();
    return ack;
}

//Checks if magnetometer conversion complete
char CheckMagneto(char addr) {
    int val=0;
    char ack=0;
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack |= WriteI2C(MAG_REG_DRS);
    RestartI2C();
    ack |= WriteI2C((addr << 1)+1);
    val=ReadI2C();
    NAckI2C();
    StopI2C();
    return (val&0x04 );
}

//Reads values from magnetometer
char ReadMagneto(char addr, uint16_t* x, uint16_t* y, uint16_t* z) {
    char ack=0;
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack |= WriteI2C(MAG_REG_VXM);
    RestartI2C();
    ack |= WriteI2C((addr << 1)+1);
    *x=(ReadI2C()&0xFF)<<8;
    AckI2C();
    *x|=(ReadI2C()&0xFF);
    AckI2C();
    *y=(ReadI2C()&0xFF)<<8;
    AckI2C();
    *y|=(ReadI2C()&0xFF);
    AckI2C();
    *z=(ReadI2C()&0xFF)<<8;
    AckI2C();
    *z|=(ReadI2C()&0xFF);
    NAckI2C();
    StopI2C();
    return ack;
}
