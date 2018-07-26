#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_16
#pragma config FPLLODIV = DIV_1
#pragma config FNOSC = PRIPLL
#pragma config POSCMOD = XT
#pragma config FPBDIV = DIV_1
#pragma config DEBUG = ON

// pragma directives for the pic32mx460
//#pragma config FNOSC = PRIPLL
//#pragma config POSCMOD = EC
//#pragma config FPLLIDIV = DIV_2
//#pragma config FPLLMUL = MUL_20
//#pragma config FPLLODIV = DIV_1
//#pragma config FPBDIV = DIV_8

#include <proc/p32mx360f512l.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <sys/appio.h>
#include "transmit.h"
#include "assert.h"
#include "ADCConfig.h"
#include "GPS.h"
#include "MS5607.h"
#include "MAG3310.h"
#include "Timing.h"

#define ADC_ADDRESS 0b1001000
#define MAG_ADDRESS 0x1E
#define FSYS 80000000L      // Frequency of system clock, 80MHz
#define CS LATDbits.LATD9

// Used for ADC calculations with ADS1118
#define FS (2.048) // Full Scale
#define bit15 (32767U) //2^15 -1
#define bit16 (65535U)
#define Mul_Factor (FS * 2 / bit16)

int result = 0;
char SendSMS(char* packet);
char CheckString(char* message);

// Global memory locations for each part of the packet
// We will take in the data from the various sensors and store them in each of these
// character arrays and then concatenate them into a character array that will
// be the packet

// Note: We have to use character arrays becuase of how the transmission of the
// rockblock is set up
char TIME[10]; // = "aaaaaaaaa";        // Time from GPS UPDATED, size 10
char LATI[10]; // = "aaaaaaaaa";      // Latitude from GPS UPDATED size 10
char LONG[11]; // = "aaaaaaaaaa";     // Longitude from GPS UPDATED size 11
char ALT[6]; // = "aaaaa";        // Altitude from GPS UPDATED size 6
char pr[2] = "RS"; // Pressure from pressure sensor
char AT[2] = "TU"; // Air temperature
char t[1] = "V"; // Internal temperature of PIC
char B[1] = "W"; // Battery temperature of PIC
char bv[2] = "X"; // V monitor from PIC ADC
char I[1] = "Y"; // I monitor from PIC ADC
char dt[2] = "Z"; // sigma plus/minus (last) (langmuir)
char Aa[66] = {"a"}; // Bearing, magnetometer
char Bb[120] = {"b"}; // langmuir vertical
char Cc[120] = {"c"}; // langmuir horizontal
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

    //HackBusyWait(100);

    AD1PCFG = 0x0000; // Analog
    initADC();
    
    int i, j;
    char packet[340] = {0}; // Packet that wil be sent over the rockblock
    double coretimer; // Used to calculate the CPU time
    char RUN[3]; // Array that will hold the time since power on
    int count = 0;
    int count1 = 0;
    int GPScount = 0;
    //char GPSdata[80]={0};
    int first = 1;
    int loopCounter = 0;
    float x, v;
    
    InitIO();

    InitUART(); // Initializes UART
    InitGPS();
    InitInterrupt(); // Initializes interrupts

    InitI2C();
    InitMagneto(MAG_ADDR);
    InitAltimeter(ALT_ADDR);
    //RockInit();      // Initializes the rockblock modem

    SendString_UART1("Init'd\n");

    InitLoopDelay();


    while (1) { //Main loop
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
