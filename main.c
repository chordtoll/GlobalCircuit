#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_16
#pragma config FPLLODIV = DIV_1
#pragma config FNOSC = PRIPLL
#pragma config POSCMOD = XT
#pragma config FPBDIV = DIV_1
#pragma config DEBUG = ON

#include <proc/p32mx360f512l.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <sys/appio.h>
#include <stdint.h>
#include "transmit.h"
#include "GPS.h"
#include "MS5607.h"
#include "MAG3310.h"
#include "Timing.h"
#include "Resets.h"
#include "Stubs.h"
#include "Packet.h"

#define T_TICK_MS 100
#define T_SECOND (1000/T_TICK_MS)
#define T_MINUTE (T_SECOND*60)
#define T_NORM_LEN (T_MINUTE*9)
#define T_CON_LEN (T_MINUTE)
#define T_CON_CHGN_END  (T_SECOND*2)
#define T_CON_CHG1_END  (T_CON_CHGN_END+T_SECOND*2)
#define T_CON_MEAS1_END (T_CON_CHG1_END+T_SECOND*27)
#define T_CON_CHG2_END  (T_CON_MEAS1_END+T_SECOND*2)

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

int magX=0;
int magY=0;
int magZ=0;

int pressure;
int temperature;

int VpH=0x111111;
int VpL=0x222222;
int VmH=0x333333;
int VmL=0x444444;
int HpH=0x555555;
int HpL=0x666666;
int HmH=0x777777;
int HmL=0x888888;

uint32_t gTime;
int32_t gLat;
int32_t gLon;
uint32_t gAlt;

int picTemp;

typedef enum state {INIT,NORMAL,CONDUCTIVITY} state_t;

state_t state=INIT;
int statetimer;

char tempbuf[20]; //TODO: DELETE

packet_u packet;


void clearPacket(packet_u *pack) {
    int i;
    for (i=0;i<sizeof(*pack);i++)
        (*pack).bytes[i]=0;
}

void printPacket(packet_u pack) {
    int i;
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



int main(void) {
    //=============================//
    //       INITIALIZATION        //
    //=============================//

    InitGPIO();

    InitUART();

    InitGPS();
    InitInterrupt();
    InitI2C();
    InitMagneto(MAG_ADDR);
    InitAltimeter(ALT_ADDR);

    InitPICADC();

    InitADC_S();

    InitLoopDelay();

    SendString_UART1("Init'd\n");

    PrintResetReason();

    state=CONDUCTIVITY;

    GPSready=1;
    //=============================//
    //          MAIN LOOP          //
    //=============================//
    while (1) {
        //SendString_UART1(gpsbuf);
        //sprintf(tempbuf,"%2d:%02d.%03d %1d %1d",statetimer/T_MINUTE,(statetimer/T_SECOND)%60,(statetimer%T_SECOND)*T_TICK_MS,GPSready,GPSnew);
        //SendString_UART1(tempbuf);
        //if (state==NORMAL)
        //    SendString_UART1("NORMAL ");
        //if (state==CONDUCTIVITY)
        //    SendString_UART1("CONDUCTIVITY ");
        switch (state) {
            case NORMAL:
                packet.norm.type=0x55;
                if (statetimer%T_SLOWSAM_INTERVAL==0) {
                    clearPacket(&packet);
                }
                if (statetimer%T_FASTSAM_INTERVAL==0) {
                    SendChar_UART1('\n');
                    TriggerMagneto_S();
                    SampleADC_S(3);
                }
                if (statetimer%T_FASTSAM_INTERVAL==1) {
                    int mx;
                    int my;
                    int mz;
                    ReadMagneto_S(&mx,&my,&mz);
                    packet.norm.compassX[(statetimer/T_FASTSAM_INTERVAL)%12]=mx;
                    packet.norm.compassY[(statetimer/T_FASTSAM_INTERVAL)%12]=my;
                    packet.norm.horizL[(statetimer/T_FASTSAM_INTERVAL)%12]=ReadADC_S(3);
                }
                if (statetimer%T_FASTSAM_INTERVAL==2) {
                    SampleADC_S(4);
                }
                if (statetimer%T_FASTSAM_INTERVAL==3) {
                    packet.norm.horizR[(statetimer/T_FASTSAM_INTERVAL)%12]=ReadADC_S(4);
                }
                if (statetimer%T_FASTSAM_INTERVAL==4) {
                    SampleADC_S(5);
                }
                if (statetimer%T_FASTSAM_INTERVAL==5) {
                    packet.norm.horizD[(statetimer/T_FASTSAM_INTERVAL)%12]=ReadADC_S(5);
                }
                if (statetimer%T_SLOWSAM_INTERVAL==6) {
                    SampleADC_S(0);
                }
                if (statetimer%T_SLOWSAM_INTERVAL==7) {
                    packet.norm.vertH=ReadADC_S(0);
                }
                if (statetimer%T_SLOWSAM_INTERVAL==8) {
                    SampleADC_S(1);
                }
                if (statetimer%T_SLOWSAM_INTERVAL==9) {
                    packet.norm.vertL=ReadADC_S(1);
                }
                if (statetimer%T_SLOWSAM_INTERVAL==10) {
                    SampleADC_S(2);
                }
                if (statetimer%T_SLOWSAM_INTERVAL==11) {
                    packet.norm.vertD=ReadADC_S(2);
                }
                if (statetimer%T_SLOWSAM_INTERVAL==12) {
                    ReadGPS_S(&gTime, &gLat, &gLon, &gAlt);
                    packet.norm.time=gTime;
                    packet.norm.lat=gLat;
                    packet.norm.lon=gLon;
                    packet.norm.alt=gAlt;
                }
                if (statetimer%T_SLOWSAM_INTERVAL==T_SECOND*59) {
                    printPacket(packet);
                }
                break;
            case CONDUCTIVITY:
                packet.rare.type=0xAA;
              switch (statetimer) {
                    case 0:
                        SendChar_UART1('\n');
                        ChargeProbe_S(GND);
                        break;
                    case T_CON_CHGN_END:
                        SendChar_UART1('\n');
                        ChargeProbe_S(UP);
                        break;
                    case T_CON_CHG1_END:
                        SendChar_UART1('\n');
                        ChargeProbe_S(NONE);
                        break;
                    case T_CON_MEAS1_END:
                        SendChar_UART1('\n');
                        ChargeProbe_S(DOWN);
                        break;
                    case T_CON_CHG2_END:
                        SendChar_UART1('\n');
                        ChargeProbe_S(NONE);
                        break;
                }
                if (statetimer%T_SECOND==1)
                    SampleADC_S(0);
                if (statetimer%T_SECOND==2)
                    packet.rare.vertD[statetimer/T_SECOND]=ReadADC_S(0);

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
                    printPacket(packet);
                    clearPacket(&packet);
                    state=NORMAL;
                    statetimer=0;
                }
                break;

        }
        //SendString_UART1("\n");
        DelayLoopMS(T_TICK_MS);
    }

    while (1) {
        GPSready = 1;
        if (GPSnew) {
            GPSnew = 0;
            if (strncmp(GPSdata, "$GPGGA", 6) == 0) {
                ParseNMEA(GPSdata, nTime, nLati, &nLatD, nLong, &nLonD, nAlti);
                dTime = atof(nTime);
                dLati = atof(nLati);
                dLong = atof(nLong);
                dAlti = atof(nAlti);
                ReadMagneto(MAG_ADDR,&magX,&magY,&magZ);
                TriggerMagneto(MAG_ADDR);
                
                WaitMS(100);
                TriggerAltimeter_Pressure(ALT_ADDR);
                WaitMS(100);
                ReadAltimeter_ADC(ALT_ADDR, &pressure);
                WaitMS(100);
                TriggerAltimeter_Temperature(ALT_ADDR);
                WaitMS(100);
                ReadAltimeter_ADC(ALT_ADDR, &temperature);

                //ADC CODE
                //ReadADC(&VpH,&VpL,&VmH,&VmL,&HpH,&HpL,&HmH,&HmL);
                //ADC CODE

                picTemp=ReadPICADC();

                sprintf(SBDnormal, "T:%10d L:%8d L:%8d A:%6d ",dTime,dLati,dLong,dAlti);
                SendString_UART1(SBDnormal);
                sprintf(SBDnormal, "X:%4x Y:%4x Z:%4x ",magX,magY,magZ);
                SendString_UART1(SBDnormal);
                sprintf(SBDnormal, "T:%6x P:%6x ",temperature,pressure);
                SendString_UART1(SBDnormal);
                sprintf(SBDnormal, "V+H:%6x V+L:%6x V-H:%6x V-L:%6x H+H:%6x H+L:%6x H-H:%6x H-L:%6x ",VpH,VpL,VmH,VmL,HpH,HpL,HmH,HmL);
                SendString_UART1(SBDnormal);
                sprintf(SBDnormal, "A:%3x\n",picTemp);
                SendString_UART1(SBDnormal);
                /*
                 *
                 * RockBlock code here- transmit SBDnormal
                 *
                 */
                //HackRockSend(SBDnormal);
                //HackBusyWait(100);
            }
        }
    }

    return 0;
}
