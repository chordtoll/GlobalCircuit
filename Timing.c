#include "Timing.h"
#include "Yikes.h"
#include "transmit.h"
#include <proc/p32mx360f512l.h>

#define USPERR 1.00019543
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
    T2CONbits.ON = 0;   //Disable timer2
    T2CONbits.TCS = 0;  //set internal clock as source
    T2CONbits.T32 = 0;  //use a single 16-bit timer
    T2CONbits.TCKPS = 0;//1:1 prescalar
    PR2 = 799;          //set timer2 period to 10us
    T4CONbits.ON = 0;   //Disable timer2
    T4CONbits.TCS = 0;  //set internal clock as source
    T4CONbits.T32 = 0;  //use a single 16-bit timer
    T4CONbits.TCKPS = 3;//1:256 prescalar
    PR2 = 0xFFFF;       //set timer2 period to 4.75s
}

char LT(uint64_t l, uint64_t r)
{
    uint32_t lH = l >> 32;          //split left and right 64 bit values into two 32 bit values
    uint32_t lL = (l << 32 ) >> 32;
    uint32_t rH = r >> 32;
    uint32_t rL = (r <<32 ) >> 32;
    if(lH < rH)
        return 1;
    else if(lH == rH && lL < rL)
        return 1;
    else
        return 0;
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
    uint64_t donetime=GetCoreTimer()+n; //set the tick value to count to
    while (GetCoreTimer()<donetime);    //count to the tick value
}

void WaitUS(uint32_t n) {
    TMR2 = 0;           //clear the timer2 counter
    int i;              //looping variable
    n /= USPERR;        //adjust for timing error per microsecond
    T2CONSET = 0x8000;  //start timer2
    for(i = 0; i < n / 10; ++i) //count microseconds
    {
        while(!IFS0bits.T2IF){} //when 10 microseconds have passed
        IFS0bits.T2IF = 0;      //clear the period match flag
    }
    T2CONCLR = 0x8000;          //disable timer2
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

uint8_t WaitForSignal(uint32_t period, uint32_t cycles, uint8_t goal, uint8_t index) {
    int i;
    for(i = 0; i < cycles; ++i)           //loop for number of cycles
    {                                     //if the signal matches goal
        if((index == 0 && PORTDbits.RD0 == goal) || (index == 1 && PORTDbits.RD1 == goal))
            return 1;                     //return success signal
        WaitUS(period);                   //wait for period microseconds
    }
    return 0;                             //return failure signal if goal was never met
}

void StartKickTimer()
{
    TMR4 = 0;
    IFS0bits.T4IF = 0;
    T4CONSET = 0x8000;
}

void ResetKickTimer()
{
    TMR4 = 0;
    IFS0bits.T4IF = 0;
}

void StopKickTimer()
{
    T4CONCLR = 0x8000;
}

void InitLoopDelay() {
    loopstarttime=GetCoreTimer(); //set the starting loop time
}

void DelayLoopMS(uint32_t n) {
    if (GetCoreTimer()>=loopstarttime+n*(tps/1000)) { //if the loop cycle took longer than it hould have,
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