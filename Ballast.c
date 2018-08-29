#include <stdint.h>
#include "proc\p32mx360f512l.h"


#define BALLAST_IDLE()   PORTDbits.RD6=0; PORTDbits.RD7=0
#define BALLAST_ARM()    PORTDbits.RD6=1; PORTDbits.RD7=0
#define BALLAST_FIRE()   PORTDbits.RD6=0; PORTDbits.RD7=1
#define BALLAST_DISARM() PORTDbits.RD6=1; PORTDbits.RD7=1


void AddrBallast(uint8_t addr) {
    PORTDbits.RD2=addr&1?1:0;
    PORTDbits.RD3=addr&2?1:0;
    PORTDbits.RD4=addr&4?1:0;
    PORTDbits.RD5=addr&8?1:0;
}

uint8_t DeployBallast(uint8_t addr) {
    BALLAST_IDLE();
    AddrBallast(addr);
    WaitS(2);
    if (PORTDbits.RD0) {
        BALLAST_IDLE();
        return 1;
    }
    BALLAST_ARM();
    while (!PORTDbits.RD1);
    WaitS(3);
    BALLAST_FIRE();
    while (PORTDbits.RD1);
    BALLAST_IDLE();
    return 0;
}

