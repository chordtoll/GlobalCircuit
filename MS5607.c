#include "I2C.h"
#include "MS5607.h"

char alt_reset(char addr) {
    char ack=0;
    I2cStart();
    ack |= I2cWrite(addr << 1);
    ack |= I2cWrite(ALT_CMD_RESET);
    I2cStop();
    return ack;
}

char alt_start_pressure(char addr) {
    char ack=0;
    I2cStart();
    ack |= I2cWrite(addr << 1);
    ack |= I2cWrite(ALT_CMD_D1|ALT_OSR_4096);
    I2cStop();
    return ack;
}

char alt_start_temperature(char addr) {
    char ack=0;
    I2cStart();
    ack |= I2cWrite(addr << 1);
    ack |= I2cWrite(ALT_CMD_D2|ALT_OSR_4096);
    I2cStop();
    return ack;
}

char alt_read_adc(char addr, int* val) {
    char ack=0;
    I2cStart();
    ack |= I2cWrite(addr << 1);
    ack |= I2cWrite(ALT_CMD_ADC);
    I2cStop();
    I2cStart();
    ack |= I2cWrite((addr << 1)+1);
    *val=I2cRead()<<16;
    I2cAck();
    *val|=I2cRead()<<8;
    I2cAck();
    *val|=I2cRead();
    I2cNAck();
    I2cStop();
    *val&=0x00FFFFFF;
    return ack;
}

int alt_read_param(char addr, char loc) {
    int val=0;
    char ack=0;
    I2cStart();
    ack |= I2cWrite(addr << 1);
    ack |= I2cWrite(ALT_CMD_PROM+loc);
    I2cStop();
    I2cStart();
    ack |= I2cWrite((addr << 1)+1);
    val|=I2cRead()<<8;
    I2cAck();
    val|=I2cRead();
    I2cNAck();
    I2cStop();
    return val;
}
