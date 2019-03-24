/*
 * File:   UART.h
 * Author: Ethan
 *
 * Created on August 4, 2016, 3:46 PM
 */

#ifndef _UART_H_
#define	_UART_H_

#include "RockBlock.h"
#include "Packet.h"


volatile char _rb_cmdbuf[340];
volatile char RBRXbuf[270];
volatile char RBTXbuf[340];
volatile rb_status_t _rb_status;
volatile uint16_t _rb_idx;

void InitInterrupt();
void InitUART();
void SendString_UART1(unsigned char* string);
void SendBuffer_UART1(char *buf, uint16_t start, uint16_t len);
void SendChar_UART1(char letter);
void SendString_UART2(unsigned char* string);
void SendChar_UART2(char letter);

void SafeDebugString(unsigned char* string);

void Send16_UART1(uint16_t i);

char rbstrcmp(volatile char *s1,uint16_t s1i,const char *s2);

void clearPacket(packet_u *pack);
void printPacket(packet_u pack);
#endif	/* _UART_H_ */