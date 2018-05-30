/*
 * File:   UART.h
 * Author: Ethan
 *
 * Created on August 4, 2016, 3:46 PM
 */

#ifndef _TRANSMIT_H_
#define	_TRANSMIT_H_

volatile char GPSready;
volatile char GPSnew;
volatile char GPSdat[84];

void InterruptInit();
void UARTInit();
int HackBusyWait(unsigned char time);
int HackRockSend(unsigned char * message);
void SendString(unsigned char* string, char checksum);
void SendChar(char letter);
//int strlen(char* string);
int strcmp(const char* s1, const char* s2);

#endif	/* _TRANSMIT_H_ */