#include <stdint.h>
#include "proc\p32mx360f512l.h"
#include "Timing.h"
#include "Ballast.h"

#define BALLAST_IDLE()   PORTDbits.RD6=0; PORTDbits.RD7=0
#define BALLAST_ARM()    PORTDbits.RD6=1; PORTDbits.RD7=0
#define BALLAST_FIRE()   PORTDbits.RD6=0; PORTDbits.RD7=1
#define BALLAST_DISARM() PORTDbits.RD6=1; PORTDbits.RD7=1

#define BALLTIME 2814725        //time in microseconds to wait before firing

uint8_t AddrBallast(uint8_t addr) {
    uint8_t exists;
    PORTDbits.RD2=addr&1?1:0; //encode address nibble into RD2-5
    PORTDbits.RD3=addr&2?1:0;
    PORTDbits.RD4=addr&4?1:0;
    PORTDbits.RD5=addr&8?1:0;
    BALLAST_ARM();
    WaitMS(1);
    exists = !(PORTDbits.RD0);
    BALLAST_IDLE();
    return exists;
}

void DeployBallast() {
    ResetWatchdog();
    BALLAST_ARM();                            //arm the ballast
    while(!PORTDbits.RD1 && !KICKED){}        //wait for correct response, or break if waiting for too long
    if(KICKED)                                //if waiting for signal timed out,
    {
        BALLAST_IDLE();                       //set ballast idle
        ball_state = BALL_NOACKARM;            //set ballast flag to no acknowledge arm
        StopKickTimer();                      //stop the kick timer
        return;                               //break out of ballast deployment
    }
    WaitUS(BALLTIME);                         //wait for 2.8124 seconds (2.813950 - 2.815500)
    BALLAST_FIRE();                           //give fire signal
    ResetWatchdog();
    ResetKickTimer();
    while(!PORTDbits.RD0 && !KICKED){}        //wait for correct response, or break if waiting for too long
    if(KICKED)                                //if waiting for signal timed out,
        ball_state = BALL_NOACKFIRE;          //set flag to no acknowledge fire
    else                                      //if a acknowledge was received
        ball_state = BALL_SUCCESS;            //set flag to success
    BALLAST_IDLE();                           //set ballast idle
    StopKickTimer();                          //stop the kick timer
}
