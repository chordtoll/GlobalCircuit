#include "Timing.h"

unsigned long long loopstarttime;

unsigned long long GetTimer() {
    unsigned long long acc1;
    unsigned int tim;
    unsigned long long acc2;
    acc1=timer_accum;
    tim=ReadCoreTimer();
    acc2=timer_accum;
    if (acc2!=acc1) {
        tim=ReadCoreTimer();
    }
    return acc2+tim;
}

void WaitTicks(long n) {
    long long donetime=GetTimer()+n;
    while (GetTimer()<donetime);
}

void WaitUS(int n) {
    long long donetime=GetTimer()+n*(tps/1000000);
    while (GetTimer()<donetime);
}
void WaitMS(int n) {
    long long donetime=GetTimer()+n*(tps/1000);
    while (GetTimer()<donetime);
}
void WaitS(int n) {
    long long donetime=GetTimer()+n*tps;
    while (GetTimer()<donetime);
}

void InitLoopDelay() {
    loopstarttime=GetTimer();
}

void DelayLoopMS(int n) {
    if (GetTimer()<loopstarttime+n*(tps/1000))
        SendChar_UART1(' ');
    else
        SendChar_UART1('X');
    while (GetTimer()<loopstarttime+n*(tps/1000));
    loopstarttime+=n*(tps/1000);
}

unsigned int __attribute__((nomips16)) ReadCoreTimer(void)
{
    unsigned int timer;

    // get the count reg
    asm volatile("mfc0   %0, $9" : "=r"(timer));

    return timer;
}

////////////////////////////////////////////////////////////////////////////////
void __attribute__((nomips16)) WriteCoreTimer(unsigned int timer)
{
    /* get the count reg */
    asm volatile("mtc0   %0, $9": "+r"(timer));

}