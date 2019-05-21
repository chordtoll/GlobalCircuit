#include "Timing.h"
#include "Yikes.h"
#include "UART.h"
#include <proc/p32mx360f512l.h>

#define USPERR 1.0003
volatile uint8_t tmancount;

uint64_t loopstarttime;

void InitTiming() {
    //Timers
    T1CONbits.ON=0;
    T1CONbits.TCS=0;
    T1CONbits.TCKPS=3;

    T3CONbits.ON=0;      //Disable timer
    T3CONbits.TCS=0;     //Source: PBCLK
    T3CONbits.TCKPS=3;   //Prescale: 1/256
    TMR3=0;              //Clear timer register
    PR3=0xFFFF;          //Load period register
    IFS0bits.T3IF=0;     //clear timer interrupt flag
    IPC3bits.T3IP=7;     //set interrupt priority to 7 (max)
    IPC3bits.T3IS=3;     //set interrupt subpriority to 3 (max)
    IEC0bits.T3IE=1;     //enable external interrupt
    T3CONbits.ON=1;      //enable timer
    T2CONbits.ON = 0;    //Disable timer2
    T2CONbits.TCS = 0;   //set internal clock as source
    T2CONbits.T32 = 0;   //use a single 16-bit timer
    T2CONbits.TCKPS = 0; //1:1 prescalar
    PR2 = 799;           //set timer2 period to 10us
    T4CONbits.ON = 0;    //Disable timer4
    T4CONbits.TCS = 0;   //set internal clock as source
    T4CONbits.T32 = 1;   //combine with timer5 period for a 32 bit timer
    T4CONbits.TCKPS = 3; //1:256 prescalar
    PR4 = 0xC620;        //set period to 4 seconds
    PR5 = 0x025D;

    //PPS
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
void  __attribute__((vector(_TIMER_3_VECTOR), interrupt(IPL7SRS), nomips16)) TIMER3_ISR(void)
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
    IFS0bits.T3IF = 0; //clear interrupt flag status for Timer 1
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
    TMR2 = 0;                   //clear the timer2 counter
    int i;                      //looping variable
    n /= USPERR;                //adjust for timing error per microsecond
    T2CONSET = 0x8000;          //start timer2
    for(i = 0; i < n / 10; ++i) //count microseconds
    {
        while(!IFS0bits.T2IF){} //when 10 microseconds have passed
        IFS0bits.T2IF = 0;      //clear the period match flag
    }
    T2CONCLR = 0x8000;          //disable timer2
    IFS0bits.T2IF = 0;
}

void WaitMS(uint32_t n) {
    uint32_t i;
    uint64_t donetime=GetCoreTimer()+((n%70)*tps)/1000;
    while (GetCoreTimer()<donetime);               //count to the tick value
    for(i = 0; i < n/70; ++i)
    {
        uint64_t donetime=GetCoreTimer()+(70*tps)/1000;
        while(GetCoreTimer()<donetime);
    }
}

void WaitS(uint32_t n) {
    uint64_t donetime=GetCoreTimer()+n*tps; //set the tick value to count to
    while (GetCoreTimer()<donetime);        //count to the tick value
}

void StartKickTimer()
{
    TMR4 = 0;          //clear timer counters
    TMR5 = 0;
    KICKED = 0; //clear period match flag
    T4CONSET = 0x8000; //enable the timer
}

void ResetKickTimer()
{
    TMR4 = 0;          //clear timer counters
    TMR5 = 0;
    KICKED = 0; //clear period match flag
}

void StopKickTimer()
{
    T4CONCLR = 0x8000; //disable the timer
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

void SetClockPLL(uint8_t val)
{
    SYSKEY = 0xAA996655;       //unlock OSCCON register
    SYSKEY = 0x556699AA;
    OSCCONbits.NOSC = 0;       //set source to FRC
    OSCCONbits.OSWEN = 1;      //initiate source switch
    while(OSCCONbits.OSWEN){}  //wait for switch to complete
    OSCCONbits.PLLODIV = val;  //set PLL output divisor to new value
    OSCCONbits.NOSC = 0b011;   //set source to Primary Internal Oscillator with PLL
    OSCCONbits.OSWEN = 1;      //initiate source switch
    while(OSCCONbits.OSWEN){}  //wait for switch to complete
    SYSKEY = 0;                //lock OSCCON register
}

void Idle(uint16_t time)
{
    __builtin_disable_interrupts(); //disable interrupts
    PR1 = time * 120;               //set timer period to 100ms*time
    TMR1 = 0;                       //clear timer counter
    IFS0bits.T1IF = 0;              //clear timer match flag
    SetClockPLL(0b111);             //set PLL to 1:256
    T1CONSET = 0x8000;              //start the timer
    while(!IFS0bits.T1IF){}         //wait for period match
    T1CONCLR = 0x8000;              //stop the timer
    SetClockPLL(0);                 //set PLL to 1:1
    __builtin_enable_interrupts();  //enable interrupts
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