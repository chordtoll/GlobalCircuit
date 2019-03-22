/* 
 * File:   GPIO.h
 * Author: asent
 *
 * Created on June 13, 2018, 3:04 PM
 */
/*
      PIC16  <->  PIC32         - FUNCTION

pin9  | RC7   -> pin5   | RE7   - PIC16_DATA1
pin8  | RC6   -> pin4   | RE6   - PIC16_DATA0
pin7  | RC3   -> pin99  | RE3   - PIC16_TxEnable
pin6  | RC4  <-  pin100 | RE4   - PIC32_CLK0
pin5  | RC5  <-  pin3   | RE5   - PIC32_CLK1
pin14 | RC2  <-  pin98  | RE2   - PIC32_TxEnable
pin15 | RC1  <-  pin94  | RE1   - PIC32_DATA1
pin16 | RC0  <-  pin93  | RE0   - PIC32_DATA0

pin17 | RA2  <->  pin18  | RE8  - UNUSED
*/

#ifndef GPIO_H
#define	GPIO_H

#include "proc\p32mx360f512l.h"


#define OUT_DATA0 PORTEbits.RE0     //bit 0 of Tx data
#define OUT_DATA1 PORTEbits.RE1     //bit 1 of Tx data
#define OUT_TxEnable PORTEbits.RE2  //Tx transmission flag
#define OUT_CLK0 PORTEbits.RE4      //bit 0 of Tx clock
#define OUT_CLK1 PORTEbits.RE5      //bit 1 of Tx clock
#define IN_TxEnable PORTEbits.RE3   //Rx transmission flag
#define IN_DATA0 PORTEbits.RE6      //bit 0 of Rx data
#define IN_DATA1 PORTEbits.RE7      //bit 1 of Rx data

#define CDFLAG_SUCCESS 1            //cutdown was a success
#define CDFLAG_INPROGRESS 0b10      //cutdown in progress
#define CDFLAG_NORSP16 0b100        //cutdown failed due to no response from PIC16
#define CDFLAG_NORSPCD 0b1000       //cutdown failed due to no response from cutdown apparatus
#define CDFLAG_BADRSP 0b10000       //cutdown failed due to a bad response from cutdown apparatus

typedef enum chgst {NONE,GND,UP,DOWN} chgst_t;

//flag indicating whether or not the cutdown procedure has been completed
char cutdown_ip = 0;

//initialize GPIO settings
void InitGPIO();

/*send and/or receive a character from PIC16
 * transmit = 0: No transmission
 *         != 0: Transmitting*/
char ExchangeChar_GPIO(char c, char transmit);

//send a string, one character at a time
void SendString_GPIO(char *s);

//charge probes to the specified state
void ChargeProbe(chgst_t state);
#endif	/* GPIO_H */