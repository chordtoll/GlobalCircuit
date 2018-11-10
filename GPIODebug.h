/* 
 * File:   GPIODebug.h
 * Author: asent
 *
 * Created on June 13, 2018, 3:04 PM
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

