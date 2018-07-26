#include "I2C.h"
#include "MS5607.h"

//Initializes altimeter module
char InitAltimeter(char addr) {
    char ack=0;
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack |= WriteI2C(ALT_CMD_RESET);
    StopI2C();
    return ack;
}

//Begins altimeter pressure conversion
char TriggerAltimeter_Pressure(char addr) {
    char ack=0;
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack |= WriteI2C(ALT_CMD_D1|ALT_OSR_4096);
    StopI2C();
    return ack;
}

//Begins altimeter temperature conversion
char TriggerAltimeter_Temperature(char addr) {
    char ack=0;
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack |= WriteI2C(ALT_CMD_D2|ALT_OSR_4096);
    StopI2C();
    return ack;
}

//Reads altimeter ADC value
char ReadAltimeter_ADC(char addr, int* val) {
    char ack=0;
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack |= WriteI2C(ALT_CMD_ADC);
    StopI2C();
    StartI2C();
    ack |= WriteI2C((addr << 1)+1);
    *val=(ReadI2C()&0xFF)<<16;
    AckI2C();
    *val|=(ReadI2C()&0xFF)<<8;
    AckI2C();
    *val|=ReadI2C()&0xFF;
    NAckI2C();
    StopI2C();
    //*val&=0x00FFFFFF;
    return ack;
}

//Reads altimeter parameter
int ReadAltimeter_Param(char addr, char loc) {
    int val=0;
    char ack=0;
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack |= WriteI2C(ALT_CMD_PROM+loc);
    StopI2C();
    StartI2C();
    ack |= WriteI2C((addr << 1)+1);
    val|=ReadI2C()<<8;
    AckI2C();
    val|=ReadI2C();
    NAckI2C();
    StopI2C();
    return val;
}
