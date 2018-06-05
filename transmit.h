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

volatile char rockReady;
volatile char rockData[60];

void InterruptInit();
void UARTInit();
//int RockInit();

void RockBlockInit();
void RockBlockTransmitBinaryString(char* stringToSendInBinaryMode);
void RockBlockRecieveBinaryString(int* indexIntoGlobalString);

//The list of all the very specific commands for the rockblock
    //You need these for RockBlockInit()
int RockBlockEnableEcho(int* indexIntoGlobalString);                                                     //Turns on echo
int RockBlockDisableFlowControl(int* indexIntoGlobalString);                                             //Turns off flow control
int RockBlockPing(int* indexIntoGlobalString);                                                           //Just waiting for that sweet, sweet OK
int RockBlockDisableRinger(int* indexIntoGlobalString);                                                  //Turns the ringer off
    //You need these for RockBlockTransmitBinaryString()
int RockBlockClearISUBuffer(int* indexIntoGlobalString);                                                 //Clears the ISU buffer
int RockBlockSendBinaryStringToISUBuffer(char* stringToSendInBinaryMode, int* indexIntoGlobalString);    //Transfers data to the ISU buffer to be sent
int RockBlockInitiateSBDSession(int* indexIntoGlobalString);                                             //Initiates an SBD session
    //You need these for RockBlockRecieveBinaryString(int* indexIntoGlobalString)
int RockBlockRequestRecievedMessage(int* indexIntoGlobalString);                                         //Requests the recieved message in the ISU buffer

int RockBlockCheckResponse(int* indexIntoGlobalString);
void RockBlockSendString(char* command);
int RockBlockSendStringCheckResponse(char* command, int* indexIntoGlobalString);
void RockBlockSendCheckSumString(char* command);
int RockBlockSendCheckSumStringCheckResponse(char* command, int* indexIntoGlobalString);

//int HackBusyWait(unsigned char time);
//int HackRockSend(unsigned char * message);

void SendString(unsigned char* string, char checksum);
void SendChar(char letter);
//int strlen(char* string);
int saferStrLen(char* string, int maxLen);
int strcmp(const char* s1, const char* s2);

#endif	/* _TRANSMIT_H_ */