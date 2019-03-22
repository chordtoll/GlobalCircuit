/* 
 * File:   Cutdown.h
 * Author: Collin MacDicken
 *
 * Created on March 21, 2019, 8:00 PM
 */

#ifndef CUTDOWN_H
#define	CUTDOWN_H

//try a cutdown with the PIC16, returns 1 on success, 0 on failure
void InitiateCutdown();

//update yikes and cutdown flags based on result read from PIC16
uint8_t GetCutdownStatus();

#endif	/* CUTDOWN_H */

