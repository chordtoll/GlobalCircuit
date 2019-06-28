#include <stdint.h>
#include "proc\p32mx360f512l.h"
#include "Timing.h"
#include "Ballast.h"

#define BALLAST_IDLE()   PORTDbits.RD6=0; PORTDbits.RD7=0
#define BALLAST_ARM()    PORTDbits.RD6=1; PORTDbits.RD7=0
#define BALLAST_FIRE()   PORTDbits.RD6=0; PORTDbits.RD7=1
#define BALLAST_DISARM() PORTDbits.RD6=1; PORTDbits.RD7=1

#define BAFLAG_SUCCESS 1        //ballast was completed successfully
#define BAFLAG_NOACKADDR 2      //ballast did not receive an acknowledge after addressing
#define BAFLAG_NOACKARM 3       //ballast did not receive an acknowledge after arming
#define BAFLAG_NOACKFIRE 4      //ballast did not receive an acknowledge after firing

uint8_t ba_flag = 0;            //holds current ballast status

void AddrBallast(uint8_t addr) {
    PORTDbits.RD2=addr&1?1:0; //encode address nibble into RD2-5
    PORTDbits.RD3=addr&2?1:0;
    PORTDbits.RD4=addr&4?1:0;
    PORTDbits.RD5=addr&8?1:0;
}

void DeployBallast(uint8_t addr) {
    ResetWatchdog();
    BALLAST_IDLE();                           //set ballast idle
    AddrBallast(addr);                        //set ballast address
    StartKickTimer();
    while(PORTDbits.RD0 && !KICKED) {}        //wait for the correct signal, or break if waiting for too long
    if(KICKED)                                //if waiing for signal timed out,
    {
        BALLAST_IDLE();                       //set ballast idle
        ba_flag = BAFLAG_NOACKADDR;           //set ballast flag to no acknowledge address
        StopKickTimer();                      //stop the kick timer
        return;                               //break out of ballast deployment
    }
    ResetWatchdog();
    ResetKickTimer();
    BALLAST_ARM();                            //arm the ballast
    while(!PORTDbits.RD1 && !KICKED){}        //wait for correct response, or break if waiting for too long
    if(KICKED)                                //if waiting for signal timed out,
    {
        BALLAST_IDLE();                       //set ballast idle
        ba_flag = BAFLAG_NOACKARM;            //set ballast flag to no acknowledge arm
        StopKickTimer();                      //stop the kick timer
        return;                               //break out of ballast deployment
    }
    WaitUS(2814725);                          //wait for 2.8124 seconds (2.813950 - 2.815500)
    BALLAST_FIRE();                           //give fire signal
    ResetWatchdog();
    ResetKickTimer();
    while(!PORTDbits.RD0 && !KICKED){}         //wait for correct response, or break if waiting for too long
    if(KICKED)                                //if waiting for signal timed out,
        ba_flag = BAFLAG_NOACKFIRE;           //set flag to no acknowledge fire
    else                                      //if a acknowledge was received
        ba_flag = BAFLAG_SUCCESS;             //set flag to success
    BALLAST_IDLE();                           //set ballast idle
    StopKickTimer();                          //stop the kick timer
}

uint8_t GetBallastStatus()
{
    if(ballast_rq)              //if ballast has been requested
    {
        ++ballast_rq;           //increment the ballast request counter
        if(ballast_rq == 8)     //if the window for confirmation was missed
        {
            ballast_rq = 0;     //clear the ballast request counter
            return 0;           //clear the ballast flag
        }
        else                    //if currently waiting for confirmation
            return 0xAC;        //send ballast acknowledge
    }
    else
    {
        uint8_t temp = ba_flag; //store current value of ballast flag
        ba_flag = 0;            //clear ballast flag
        return temp;            //return ballast status
    }
}
