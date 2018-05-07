/*
 * File:   UART.h
 * Author: Ethan
 *
 * Created on August 4, 2016, 3:46 PM
 */

#ifndef _TRANSMIT_H_
#define	_TRANSMIT_H_

void InterruptInit();
void UARTInit();
int RockInit();
int checkService();
char ReceivedLine(void);
void SendString(unsigned char* string, char checksum);
volatile char* GetString(void);
void SendChar(char letter);
//int strlen(char* string);
int strcmp(const char* s1, const char* s2);

#endif	/* _TRANSMIT_H_ */