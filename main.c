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
#include "SPI.h"

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
uint32_t gLat;
uint32_t gLon;
uint32_t gAlt;

uint16_t cVertH[150];
uint16_t cVertL[150];

uint32_t supTemperature=0;
uint32_t supPressure=0;
uint16_t supIl0=0;
uint16_t supIl1;
uint16_t supIl2;
uint16_t supIh0;
uint16_t supIh1;
uint16_t supIh2;
uint16_t supT0;
uint16_t supT1;
uint16_t supT2;

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
    statetimer=0;

    InitGPIO();

    InitUART();

    InitGPS();
    InitRB();

    InitInterrupt();

    InitI2C();
    InitMagneto(MAG_ADDR);
    InitAltimeter(ALT_ADDR);

    InitSPI1();
    InitSPI2();

    InitPICADC();

    InitLoopDelay();

    InitWatchdog();

    GPSready=1;



    //=============================//
    //          MAIN LOOP          //
    //=============================//
    while (1) {

        TickRB();   //Advance RockBlock state machine

        if (statetimer==0) { //If we're at the start of a packet,
            switch (sequence%10) {  //Send supervision data, rotated based on sequence number
                case 0:
                    packet.norm.sup._1u32=supTemperature;
                    break;
                case 1:
                    packet.norm.sup._1u32=supPressure;
                    break;
                case 2:
                    packet.norm.sup._2u16.a=supIl0;
                    packet.norm.sup._2u16.b=supIl1;
                    break;
                case 3:
                    packet.norm.sup._2u16.a=supIl2;
                    packet.norm.sup._2u16.b=supIh0;
                    break;
                case 4:
                    packet.norm.sup._2u16.a=supIh1;
                    packet.norm.sup._2u16.b=supIh2;
                    break;
                case 5:
                    packet.norm.sup._2u16.a=supT0;
                    packet.norm.sup._2u16.b=supT1;
                    break;
                case 6:
                    packet.norm.sup._2u16.a=supT2;
                    packet.norm.sup._2u16.b=0;
                    break;
            }

            //Copy slice of conductivity data into the packet.
            memcpy(packet.norm.cVertH,cVertH+(sequence%10)*15,15*2);
            memcpy(packet.norm.cVertL,cVertL+(sequence%10)*15,15*2);
        }

        //We perform normal voltage measurements for the first 9/10 intervals
        if (sequence%10<9) {
            //BEGIN every 5 seconds
            if (statetimer%T_FASTSAM_INTERVAL==0) { //Tick 0
                TriggerMagneto_S(); //Trigger the magnetometer to begin measurement
            }
            if (statetimer%T_FASTSAM_INTERVAL==1) { //Tick 1
                uint16_t mx;
                uint16_t my;
                uint16_t mz;
                ReadMagneto_S(&mx,&my,&mz); //Read magnetometer values
                packet.norm.compassX[(statetimer/T_FASTSAM_INTERVAL)%12]=mx; //Store magnetometer values in the packet
                packet.norm.compassY[(statetimer/T_FASTSAM_INTERVAL)%12]=my;
                uint16_t h1=ReadADC_S(2);
                uint16_t h2=ReadADC_S(5);
                packet.norm.horiz1[(statetimer/T_FASTSAM_INTERVAL)%12]=h1; //Store horizontal probe values in the packet
                packet.norm.horiz2[(statetimer/T_FASTSAM_INTERVAL)%12]=h2;
            }
            if (statetimer%T_FASTSAM_INTERVAL==2) { //Tick 2
                uint16_t hD=ReadADC_S(3);
                packet.norm.horizD[(statetimer/T_FASTSAM_INTERVAL)%12]=hD;  //Store horizontal differential value in the packet
            }
            //END every 5 seconds
            //BEGIN every 60 seconds
            if (statetimer==3) { //Tick 3 (Starts at 3 so as not to conflict with ^^^)
                packet.norm.vert1=ReadADC_S(0);  //Store vertical probe values in packet
                packet.norm.vert2=ReadADC_S(4);
            }
            if (statetimer==4) { //Tick 4
                packet.norm.vertD=ReadADC_S(1);  //Store vertical differential value in packet
            }
            if (statetimer==5) {
                ReadGPS_S(&gTime, &gLat, &gLon, &gAlt); //Read our GPS time and location
                packet.norm.time=gTime; //Store GPS time&location in packet
                packet.norm.lat=gLat;
                packet.norm.lon=gLon;
                packet.norm.alt=gAlt;
            }
        //We perform a conductivity measurement on packet 10/10
        } else {
            //Charge the probes in sequence
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
            if (statetimer==0) { //Tick 0
                packet.norm.vert1=ReadADC_S(0); //Store vertical probe values
                packet.norm.vert2=ReadADC_S(4); //(Before conductivity charging)
            }
            if (statetimer==1) {
                packet.norm.vertD=ReadADC_S(1); //Store vertical differential value
            }
            if (statetimer==2) {
                ReadGPS_S(&gTime, &gLat, &gLon, &gAlt); //Read our GPS time and location
                packet.norm.time=gTime; //Store GPS time&location in packet
                packet.norm.lat=gLat;
                packet.norm.lon=gLon;
                packet.norm.alt=gAlt;
            }
            //While in conductivity measuring interval,
            if (statetimer>=T_CON_CHG_BEGIN && statetimer<T_CON_MEAS_END) {
                cVertH[statetimer-T_CON_CHG_BEGIN]=ReadADC_S(0); //Store vertical probe values in temporary array
                cVertL[statetimer-T_CON_CHG_BEGIN]=ReadADC_S(4);
            }
            //Measure, store supervision values in temp variables
            switch (statetimer) {
                case 0:
                    TriggerAltimeter_Temperature_S();
                    supIl0=ReadPICADC(4);
                    break;
                case 1:
                    supTemperature=ReadAltimeter_S();
                    supIl1=ReadPICADC(8);
                    break;
                case 2:
                    TriggerAltimeter_Pressure_S();
                    supIl2=ReadPICADC(10);
                    break;
                case 3:
                    supPressure=ReadAltimeter_S();
                    supIh0=ReadPICADC(5);
                    break;
                case 4:
                    supIh1=ReadPICADC(9);
                    break;
                case 5:
                    supIh2=ReadPICADC(11);
                    break;
                case 6:
                    supT0=ReadPICADC(0);
                    break;
                case 7:
                    supT1=ReadPICADC(1);
                    break;
                case 8:
                    supT2=ReadPICADC(3);
                    break;
                }
        }
        statetimer++;
        //If it's time to send a packet,
        if (statetimer>T_SLOWSAM_INTERVAL) {
            packet.norm.version=PACKET_VERSION; //Write version ID
            packet.norm.yikes=yikes.byte; //Write error flags to packet
            yikes.byte=0; //Clear error flags
            packet.norm.seq=sequence; //Write sequence ID
            sequence++;
            RockSend_S(packet.bytes); //Send packet
            clearPacket(&packet); //Clear packet buffer
            statetimer=0; //Reset state timer for start of next packet
        }
        ResetWatchdog(); //Clear watchdog timer
        DelayLoopMS(T_TICK_MS); //Delay to maintain constant tick rate
    }

    return 0;
}
