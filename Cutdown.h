/* 
 * File:   Cutdown.h
 * Author: Collin MacDicken
 *
 * Created on March 21, 2019, 8:00 PM
 */

#ifndef CUTDOWN_H
#define	CUTDOWN_H

#define CUT_ACK_TIMEOUT 10   //number of minutes that cutdown will wait for acknowledge before timing out

char cutdown_rq = 0;         //flag for cutdown dealings

//try a cutdown with the PIC16, returns 1 on success, 0 on failure
void InitiateCutdown();

//update yikes and cutdown flags based on result read from PIC16
uint8_t GetCutdownStatus();

#endif	/* CUTDOWN_H */

