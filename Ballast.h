/* 
 * File:   Ballast.h
 * Author: asent
 *
 * Created on August 29, 2018, 11:30 AM
 */

#ifndef BALLAST_H
#define	BALLAST_H

#define BALL_ACK_TIMEOUT 10 //number of minutes that ballast will wait for acknowledge before timing out

typedef enum {BALL_DISARMED, BALL_ARMED, BALL_SUCCESS, BALL_ALREADY_DROPPED, BALL_NOACKARM, BALL_NOACKFIRE} ballast_state_t;
ballast_state_t ball_state = BALL_DISARMED;

//Drops current addressed ballast
void DeployBallast();

//Sets active ballast to the given address
//returns 1: Ballast exists
//returns 0: Ballast doesn't exist (dropped already)
uint8_t AddrBallast(uint8_t addr);

#endif	/* BALLAST_H */
