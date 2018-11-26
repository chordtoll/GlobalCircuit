/* 
 * File:   GPIODebug.h
 * Author: asent
 *
 * Created on June 13, 2018, 3:04 PM
 */
/*
      PIC16  <->  PIC32

            DATA
pin9  | RC7  <->  pin5   | RE7
pin8  | RC6  <->  pin4   | RE6
pin7  | RC3  <->  pin99  | RE3
pin6  | RC4  <->  pin100 | RE4
pin5  | RC5  <->  pin3   | RE5
pin14 | RC2  <->  pin98  | RE2
pin15 | RC1  <->  pin94  | RE1
pin16 | RC0  <->  pin93  | RE0

            CLOCK
pin17 | RA2  <->  pin18  | RE8
*/

#ifndef GPIODEBUG_H
#define	GPIODEBUG_H

//initialize GPIO settings
void InitGPIO();

//send a single character on RE0-7
void SendChar_GPIO(char c);

//send a string, one character at a time, on RE0-7
void SendString_GPIO(char *s);

#endif	/* GPIODEBUG_H */

