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
volatile char GPSdata[84];

void InitInterrupt();
void InitUART();
int HackBusyWait(unsigned char time);
int HackRockSend(unsigned char * message);
void SendString_UART1(unsigned char* string);
void SendChar_UART1(char letter);
void SendString_UART2(unsigned char* string);
void SendChar_UART2(char letter);
//int strlen(char* string);
int strcmp(const char* s1, const char* s2);

#endif	/* _TRANSMIT_H_ */