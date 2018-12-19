/* 
 * File:   GPIODebug.h
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

            CLOCK (NOT CURRENTLY USED)
pin17 | RA2  <->  pin18  | RE8
*/

#ifndef GPIODEBUG_H
#define	GPIODEBUG_H

//initialize GPIO settings
void InitGPIO();

/*send a single character on RE0-7
 * transmit = 0: No transmission
 *          = 1: Transmitting
 *          > 1: Transmitting final byte
char SendChar_GPIO(char c, char end);
*/
//send a string, one character at a time, on RE0-7
void SendString_GPIO(char *s);

#endif	/* GPIODEBUG_H */

