/* 
 * File:   GPIODebug.h
 * Author: asent
 *
 * Created on June 13, 2018, 3:04 PM
 */

#ifndef GPIODEBUG_H
#define	GPIODEBUG_H

void InitIO();
void SendChar_GPIO(char c);
void SendString_GPIO(char *s);

#endif	/* GPIODEBUG_H */

