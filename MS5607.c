#include "I2C.h"
#include "MS5607.h"

//Initializes altimeter module
uint8_t StartAltimeter(uint8_t addr) {
    uint8_t ack=0;
    StartI2C();                    //start I2C communication
    ack |= WriteI2C(addr << 1);    //reset the altimeter
    ack |= WriteI2C(ALT_CMD_RESET);
    StopI2C();                     //stop I2C communication
    return ack;                    //return acknowledge status
}

uint8_t InitAltimeter(uint8_t addr)
{
    StartAltimeter(addr);
    WaitMS(10);
    C1 = ReadAltimeter_Param(addr, ALT_C1_ADD);
    C2 = ReadAltimeter_Param(addr, ALT_C2_ADD);
    C3 = ReadAltimeter_Param(addr, ALT_C3_ADD);
    C4 = ReadAltimeter_Param(addr, ALT_C4_ADD);
    C5 = ReadAltimeter_Param(addr, ALT_C5_ADD);
    C6 = ReadAltimeter_Param(addr, ALT_C6_ADD);
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

uint32_t ConvertAltimeter_Temp(uint32_t rawTemp)
{
    dT = rawTemp - C5 * (1 << 8);
    int64_t temp = (2000+dT*C6/(1<<23))+27300;
    return temp;
}

int32_t ConvertAltimeter_Pressure(uint32_t rawPress)
{
    int64_t off = C2*(1 << 17) + (C4*dT)/(1 << 6);
    int64_t sens = C1*(1 << 16) + (C3*dT)/(1 << 7);
    int32_t press = (rawPress*sens/(1 << 21) - off)/(1<<15);
    return press;
}