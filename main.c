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
#include "transmit.h"
#include "GPS.h"
#include "MS5607.h"
#include "MAG3310.h"
#include "Timing.h"

#define FSYS 80000000L      // Frequency of system clock, 80MHz


unsigned char SBDnormal[512] = {0};

char nTime[20];
char nLati[20];
char nLong[20];
char nAlti[20];
char nLatD;
char nLonD;

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

    InitLoopDelay();

    SendString_UART1("Init'd\n");

    //=============================//
    //          MAIN LOOP          //
    //=============================//

    while (1) {
        GPSready = 1;
        if (GPSnew) {
            GPSnew = 0;
            if (strncmp(GPSdata, "$GPGGA", 6) == 0) {
                ParseNMEA(GPSdata, nTime, nLati, &nLatD, nLong, &nLonD, nAlti);
                double dTime = atof(nTime);
                double dLati = atof(nLati);
                double dLong = atof(nLong);
                double dAlti = atof(nAlti);
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

                sprintf(SBDnormal, "Time:%10d Lat:%8d Lon:%8d Alt:%6d ",dTime,dLati,dLong,dAlti);
                SendString_UART1(SBDnormal);
                sprintf(SBDnormal, "X:%4x Y:%4x Z:%4x ",magX,magY,magZ);
                SendString_UART1(SBDnormal);
                sprintf(SBDnormal, "T:%6x P:%6x ",temperature,pressure);
                SendString_UART1(SBDnormal);
                sprintf(SBDnormal, "V+H:%6x V+L:%6x V-H:%6x V-L:%6x H+H:%6x H+L:%6x H-H:%6x H-L:%6x\n",VpH,VpL,VmH,VmL,HpH,HpL,HmH,HmL);
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
