#include "I2C.h"
#include "MS5607.h"

//Initializes altimeter module
uint8_t InitAltimeter(uint8_t addr) {
    uint8_t ack=0;
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack |= WriteI2C(ALT_CMD_RESET);
    StopI2C();
    return ack;
}

//Begins altimeter pressure conversion
uint8_t TriggerAltimeter_Pressure(uint8_t addr) {
    uint8_t ack=0;
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack |= WriteI2C(ALT_CMD_D1|ALT_OSR_4096);
    StopI2C();
    return ack;
}

//Begins altimeter temperature conversion
uint8_t TriggerAltimeter_Temperature(uint8_t addr) {
    uint8_t ack=0;
    StartI2C();
    ack |= WriteI2C(addr << 1);
    ack |= WriteI2C(ALT_CMD_D2|ALT_OSR_4096);
    StopI2C();
    return ack;
}

//Reads altimeter ADC value
uint8_t ReadAltimeter_ADC(uint8_t addr, uint32_t* val) {
    uint8_t ack=0;
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
uint16_t ReadAltimeter_Param(uint8_t addr, uint8_t loc) {
    uint16_t val=0;
    uint8_t ack=0;
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
