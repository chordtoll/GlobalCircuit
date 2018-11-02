#include "Timing.h"
#include "Yikes.h"
#include <proc/p32mx360f512l.h>
volatile uint8_t tmancount;

uint64_t loopstarttime;

void InitTimer() {
    T1CONbits.ON=0;     //Disable timer
    T1CONbits.TCS=0;    //Source: PBCLK
    T1CONbits.TCKPS=3;  //Prescale: 1/256
    TMR1=0;
    PR1=0xFFFF;         //
    IFS0bits.T1IF=0;
    IPC1bits.T1IP=7;
    IPC1bits.T1IS=3;
    IEC0bits.T1IE=1;
    T1CONbits.ON=1;
}

void  __attribute__((vector(_TIMER_1_VECTOR), interrupt(IPL7SRS), nomips16)) TIMER1_ISR(void)
{
    
    if (ReadCoreTimer()>TPS_MAX) {
        // BEGIN TIMING CRITICAL DO NOT SPLIT
        uint32_t ctt=ReadCoreTimer();
        WriteCoreTimer(0);
        timer_accum+=ctt;
        // END TIMING CRITICAL DO NOT SPLIT
        ctt_valid=0;
        yikes.gpstick=1;
    }
    IFS0bits.T1IF = 0; //clear interrupt flag status for Timer 1
}

uint64_t GetCoreTimer() {
    uint64_t acc1;
    uint32_t tim;
    uint64_t acc2;
    acc1=timer_accum;
    tim=ReadCoreTimer();
    acc2=timer_accum;
    if (acc2!=acc1) {
        tim=ReadCoreTimer();
    }
    return acc2+tim;
}

void WaitTicks(uint64_t n) {
    uint64_t donetime=GetCoreTimer()+n;
    while (GetCoreTimer()<donetime);
}

void WaitUS(uint32_t n) {
    uint64_t donetime=GetCoreTimer()+n*(tps/1000000); //TODO: investigate timing error? division before mult?
    while (GetCoreTimer()<donetime);
}
void WaitMS(uint32_t n) {
    uint64_t donetime=GetCoreTimer()+n*(tps/1000); //TODO: investigate timing error?
    while (GetCoreTimer()<donetime);
}
void WaitS(uint32_t n) {
    uint64_t donetime=GetCoreTimer()+n*tps;
    while (GetCoreTimer()<donetime);
}

void InitLoopDelay() {
    loopstarttime=GetCoreTimer();
}

void DelayLoopMS(uint32_t n) {
    if (GetCoreTimer()>=loopstarttime+n*(tps/1000)) {
        yikes.looprate=1;
    }
    while (GetCoreTimer()<loopstarttime+n*(tps/1000)); //TODO: investigate ^^^
    loopstarttime+=n*(tps/1000);
}

uint32_t __attribute__((nomips16)) ReadCoreTimer(void)
{
    uint32_t timer;

    // get the count reg
    asm volatile("mfc0   %0, $9" : "=r"(timer));

    return timer;
}

////////////////////////////////////////////////////////////////////////////////
void __attribute__((nomips16)) WriteCoreTimer(uint32_t timer)
{
    /* get the count reg */
    asm volatile("mtc0   %0, $9": "+r"(timer));

}