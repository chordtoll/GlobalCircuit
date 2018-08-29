/* 
 * File:   Stubs.h
 * Author: asent
 *
 * Created on July 25, 2018, 11:19 PM
 */

#include <stdint.h>

#ifndef STUBS_H
#define	STUBS_H

typedef enum chgst {NONE,GND,UP,DOWN} chgst_t;

void InitADC_S();
uint16_t ReadADC_S(uint8_t channel);

uint16_t ReadPICADC_S(uint8_t channel);

void TriggerMagneto_S();
void TriggerAltimeter_Pressure_S();
void TriggerAltimeter_Temperature_S();

void ReadMagneto_S(uint16_t* a, uint16_t* b, uint16_t* c);

uint32_t ReadAltimeter_S();

void ReadGPS_S(uint32_t* time, uint32_t* lat, uint32_t* lon, uint32_t* alt);

void ChargeProbe_S(chgst_t state);

void RockSend_S(char* packet);
char RockCheck_S();
char RockReceive_S(char* packet);

#endif	/* STUBS_H */

