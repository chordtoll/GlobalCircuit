/*
 * File:   MS5607.h
 * Author: asent
 *
 * Created on May 25, 2018, 2:41 PM
 */

#ifndef MS5607_H
#define	MS5607_H

#include <stdint.h>

//ADDRESSES
#define ALT_ADDR      0x77  //altimeter address 

//COMMANDS
#define ALT_CMD_RESET 0x1E  //reset
#define ALT_CMD_D1    0x40  //pressure conversion
#define ALT_CMD_D2    0x50  //temperature conversion
#define ALT_CMD_ADC   0x00  //ADC read
#define ALT_CMD_PROM  0xA0  //PROM read
#define ALT_OSR_256   0x00  //resolution at 256
#define ALT_OSR_512   0x02  //resolution at 512
#define ALT_OSR_1024  0x04  //resolution at 1024
#define ALT_OSR_2048  0x06  //resolution at 2048
#define ALT_OSR_4096  0x08  //resolution at 4096

#define ALT_C1_ADD 2
#define ALT_C2_ADD 4
#define ALT_C3_ADD 6
#define ALT_C4_ADD 8
#define ALT_C5_ADD 10
#define ALT_C6_ADD 12

uint64_t C1 = 0;
uint64_t C2 = 0;
uint64_t C3 = 0;
uint64_t C4 = 0;
uint64_t C5 = 0;
uint64_t C6 = 0;
int32_t dT = 0;

//initialize altimeter
uint8_t InitAltimeter(uint8_t addr);

//trigger a pressure reading and return acknowledge status
uint8_t TriggerAltimeter_Pressure(uint8_t addr);

//trigger a temperature reading and return acknowledge status
uint8_t TriggerAltimeter_Temperature(uint8_t addr);

//read ADC value, store result in *val and return acknowledge status
uint8_t ReadAltimeter_ADC(uint8_t addr, uint32_t* val);

//read a apecific location of PROM
uint16_t ReadAltimeter_Param(uint8_t addr, uint8_t loc);

//converts raw temperature reading to °K*100
uint32_t ConvertAltimeter_Temp(uint32_t rawTemp);

//converts raw pressure reading to mbar*100
uint32_t ConvertAltimeter_Pressure(uint32_t rawPress);

#endif	/* MS5607_H */

