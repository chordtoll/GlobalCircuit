#include "Timing.h"
#include "Yikes.h"
#include <proc/p32mx360f512l.h>
volatile uint8_t tmancount;

uint64_t loopstarttime;

void InitTimer() {
    T1CONbits.ON=0;     //Disable timer
    T1CONbits.TCS=0;    //Source: PBCLK
    T1CONbits.TCKPS=3;  //Prescale: 1/256
    TMR1=0;             //Clear timer register
    PR1=0xFFFF;         //Load period register
    IFS0bits.T1IF=0;    //clear timer interrupt flag
    IPC1bits.T1IP=7;    //set interrupt priority to 7 (max)
    IPC1bits.T1IS=3;    //set interrupt subpriority to 3 (max)
    IEC0bits.T1IE=1;    //enable external interrupt
    T1CONbits.ON=1;     //enable timer
}

//ANDREW QUESTIONS
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
//END ANDREW QUESTIONS


void WaitTicks(uint64_t n) {
    uint64_t donetime=GetCoreTimer()+n; //set the tick value to count to
    while (GetCoreTimer()<donetime);    //count to the tick value
}

void WaitUS(uint64_t n) {
    //uint64_t donetime=GetCoreTimer()+n*(tps/1000000); //THIS DOESN'T WORK
    uint64_t donetime=GetCoreTimer()+(n*tps)/1000000;//TRY THIS
    while (GetCoreTimer()<donetime);                 //count to the tick value
}
void WaitMS(uint32_t n) {
    //uint64_t donetime=GetCoreTimer()+n*(tps/1000); //THIS DOESN'T WORK
    uint64_t donetime=GetCoreTimer()+(n*tps)/1000; //TRY THIS
    while (GetCoreTimer()<donetime);               //count to the tick value
}
void WaitS(uint32_t n) {
    uint64_t donetime=GetCoreTimer()+n*tps; //set the tick value to count to
    while (GetCoreTimer()<donetime);        //count to the tick value
}


void InitLoopDelay() {
    loopstarttime=GetCoreTimer(); //set the starting loop time
}

void DelayLoopMS(uint32_t n) {
    if (GetCoreTimer()>=loopstarttime+n*(tps/1000)) { //if the loop cycle took longer than it should have,
        yikes.looprate=1;                             //set looprate yikes flag
    }
    while (GetCoreTimer()<loopstarttime+n*(tps/1000));//delay until the target time has passed
    loopstarttime+=n*(tps/1000);                      //move loop time to next target time
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