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
    while (GetTimer()<loopstarttime+n*(tps/1000));
    loopstarttime=GetTimer();
}