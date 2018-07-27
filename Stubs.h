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
void SampleADC_S(char channel);
int ReadADC_S(char channel);

int ReadPICADC_S(char channel);

void TriggerMagneto_S();
void TriggerAltimeter_Pressure_S();
void TriggerAltimeter_Temperature_S();

void ReadMagneto_S(uint16_t* a, uint16_t* b, uint16_t* c);

uint32_t ReadAltimeter_S();

void ReadGPS_S(double* time, double* latitude, double* longitude, double* altitude);

void ChargeProbe_S(chgst_t state);

void RockSend_S(char* packet);
char RockCheck_S();
char RockReceive_S(char* packet);

#endif	/* STUBS_H */

