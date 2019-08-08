#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_16
#pragma config FPLLODIV = DIV_1
#pragma config FCKSM = 1
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
#include "ballast.h"

#define T_TICK_MS 100                                 //number of milliseconds per tick
#define T_SECOND (1000/T_TICK_MS)                     //ticks per second
#define T_MINUTE (T_SECOND*60)                        //ticks per minute
#define T_NORM_LEN (T_MINUTE*9)                       //ticks where normal measurements are taken
#define T_CON_CHG_BEGIN  (T_SECOND*15)                //ticks to begin probe charging
#define T_CON_CHG1_END  (T_CON_CHG_BEGIN+T_SECOND*1)  //ticks to finish first charging stage
#define T_CON_CHG2_END  (T_CON_CHG1_END+T_SECOND*1)   //ticks to finish second charging stage
#define T_CON_MEAS_END (T_CON_CHG2_END+T_SECOND*13)   //ticks to finishing conductivity measurements
#define T_CON_LEN (T_CON_MEAS_END - T_CON_CHG_BEGIN)  //ticks where conductivity measurements are taken


#define T_FASTSAM_INTERVAL (T_SECOND*5)               //ticks in one interval of normal measurements
#define T_SLOWSAM_INTERVAL (T_FASTSAM_INTERVAL*12)    //ticks per packet

#define SEQUENCE_CYCLE 9                              //number of normal packet measurements per cycle

#define FSYS 80000000L

int main(void) {
    //=============================//
    //       INITIALIZATION        //
    //=============================//
    uint16_t cVert1[150];       //conductivity data 1
    uint16_t cVert2[150];       //conductivity data 2

    uint32_t gTime=0;             //GPS data
    uint32_t gCondTime=0xDEAD;
    uint32_t gLat=0;
    uint32_t gLon=0;
    uint32_t gAlt=0;
    uint8_t gSats=0;

    uint8_t conductivityDone=0; //flag indicating state of conductivity measurements
    uint16_t conductivityDir=0; //direction of probe charging for conductivity measurements, odd=up, even=down

    uint16_t sequence=0;        //packet number

    uint32_t statetimer=0;      //packet tick

    packet_u packet;            //packet for storing measurements

    yikes.byte=0;               //clear error flags
    yikes.reset=1;              //set reset flag

    InitUART();                 //initialize UART
    InitInterrupt();            //initialize interrupts

    InitGPIO();                 //initialize GPIO

    InitTiming();               //initialize timer for delays

    InitGPS();                  //initialize GPS
    InitRB();                   //initialize RockBlock

    InitI2C();                  //initialize I2C
    InitMagneto(MAG_ADDR);      //initialize magnetometer
    InitAltimeter(ALT_ADDR);    //intialize altimeter

    InitSPI1();                 //initialize SPI 1

    InitPICADC();               //initialize ADC

    InitLoopDelay();            //initialize packet loop delays

    InitWatchdog();             //initialize watchdog timer

    ChargeProbe(NONE);      //discharge probes

    clearPacket(&packet);   //Clear packet buffer

//#define TEST_LOOP //COMMENT FOR NORMAL RUNNING, UNCOMMENT FOR TESTING LOOP

#ifdef TEST_LOOP
//TEST CODE HERE
    while(1)
    {
        SafeDebugString("YEP");
        WaitS(5);
        ResetWatchdog();
    }
#endif

#ifndef TEST_LOOP

    //=============================//
    //          MAIN LOOP          //
    //=============================//
    while (1) {
        TickRB();                                                      //Advance RockBlock state machine
        if(sequence%(SEQUENCE_CYCLE+1) || statetimer < T_CON_CHG_BEGIN || conductivityDone)            //if not on conductivity packet, or conductivity readings have been finished
        {
            switch(statetimer % T_FASTSAM_INTERVAL)                          //rotate readings based on statetimer (every 5 seconds)
            {
                uint16_t h1;                                                 //temporary horizontal probe variables
                uint16_t h2;
                uint16_t hD;
                uint16_t vert1;                                              //temporary vertical probe variables
                uint16_t vert2;
                uint16_t vertD;
                case 0:                                                      //if 0s into interval
                    TriggerMagneto(MAG_ADDR);                                //trigger the magnetometor for reading
                    if(statetimer == T_SECOND*45)
                        CheckSig_RB();
                    break;
                case 1:                                                      //if 0.1s into interval
                    ;
                    uint16_t mx;                                             //temporary magnetometer variables
                    uint16_t my;
                    uint16_t mz;
                    ReadMagneto(MAG_ADDR, &mx,&my,&mz);                      //Read magnetometer values
                    Pack_Mag(&packet, (statetimer/T_FASTSAM_INTERVAL)%12, mx, my, mz);       //store magnetometor values into packet
                    h1=ReadExtADC(2);                                        //read horizontal probe values
                    h2=ReadExtADC(5);
                    break;
                case 2:                                                      //if 0.2s into interval
                    hD=ReadExtADC(3);                                        //read horizontal differential value
                    Pack_Horiz(&packet, (statetimer/T_FASTSAM_INTERVAL)%12, h1, h2, hD); //store horizontal probe values into packet
                    break;
                case 3:
                        vert1=ReadExtADC(0);                                 //read vertical probe values
                        vert2=ReadExtADC(4);
                    break;
                case 4:
                        vertD=ReadExtADC(1);                                 //read vertical differential value
                        Pack_Vert(&packet, (statetimer/T_FASTSAM_INTERVAL)%12, vert1, vert2, vertD);             //store vertical values into packet

                    break;
                case 5:
                    if(statetimer == T_SECOND*35 + 5)                                   //if 35.5s into packet
                    {
                        ReadGPS(&gTime, &gLat, &gLon, &gAlt, &gSats);                   //read GPS values
                        Pack_GPS(&packet, gTime, gCondTime, gLat, gLon, gAlt, gSats);   //store GPS values into packet
                    }
                    break;
                case 6:                                                //if 35.6s into packet
                    if(statetimer == T_SECOND*35 + 6)
                    {
                        TriggerAltimeter_Temperature(ALT_ADDR);        //trigger altimeter for temperature reading
                        supVbatP=ReadPICADC(4);                        //store positive battery voltage value
                    }
                    break;
                case 7:                                                //if 35.7s into packet
                    if(statetimer == T_SECOND*35 + 7)
                    {
                        ReadAltimeter_ADC(ALT_ADDR, &supTemperature);  //read altimeter temperature
                        supTemperature = ConvertAltimeter_Temp(supTemperature);
                        supVbatN=ReadPICADC(8);                        //store negative battery voltage value
                    }
                    break;
                case 8:                                             //if 35.8s into packet
                    if(statetimer == T_SECOND*35 + 8)
                    {
                        TriggerAltimeter_Pressure(ALT_ADDR);        //trigger altimeter for pressure reading
                        supVbat3V6=ReadPICADC(10);                  //store 3.6V battery voltage value
                    }
                    break;
                case 9:                                             //if 35.9s into packet
                    if(statetimer == T_SECOND*35 + 9)
                    {
                        ReadAltimeter_ADC(ALT_ADDR, &supPressure);  //read altimeter pressure
                        supPressure = ConvertAltimeter_Pressure(supPressure);
                    }
                    break;
                case 10:                                            //if 36.0s into packet
                    if(statetimer == T_SECOND*35 + 10)
                    {
                        supI1=ReadPICADC(5);                        //store current1 value
                        supI2=ReadPICADC(9);                        //store current2 value
                    }
                    break;
                case 11:                                            //if 36.1s into packet
                    if(statetimer == T_SECOND*35 + 11)
                    {
                        supT1=ReadPICADC(0);                        //store temperature1 value
                        supT2=ReadPICADC(1);                        //store temperature2 value
                    }
                    break;
                case 12:                                            //if 36.2s into packet
                    if(statetimer == T_SECOND*35 + 12)
                    {
                        supText = ReadPICADC(12);                   //store external termperature value
                        supTRB = ReadPICADC(13);                    //store rockblock temperature value
                    }
                    break;
                case 13:                                            //if 36.3s into packet
                    if(statetimer == T_SECOND*35 + 13)
                    {
                        ReadMagTemp(MAG_ADDR, &supTmag);            //store magnetometer temperature
                        supTadc2=ReadExtADCTemp(1);
                    }
                    break;
                case 14:                                            //if 36.4s into packet
                    if(statetimer == T_SECOND*35 + 14)
                    {
                        supTadc1=ReadExtADCTemp(0);
                    }
                    break;
                }
        }
        else                                                       //if on conductivity packet
        {
            switch (statetimer)                                    //alternate readings based on statetimer
            {
                case T_CON_CHG_BEGIN:                              //if time to start conductivity charging (15s into packet)
                    gCondTime = gTime;
                    ChargeProbe(GND);                              //charge probes to ground
                    break;
                case T_CON_CHG1_END:                               //if first charging cycle is complete (16s into packet)
                    if(conductivityDir++ % 2)                      //if on an odd interval,
                        ChargeProbe(DOWN);                         //charge probes down
                    else                                           //if on an even interval,
                        ChargeProbe(UP);                           //charge probes up
                    break;
                case T_CON_CHG2_END:                               //if second charging cycle is complete (17s into packet)
                    ChargeProbe(NONE);                             //stop charging probes
                    ReadGPS(&gTime, &gLat, &gLon, &gAlt, &gSats);                   //read GPS values
                    Pack_GPS(&packet, gTime, gCondTime, gLat, gLon, gAlt, gSats);   //store GPS values into packet
                    break;
                }
            //While in conductivity measuring interval,
            if (statetimer>=T_CON_CHG_BEGIN && statetimer < T_CON_MEAS_END) {
                cVert1[statetimer-T_CON_CHG_BEGIN]=ReadExtADC(0); //Store vertical probe values over course of conductivity charging, 150 samples total
                cVert2[statetimer-T_CON_CHG_BEGIN]=ReadExtADC(4);
            }
            else if(statetimer == T_CON_MEAS_END)
                conductivityDone = 1;
        }
        ++statetimer;
        //If it's time to send a packet,
        if (statetimer>T_SLOWSAM_INTERVAL) {
            Pack_Supervision(&packet);                             //pack supervision values into the packet
            Pack_Conductivity(&packet, sequence, cVert1, cVert2);  //pack conductivity values into the packet
            packet.norm.RBSig = _rb_sig;
            packet.norm.cutdown = GetCutdownStatus();              //update cutdown status
            packet.norm.ballast = GetBallastStatus();              //update ballast status
            packet.norm.version=(PAYLOAD_ID<<4) | PACKET_VERSION;  //Write version ID
            packet.norm.epochdays=DAYS_SINCE_EPOCH;                //write days since epoch
            yikes.condpol = (conductivityDir-1)%2;
            packet.norm.yikes=yikes.byte;                          //Write error flags to packet
            yikes.byte=0;                                          //Clear error flags
            packet.norm.seq=sequence;                              //Write sequence ID
            ++sequence;                                            //update sequence counter
            SendString_RB(packet.bytes);                           //Send packet
            clearPacket(&packet);                                  //Clear packet buffer
            statetimer=0;                                          //Reset state timer for start of next packet
            conductivityDone=0;                                    //reset conductivity flag
        }
        ResetWatchdog();        //Clear watchdog timer
        DelayLoopMS(T_TICK_MS); //Delay to maintain constant tick rate
    }
#endif
    return 0;
}
