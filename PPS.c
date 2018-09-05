
#include <proc/p32mx360f512l.h>
#include "Timing.h"
#include "Yikes.h"

void InitPPS() {
    uint8_t i;
    TRISDbits.TRISD14=1;
    CNCONbits.ON=1;
    CNENbits.CNEN20=1;
    PORTD;
    IPC6bits.CNIP=7;
    IPC6bits.CNIS=3;
    IFS1bits.CNIF=0;
    IEC1bits.CNIE=1;
    ctt_valid=0;
}

void  __attribute__((vector(_CHANGE_NOTICE_VECTOR), interrupt(IPL7SRS), nomips16)) CN_ISR(void) {
    if (PORTDbits.RD14) {
        
        if (ReadCoreTimer()<TPS_MAX) {
            // BEGIN TIMING CRITICAL DO NOT SPLIT
            uint32_t ctt=ReadCoreTimer();
            WriteCoreTimer(0);
            timer_accum+=ctt;
            // END TIMING CRITICAL DO NOT SPLIT
            if (ctt>TPS_MIN && ctt_valid) {
                tps*=9;
                tps+=ctt;
                tps/=10;
            } else {
                yikes.gpstick=1;
            }
            ctt_valid=1;
        } else {
            yikes.gpstick=1;
        }
        
        
        
    }
    IFS1bits.CNIF=0;
}