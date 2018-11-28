#include <stdint.h>
#include "proc\p32mx360f512l.h"


#define BALLAST_IDLE()   PORTDbits.RD6=0; PORTDbits.RD7=0
#define BALLAST_ARM()    PORTDbits.RD6=1; PORTDbits.RD7=0
#define BALLAST_FIRE()   PORTDbits.RD6=0; PORTDbits.RD7=1
#define BALLAST_DISARM() PORTDbits.RD6=1; PORTDbits.RD7=1


void AddrBallast(uint8_t addr) {
    PORTDbits.RD2=addr&1?1:0; //encode address byte into RD2-5
    PORTDbits.RD3=addr&2?1:0;
    PORTDbits.RD4=addr&4?1:0;
    PORTDbits.RD5=addr&8?1:0;
}

uint8_t DeployBallast(uint8_t addr) {
    ResetWatchdog();
    BALLAST_IDLE();        //set ballast idle
    AddrBallast(addr);     //set ballast address
    WaitS(2);              //wait for 2 seconds
    if (PORTDbits.RD0) {   //if the ballast did not acknowledge our signal
        BALLAST_IDLE();    //set ballast idle
        return 1;          //return failed condition
    }
    ResetWatchdog();
    BALLAST_ARM();         //arm the ballast
    while (!PORTDbits.RD1);//wait for response (forever until signal is high)
    WaitUS(2812400);       //wait for 2.8124 seconds
    BALLAST_FIRE();        //give fire signal
    ResetWatchdog();
    while (PORTDbits.RD1); //wait for response (forever until signal is low)
    BALLAST_IDLE();        //set ballast idle
    return 0;              //return success condition
    /*BALLAST_IDLE();
    WaitMS(10);
    BALLAST_ARM();
    WaitMS(10);
    BALLAST_FIRE();
    WaitMS(10);
    BALLAST_IDLE();*/
}
