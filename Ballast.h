/* 
 * File:   Ballast.h
 * Author: asent
 *
 * Created on August 29, 2018, 11:30 AM
 */

#ifndef BALLAST_H
#define	BALLAST_H

//runs through sequence of deploying the ballast with a passed address, returns 0 on success, 1 on failure
//DOCUMENT MORE FULLY FUNCTION OF EACH PIN
uint8_t DeployBallast(uint8_t addr);

#endif	/* BALLAST_H */

