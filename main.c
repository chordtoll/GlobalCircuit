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
#include "transmit.h"
#include "assert.h"
#include "TimerFunctions.h"
#include <sys/appio.h>
#include "ADCConfig.h"
#include "SPIConfig.h"
#include "ADS1118.h"
#include "GPS.h"
#include "conversion.h"
#include "MS5607.h"
#include "MAG3310.h"

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
double read_timer(void);    // Function to read core timer

// Global memory locations for each part of the packet
// We will take in the data from the various sensors and store them in each of these
// character arrays and then concatenate them into a character array that will
// be the packet

// Note: We have to use character arrays becuase of how the transmission of the
// rockblock is set up
char TIME[10];// = "aaaaaaaaa";        // Time from GPS UPDATED, size 10
char LATI[10];// = "aaaaaaaaa";      // Latitude from GPS UPDATED size 10
char LONG[11];// = "aaaaaaaaaa";     // Longitude from GPS UPDATED size 11
char ALT[6];// = "aaaaa";        // Altitude from GPS UPDATED size 6
char pr[2] = "RS";          // Pressure from pressure sensor
char AT[2] = "TU";          // Air temperature
char t[1] = "V";            // Internal temperature of PIC
char B[1] = "W";            // Battery temperature of PIC
char bv[2] = "X";           // V monitor from PIC ADC
char I[1] = "Y";            // I monitor from PIC ADC
char dt[2] = "Z";           // sigma plus/minus (last) (langmuir)
char Aa[66] = {"a"};        // Bearing, magnetometer
char Bb[120] = {"b"};       // langmuir vertical
char Cc[120] = {"c"};       // langmuir horizontal
unsigned char SBDnormal[512] = {0};

char nTime[20];
char nLati[20];
char nLong[20];
char nAlti[20];

signed short mx;
signed short my;
signed short mz;
unsigned int pres;
int main(void)
{

    //HackBusyWait(100);

    AD1PCFG = 0x0000;      // Analog
    // These are the SPI pins on the PIC. The current are for the PIC32MX460 and
    // will have to be changed to the spins used for the PIC32MX360
    TRISDbits.TRISD10 = 0; // SCK is output RD10
    TRISCbits.TRISC4 = 1;  // SDI is input RC04
    TRISDbits.TRISD0 = 0;  // SDO is output RD00
    TRISDbits.TRISD9 = 0;  // CHIP SELECT IS O/P output RD09

    initADC();
    initSPI();
    int i, j;
    char packet[340] = {0}; // Packet that wil be sent over the rockblock
    double coretimer; // Used to calculate the CPU time
    char RUN[3]; // Array that will hold the time since power on
    int count = 0;
    int count1 = 0;
    int GPScount = 0;
    char GPSdata[80]={0};
    int first = 1;
    int loopCounter = 0;
    float x, v;
    InterruptInit(); // Initializes interrupts
    UARTInit();      // Initializes UART
    //RockInit();      // Initializes the rockblock modem



    char receivedChar;
    char n[50];
    I2cConfig();
    mag_reset(MAG_ADDR);
    ///////////////////////////////GPS//////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // Transmits to the GPS to tell it to only print two NMEA strings instead of four
    U2TXREG = (0xA0); //skytraq start of sequence: 0xA0A1
    while (U2STAbits.TRMT == 0); // Waiting for transmission to complete
    U2TXREG = (0xA1);
    while (U2STAbits.TRMT == 0);
    U2TXREG = (0x00); //payload len: 0x0009
    while (U2STAbits.TRMT == 0);
    U2TXREG = (0x09);
    while (U2STAbits.TRMT == 0);
    U2TXREG = (0x08); //message id: 0x08
    while (U2STAbits.TRMT == 0);
    U2TXREG = (0x01); //GGA interval: 0x01
    while (U2STAbits.TRMT == 0);
    U2TXREG = (0x00); //GSA interval: 0x00
    while (U2STAbits.TRMT == 0);
    U2TXREG = (0x00); //GSV interval: 0x00
    while (U2STAbits.TRMT == 0);
    U2TXREG = (0x00); //GLL interval: 0x00
    while (U2STAbits.TRMT == 0);
    U2TXREG = (0x00); //RMC interval: 0x00
    while (U2STAbits.TRMT == 0);
    U2TXREG = (0x00); //VTG interval: 0x00
    while (U2STAbits.TRMT == 0);
    U2TXREG = (0x00); //ZDA interval: 0x00
    while (U2STAbits.TRMT == 0);
    U2TXREG = (0x00); //update ram only: 0x00
    while (U2STAbits.TRMT == 0);
    U2TXREG = (0x09); //checksum: 0x08
    while (U2STAbits.TRMT == 0);
    U2TXREG = (0x0D); //skytraq end of sequence: 0x0D0A
    while (U2STAbits.TRMT == 0);
    U2TXREG = (0x0A);
    while (U2STAbits.TRMT == 0);
    //SendString(""/*"<SILLY>Init'd<SILLY>"*/,0);

    //while(1) {
    //    mag_reset(MAG_ADDR);
        SendString("Init'd\n",0);
    //    for (i=0;i<10000;i++);
    //}
    //while(1);
    //for(i=0;i<0x80;i++) {
    //    sprintf(SBDnormal,"%x",i);
    //    SendString(SBDnormal,0);
    //    if (I2cEnum(i))
    //        SendString("A\n",0);
    //    else
    //        SendString("NAK\n",0);
    //}
    while(1) {
        GPSready=1;
        if (GPSnew) {
            SendString(GPSdat,0);
            GPSnew=0;
        }
    }
    while(1) {  //Main loop
        //if (U1STAbits.OERR) // If there is an overflow, do not read, and reset
        //    U1STAbits.OERR = 0;
        if (U2STAbits.OERR) // If there is an overflow, do not read, and reset
            U2STAbits.OERR = 0;
        if (U2STAbits.URXDA) {  //If data available from GPS
            receivedChar = U2RXREG; // Read the receive line of the PIC from the GPS
            if (GPScount<75) {
                GPSdata[GPScount]=receivedChar;
                GPScount++;
            }
            if (receivedChar==0x0A) {
                GPSdata[GPScount]=0;
                if (strncmp(GPSdata,"$GPGGA",6)==0) {
                    ParseNMEA(GPSdata,nTime,nLati,nLong,nAlti);
                    double dTime = atof(nTime);
                    double dLati = atof(nLati);
                    double dLong = atof(nLong);
                    double dAlti = atof(nAlti);
                    itob64(dTime,TIME);
                    itob64(dLati*10000,LATI);
                    itob64(dLong*10000,LONG);
                    itob64(dAlti*10,ALT);

                    /*
                     *
                     * ADC code here- Store value into Bb
                     *
                     */

                    SendString("OK\n",0);
                    
                    char ack=alt_read_adc(ALT_ADDR,&pres);
                    sprintf(SBDnormal,"P:%x %d\n",pres,ack);
                    SendString(SBDnormal,0);
                    alt_start_pressure(ALT_ADDR);
                    SendString("OK\n",0);
                    mag_start(MAG_ADDR);
                    while (!mag_check(MAG_ADDR));
                    mag_read(MAG_ADDR,&mx,&my,&mz);
                    sprintf(SBDnormal,"M:%d,%d,%d\n",mx,my,mz);
                    SendString(SBDnormal,0);

                    //sprintf(SBDnormal,"%9s%9s%10s%5s%2s%2s%1s%1s%2s%1s%2s%66s%120s%120s",TIME,LATI,LONG,ALT,pr,AT,t,B,bv,I,dt,Aa,Bb,Cc);
                    //sprintf(SBDnormal,"%9s%9s%10s%5s%120s",TIME,LATI,LONG,ALT,Bb); //Partial packet for Moses Lake
                    sprintf(SBDnormal,"%9s%9s%10s%5s",TIME,LATI,LONG,ALT); //Partial packet for Moses Lake
                    //SendString(SBDnormal,0);
                    //SendString("\n",0);

                    /*
                     *
                     * RockBlock code here- transmit SBDnormal
                     *
                     */
                    //HackRockSend(SBDnormal);
                    //HackBusyWait(100);
                }
                GPScount=0;
            }
        }
    }
    
    return 0;
}

double read_timer(void) // Reads the core timer to get the time since power on
{
    unsigned int core;
    core = ReadCoreTimer();   // Get the core timer count
    return (core * 2.0 / FSYS);
}
