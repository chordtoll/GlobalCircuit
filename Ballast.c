#include <stdint.h>
#include "proc\p32mx360f512l.h"


#define BALLAST_IDLE()   PORTDbits.RD6=0; PORTDbits.RD7=0
#define BALLAST_ARM()    PORTDbits.RD6=1; PORTDbits.RD7=0
#define BALLAST_FIRE()   PORTDbits.RD6=0; PORTDbits.RD7=1
#define BALLAST_DISARM() PORTDbits.RD6=1; PORTDbits.RD7=1

#define BAFLAG_SUCCESS 1        //ballast was completed successfully
#define BAFLAG_NOACKADDR 0b10   //ballast did not receive an acknowledge after addressing
#define BAFLAG_NOACKARM 0b100   //ballast did not receive an acknowledge after arming
#define BAFLAG_NOACKFIRE 0b1000 //ballast did not receive an acknowledge after firing

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
    if (!WaitForSignal(1000, 2000, 0, 0)) {   //if the ballast did not acknowledge our signal in a 2 second window,
        ba_flag = BAFLAG_NOACKADDR;           //set ballast flag to no acknowledge address
        BALLAST_IDLE();                       //set ballast idle
        return;                               //break out of ballast deployment
    }
    ResetWatchdog();
    BALLAST_ARM();                            //arm the ballast
    if(!WaitForSignal(10, 500000, 1, 1))      //if the ballast did not acknowledge arming in a 5 second window,
    {
        ba_flag = BAFLAG_NOACKARM;            //set ballast flag to no acknowledge arm
        BALLAST_IDLE();                       //set ballast idle
        return;                               //break out of ballast deployment
    }
    WaitUS(2814300);                          //wait for 2.8143 seconds
    BALLAST_FIRE();                           //give fire signal
    ResetWatchdog();
    if(!WaitForSignal(10, 500000, 0, 1))      //if the ballast did not acknowledge firing in a 5 second window,
        ba_flag = BAFLAG_NOACKFIRE;           //set flag to no acknowledge fire
    else                                      //if a acknowledge was received
        ba_flag = BAFLAG_SUCCESS;             //set flag to success
    BALLAST_IDLE();                           //set ballast idle
}

uint8_t GetBallastStatus()
{
    uint8_t temp = ba_flag; //store current value of ballast flag
    ba_flag = 0;            //clear ballast flag
    return temp;            //return ballast status
}
