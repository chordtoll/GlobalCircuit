/* 
 * File:   Ballast.h
 * Author: asent
 *
 * Created on August 29, 2018, 11:30 AM
 */

#ifndef BALLAST_H
#define	BALLAST_H

char ballast_rq = 0;    //flag for ballast dealings

//runs through sequence of deploying the ballast with a passed address, updates flag based on success of deployment
//DOCUMENT MORE FULLY FUNCTION OF EACH PIN
void DeployBallast(uint8_t addr);

//returns a flag corresponding to the current status of ballast, clears status flag
uint8_t GetBallastStatus();

#endif	/* BALLAST_H */
