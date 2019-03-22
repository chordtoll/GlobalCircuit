#include "I2C.h"
#include "MS5607.h"

//Initializes altimeter module
uint8_t InitAltimeter(uint8_t addr) {
    uint8_t ack=0;
    StartI2C();                    //start I2C communication
    ack |= WriteI2C(addr << 1);    //reset the altimeter
    ack |= WriteI2C(ALT_CMD_RESET);
    StopI2C();                     //stop I2C communication
    return ack;                    //return acknowledge status
}

//Begins altimeter pressure conversion
uint8_t TriggerAltimeter_Pressure(uint8_t addr) {
    uint8_t ack=0;
    StartI2C();                              //start i2C communication
    ack |= WriteI2C(addr << 1);              //trigger for pressure conversion with resolution of 4096
    ack |= WriteI2C(ALT_CMD_D1|ALT_OSR_4096);
    StopI2C();                               //stop I2C communication
    return ack;                              //return acknowledge status
}

//Begins altimeter temperature conversion
uint8_t TriggerAltimeter_Temperature(uint8_t addr) {
    uint8_t ack=0;
    StartI2C();                              //start I2C communication
    ack |= WriteI2C(addr << 1);              //trigger for temperature conversion with resolution of 4096
    ack |= WriteI2C(ALT_CMD_D2|ALT_OSR_4096);
    StopI2C();                               //stop I2C communication
    return ack;                              //return acknowledge status
}

//Reads altimeter ADC value
uint8_t ReadAltimeter_ADC(uint8_t addr, uint32_t* val) {
    uint8_t ack=0;
    StartI2C();                     //start I2C communication
    ack |= WriteI2C(addr << 1);     //request data from ADC
    ack |= WriteI2C(ALT_CMD_ADC);
    StopI2C();
    StartI2C();
    ack |= WriteI2C((addr << 1)+1);
    *val=(ReadI2C()&0xFF)<<16;      //store most significant byte of ADC value
    AckI2C();
    *val|=(ReadI2C()&0xFF)<<8;      //store middle byte of ADC value
    AckI2C();
    *val|=ReadI2C()&0xFF;           //store least significant byte of ADC value
    NAckI2C();                      //send a NAck
    StopI2C();                      //stop I2C communication
    return ack;                     //return acknowledge status
}

//Reads altimeter parameter
uint16_t ReadAltimeter_Param(uint8_t addr, uint8_t loc) {
    uint16_t val=0;
    uint8_t ack=0;
    StartI2C();                       //start I2C communication
    ack |= WriteI2C(addr << 1);       //request data from passed in address of altimeter PROM
    ack |= WriteI2C(ALT_CMD_PROM+loc);
    StopI2C();                       
    StartI2C();
    ack |= WriteI2C((addr << 1)+1);   
    val|=ReadI2C()<<8;                //store most significant byte of result
    AckI2C();
    val|=ReadI2C();                   //store least significant byte of result
    NAckI2C();                        //send a NAck
    StopI2C();                        //stop I2C communication
    return val;                       //return read in value
}
