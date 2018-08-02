/*
 * File:   UART.h
 * Author: Ethan
 *
 * Created on August 4, 2016, 3:46 PM
 */

#ifndef _TRANSMIT_H_
#define	_TRANSMIT_H_

#include "RockBlock.h"

volatile char GPSready;
volatile char GPSnew;
volatile char GPSdata[84];

volatile char _rb_cmdbuf[340];
volatile char RBRXbuf[270];
volatile char RBTXbuf[340];
volatile rb_status_t _rb_status;
volatile int _rb_idx;

void InitInterrupt();
void InitUART();
int HackBusyWait(unsigned char time);
int HackRockSend(unsigned char * message);
void SendString_UART1(unsigned char* string);
void SendBuffer_UART1(char *buf, int start, int len);
void SendChar_UART1(char letter);
void SendString_UART2(unsigned char* string);
void SendChar_UART2(char letter);

int strcmp(const char* s1, const char* s2);
char rbstrcmp(const char *s1,int s1i,const char *s2);
#endif	/* _TRANSMIT_H_ */