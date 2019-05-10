#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_16
#pragma config FPLLODIV = DIV_1
#pragma config FNOSC = PRIPLL
#pragma config POSCMOD = XT
#pragma config FPBDIV = DIV_1
#pragma config DEBUG = ON
#pragma config WDTPS = PS8192  //8 second watchdog timeout
#pragma config FWDTEN = OFF

#include <proc/p32mx360f512l.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <sys/appio.h>
#include <stdint.h>
#include "GPIO.h"
#include "UART.h"
#include "GPS.h"
#include "MS5607.h"
#include "MAG3310.h"
#include "Timing.h"
#include "Packet.h"
#include "RockBlock.h"
#include "Yikes.h"
#include "SPI.h"
#include "ADC.h"
#include "Cutdown.h"

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

#define SEQUENCE_CYCLE 9

#define FSYS 80000000L

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

uint32_t gTime; //GPS time
uint32_t gLat;  //GPS latitude
uint32_t gLon;  //GPS longitude
uint32_t gAlt;  //GPS altitude

uint16_t cVert1[150]; //conductivity data 1
uint16_t cVert2[150]; //conductivity data 2

uint32_t statetimer; //timer for reading and sending correct values into the packet

uint16_t sequence;   //counter for alternating packet contents

packet_u packet;     //packet for storing measurements

int main(void) {
    //=============================//
    //       INITIALIZATION        //
    //=============================//
    yikes.byte=0;           //clear error flags
    yikes.reset=1;          //set reset flag

    uint8_t conductivityDone = 0;
    sequence=0;             //reset sequence counter
    statetimer=0;           //reset state counter

    InitGPIO();             //initialize GPIO

    InitUART();             //initialize UART

    InitGPS();              //initialize GPS
    InitRB();               //initialize RockBlock

    InitInterrupt();        //initialize interrupts

    InitI2C();              //initialize I2C
    InitMagneto(MAG_ADDR);  //initialize magnetometer with correct address
    InitAltimeter(ALT_ADDR);//intialize altimeter with correct address

    InitSPI1();             //initialize SPI 1

    InitPICADC();           //initialize ADC

    InitTiming();           //initialize timer for delays

    InitLoopDelay();        //initialize packet loop delays

    InitWatchdog();         //initialize watchdog timer

    GPSready=1;

    ChargeProbe(NONE);      //discharge probes

    clearPacket(&packet);   //Clear packet buffer

//#define TEST_LOOP //COMMENT FOR NORMAL RUNNING, UNCOMMENT FOR TESTING LOOP

#ifdef TEST_LOOP
//TEST CODE HERE
    while(1)
    {
        WakeGPS();
        //ResetWatchdog();
        WaitS(1);
        SleepGPS();
        //ResetWatchdog();
        while(1){}
        //Idle(50);
    }
#endif

#ifndef TEST_LOOP
    //SendChar_UART1('A');

    //=============================//
    //          MAIN LOOP          //
    //=============================//
    while (1) {
        TickRB();                                                      //Advance RockBlock state machine
        if (statetimer==0 && sequence > SEQUENCE_CYCLE)                //If we're at the start of a packet and supervision/conductivity data is available
        {
                Pack_Supervision(&packet, sequence);                   //pack supervision values into the packet
                Pack_Conductivity(&packet, sequence, cVert1, cVert2);  //pack conductivity values into the packet
        }
        if(sequence%(SEQUENCE_CYCLE+1) < SEQUENCE_CYCLE || conductivityDone) //if not on conductivity packet, or conductivity readings have been finished
        {
            switch(statetimer % T_FASTSAM_INTERVAL)                          //rotate readings based on statetimer (every 5 seconds)
            {
                uint16_t h1;                                                 //temporary horizontal probe variables
                uint16_t h2;
                uint16_t hD;
                uint16_t vert1;
                uint16_t vert2;
                uint16_t vertD;
                case 0:                                                      //if 0s into packet
                    TriggerMagneto(MAG_ADDR);                                //trigger the magnetometor for reading
                    break;
                case 1:                                                      //if 0.1s into packet
                    ;
                    uint16_t mx;                                             //temporary magnetometer variables
                    uint16_t my;
                    uint16_t mz;
                    ReadMagneto(MAG_ADDR, &mx,&my,&mz);                      //Read magnetometer values
                    Pack_Mag(&packet, (statetimer/T_FASTSAM_INTERVAL)%12, mx, my);       //store magnetometor values into packet
                    h1=ReadExtADC(2);                                        //read horizontal probe values
                    h2=ReadExtADC(5);
                    break;
                case 2:                                                      //if 0.2s into packet
                    hD=ReadExtADC(3);                                        //read horizontal differential value
                    Pack_Horiz(&packet, (statetimer/T_FASTSAM_INTERVAL)%12, h1, h2, hD); //store horizontal probe values into packet
                    break;
                case 3:
                    if(statetimer == 3)
                    {
                        vert1=ReadExtADC(0);
                        vert2=ReadExtADC(4);
                    }
                    break;
                case 4:
                    if(statetimer == 4)
                    {
                        vertD=ReadExtADC(1);
                    }
                    break;
                case 5:
                    if(statetimer == 5)
                    {
                        Pack_Vert(&packet, vert1, vert2, vertD);
                        ReadGPS(&gTime, &gLat, &gLon, &gAlt);
                        Pack_GPS(&packet, gTime, gLat, gLon, gAlt);
                    }
                    break;
                /*case 6:
                    if(!GPS_EN && _rb_state == RB_IDLE)
                    {
                        /*U1MODEbits.ON = 0;
                        U2MODEbits.ON = 0;
                        SPI1CONbits.ON = 0;
                        I2C1CONbits.ON = 0;
                        AD1CON1bits.ON = 0;
                        WDTCONbits.ON = 1;
                        //ResetWatchdog();
                        //Idle(T_FASTSAM_INTERVAL-(statetimer%T_FASTSAM_INTERVAL));
                        WaitMS((T_FASTSAM_INTERVAL-(statetimer%T_FASTSAM_INTERVAL))*100);
                        statetimer += T_FASTSAM_INTERVAL-(statetimer%T_FASTSAM_INTERVAL) - 1;
                        WDTCONbits.ON = 1;
                        U1MODEbits.ON = 1;
                        U2MODEbits.ON = 1;
                        SPI1CONbits.ON = 1;
                        I2C1CONbits.ON = 1;
                        AD1CON1bits.ON = 1;
                        break;
                     }*/
               }
        }
        else                                                       //if on conductivity packet
        {
            switch (statetimer)                                    //alternate readings based on statetimer
            {
                uint16_t vert1;                                    //temporary vertical probe variables
                uint16_t vert2;
                uint16_t vertD;
                case 0:                                            //if 0s into packet
                    TriggerAltimeter_Temperature(ALT_ADDR);        //trigger altimeter for temperature reading
                    supIl0=ReadPICADC(4);                          //store PICADC4 value
                    break;
                case 1:                                            //if 0.1s into packet
                    ReadAltimeter_ADC(ALT_ADDR, &supTemperature);  //read altimeter temperature
                    supIl1=ReadPICADC(8);                          //store PICADC8 value
                    break;
                case 2:                                            //if 0.2s into packet
                    TriggerAltimeter_Pressure(ALT_ADDR);           //trigger altimeter for pressure reading
                    supIl2=ReadPICADC(10);                         //store PICADC10 value
                    break;
                case 3:                                            //if 0.3s into packet
                    ReadAltimeter_ADC(ALT_ADDR, &supPressure);     //read altimeter pressure
                    supIh0=ReadPICADC(5);                          //store PICADC5 value
                    break;
                case 4:                                            //if 0.4s into packet
                    supIh1=ReadPICADC(9);                          //store PICADC9 value
                    break;
                case 5:                                            //if 0.5s into packet
                    supIh2=ReadPICADC(11);                         //store PICADC11 value
                    break;
                case 6:                                            //if 0.6s into packet
                    supT0=ReadPICADC(0);                           //store PICADC0 value
                    break;
                case 7:                                            //if 0.7s into packet
                    supT1=ReadPICADC(1);                           //store PICADC1 value
                    break;
                case 8:                                            //if 0.8s into packet
                    supT2=ReadPICADC(3);                           //store PICADC3 value
                    break;
                case 9:                                                    //if 0.9s into packet
                    vert1=ReadExtADC(0);                                   //read vertical probe values
                    vert2=ReadExtADC(4);
                    break;
                case 10:                                                   //if 1s into packet
                    vertD=ReadExtADC(1);                                   //read vertical differential value
                    break;
                case 11:                                                   //if 1.1s into packet
                    Pack_Vert(&packet, vert1, vert2, vertD);               //store vertical probe values into packet
                    ReadGPS(&gTime, &gLat, &gLon, &gAlt);                  //Read our GPS time and location
                    Pack_GPS(&packet, gTime, gLat, gLon, gAlt);            //store GPS data into packet
                    break;
                case T_CON_CHG_BEGIN:                              //if time to start conductivity charging (2s into packet)
                    ChargeProbe(GND);                              //charge probes to ground
                    break;
                case T_CON_CHG1_END:                               //if first charging cycle is complete (4s into packet)
                    ChargeProbe(UP);                               //charge probes up
                    break;
                case T_CON_CHG2_END:                               //if second charging cycle is complete (7s into packet)
                    ChargeProbe(NONE);                             //stop charging probes
                    conductivityDone = 1;                          //signal that conductivity measurements
                    break;
                }
            //While in conductivity measuring interval,
            if (statetimer>=T_CON_CHG_BEGIN && statetimer < T_CON_MEAS_END) {
                cVert1[statetimer-T_CON_CHG_BEGIN]=ReadExtADC(0); //Store vertical probe values over course of conductivity charging, 150 samples total
                cVert2[statetimer-T_CON_CHG_BEGIN]=ReadExtADC(4);
            }
        }
        ++statetimer;
        //If it's time to send a packet,
        if (statetimer>T_SLOWSAM_INTERVAL) {
            packet.norm.cutdown = GetCutdownStatus(); //update cutdown status
            packet.norm.ballast = GetBallastStatus(); //update ballast status
            packet.norm.version=PACKET_VERSION;       //Write version ID
            packet.norm.yikes=yikes.byte;             //Write error flags to packet
            yikes.byte=0;                             //Clear error flags
            packet.norm.seq=sequence;                 //Write sequence ID
            ++sequence;                               //update sequence counter
            SendString_RB(packet.bytes);              //Send packet
            clearPacket(&packet);                     //Clear packet buffer
            statetimer=0;                             //Reset state timer for start of next packet
            conductivityDone=0;                       //reset conductivity flag
        }
        ResetWatchdog();        //Clear watchdog timer
        DelayLoopMS(T_TICK_MS); //Delay to maintain constant tick rate
    }
#endif
    return 0;
}
