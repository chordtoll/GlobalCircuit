#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_16
#pragma config FPLLODIV = DIV_1
#pragma config FNOSC = PRIPLL
#pragma config POSCMOD = XT
#pragma config FPBDIV = DIV_1
#pragma config DEBUG = ON
#pragma config WDTPS = PS1024
#pragma config FWDTEN = OFF

#include <proc/p32mx360f512l.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <sys/appio.h>
#include <stdint.h>
#include "GPIODebug.h"
#include "transmit.h"
#include "GPS.h"
#include "MS5607.h"
#include "MAG3310.h"
#include "Timing.h"
#include "Resets.h"
#include "Stubs.h"
#include "Packet.h"
#include "RockBlock.h"
#include "Yikes.h"

#define T_TICK_MS 100
#define T_SECOND (1000/T_TICK_MS)
#define T_MINUTE (T_SECOND*60)
#define T_NORM_LEN (T_MINUTE*9)
#define T_CON_LEN (T_MINUTE)
#define T_CON_CHG_BEGIN  (T_SECOND*2)
#define T_CON_CHG1_END  (T_CON_CHG_BEGIN+T_SECOND*2)
#define T_CON_CHG2_END  (T_CON_CHG1_END+T_SECOND*3)
#define T_CON_MEAS_END (T_CON_CHG2_END+T_SECOND*10)

#define T_FASTSAM_INTERVAL (T_SECOND*5)
#define T_SLOWSAM_INTERVAL (T_FASTSAM_INTERVAL*12)

#define FSYS 80000000L      // Frequency of system clock, 80MHz


unsigned char SBDnormal[512] = {0};

char nTime[20];
char nLati[20];
char nLong[20];
char nAlti[20];
char nLatD;
char nLonD;

double dTime;
double dLati;
double dLong;
double dAlti;

uint16_t magX=0;
uint16_t magY=0;
uint16_t magZ=0;

uint32_t pressure;
uint32_t temperature;

uint32_t gTime;
int32_t gLat;
int32_t gLon;
uint32_t gAlt;

uint16_t cVertH[150];
uint16_t cVertL[150];

typedef enum state {NORMAL,CONDUCTIVITY} state_t;

state_t state;
uint32_t statetimer;

uint16_t sequence;

packet_u packet;


void clearPacket(packet_u *pack) {
    uint16_t i;
    for (i=0;i<sizeof(*pack);i++)
        (*pack).bytes[i]=0;
}

void printPacket(packet_u pack) {
    uint16_t i;
    SendChar_UART1('\n');
    for (i=0;i<sizeof(pack);i++) {
        if ((pack.bytes[i]&0xF)>9)
            SendChar_UART1((pack.bytes[i]&0xF)+'A'-10);
        else if ((pack.bytes[i]&0xF)>0)
            SendChar_UART1((pack.bytes[i]&0xF)+'0');
        else
            SendChar_UART1('.');

        if (((pack.bytes[i]>>4)&0xF)>9)
            SendChar_UART1(((pack.bytes[i]>>4)&0xF)+'A'-10);
        else if (((pack.bytes[i]>>4)&0xF)>0)
            SendChar_UART1(((pack.bytes[i]>>4)&0xF)+'0');
        else
            SendChar_UART1('.');
    }
    SendChar_UART1('\n');
}

uint32_t i,j;

int main(void) {
    //=============================//
    //       INITIALIZATION        //
    //=============================//
    yikes.byte=0;
    yikes.reset=1;

    sequence=0;

    InitGPIO();

    InitUART();

    InitGPS();
    InitRB();

    InitInterrupt();

    InitI2C();
    InitMagneto(MAG_ADDR);
    InitAltimeter(ALT_ADDR);

    InitPICADC();

    InitADC_S();

    InitLoopDelay();

    InitWatchdog();

    state=CONDUCTIVITY;

    GPSready=1;



    //=============================//
    //          MAIN LOOP          //
    //=============================//
    while (1) {

        TickRB();
        
        switch (state) {
            case NORMAL:
                if (statetimer%T_SLOWSAM_INTERVAL==0) {
                    if ((statetimer/T_MINUTE)<6) {
                        memcpy(packet.norm.cVertH,cVertH+(statetimer/T_MINUTE)*25,25*2);
                        memcpy(packet.norm.cVertL,cVertL+(statetimer/T_MINUTE)*25,25*2);
                    }
                }
                if (statetimer%T_FASTSAM_INTERVAL==0) {
                    TriggerMagneto_S();
                }
                if (statetimer%T_FASTSAM_INTERVAL==1) {
                    uint16_t mx;
                    uint16_t my;
                    uint16_t mz;
                    ReadMagneto_S(&mx,&my,&mz);
                    packet.norm.compassX[(statetimer/T_FASTSAM_INTERVAL)%12]=mx;
                    packet.norm.compassY[(statetimer/T_FASTSAM_INTERVAL)%12]=my;
                    packet.norm.horizL[(statetimer/T_FASTSAM_INTERVAL)%12]=ReadADC_S(2);
                    packet.norm.horizR[(statetimer/T_FASTSAM_INTERVAL)%12]=ReadADC_S(5);
                }
                if (statetimer%T_FASTSAM_INTERVAL==2) {
                    packet.norm.horizD[(statetimer/T_FASTSAM_INTERVAL)%12]=ReadADC_S(3);
                }
                if (statetimer%T_SLOWSAM_INTERVAL==3) {
                    packet.norm.vertH=ReadADC_S(0);
                    packet.norm.vertL=ReadADC_S(4);
                }
                if (statetimer%T_SLOWSAM_INTERVAL==4) {
                    packet.norm.vertD=ReadADC_S(1);
                }
                if (statetimer%T_SLOWSAM_INTERVAL==5) {
                    ReadGPS_S(&gTime, &gLat, &gLon, &gAlt);
                    packet.norm.time=gTime;
                    packet.norm.lat=gLat;
                    packet.norm.lon=gLon;
                    packet.norm.alt=gAlt;
                }
                if (statetimer%T_SLOWSAM_INTERVAL==T_SECOND*59) {
                    packet.norm.type=0x55;
                    packet.norm.yikes=yikes.byte;
                    yikes.byte=0;
                    packet.norm.seq=sequence;
                    sequence++;
                    RockSend_S(packet.bytes);
                    clearPacket(&packet);
                }
                break;
            case CONDUCTIVITY:
                packet.rare.type=0xAA;
                switch (statetimer) {
                    case T_CON_CHG_BEGIN:
                        ChargeProbe_S(GND);
                        break;
                    case T_CON_CHG1_END:
                        ChargeProbe_S(UP);
                        break;
                    case T_CON_CHG2_END:
                        ChargeProbe_S(NONE);
                        break;
                }
                if (statetimer==0) {
                    packet.rare.vertH=ReadADC_S(0);
                    packet.rare.vertL=ReadADC_S(4);
                }
                if (statetimer==1) {
                    packet.rare.vertD=ReadADC_S(1);
                }
                if (statetimer>=T_CON_CHG_BEGIN && statetimer<T_CON_MEAS_END) {
                    cVertH[statetimer-T_CON_CHG_BEGIN]=ReadADC_S(0);
                    cVertL[statetimer-T_CON_CHG_BEGIN]=ReadADC_S(4);
                }
                if (statetimer==0)
                    TriggerAltimeter_Temperature_S();
                if (statetimer==1)
                    packet.rare.temperature=ReadAltimeter_S();
                if (statetimer==2)
                    TriggerAltimeter_Pressure_S();
                if (statetimer==3)
                    packet.rare.pressure=ReadAltimeter_S();
                if (statetimer==4)
                    packet.rare.thermval=ReadPICADC_S(0);
                if (statetimer==5)
                    packet.rare.batvolt=ReadPICADC_S(1);
                if (statetimer==6)
                    packet.rare.batcurr=ReadPICADC_S(2);
                break;
        }
        statetimer++;
        switch (state) {
            case NORMAL:
                if (statetimer>T_NORM_LEN) {
                    state=CONDUCTIVITY;
                    statetimer=0;
                }
                break;
            case CONDUCTIVITY:
                if (statetimer>T_CON_LEN) {
                    packet.rare.type=0xAA;
                    packet.rare.yikes=yikes.byte;
                    yikes.byte=0;
                    packet.rare.seq=sequence;
                    sequence++;
                    RockSend_S(packet.bytes);
                    clearPacket(&packet);
                    state=NORMAL;
                    statetimer=0;
                }
                break;

        }
        ResetWatchdog();
        i++;
        DelayLoopMS(T_TICK_MS);
    }

    return 0;
}
