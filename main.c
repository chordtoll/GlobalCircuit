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
char SBDnormal[512] = {0};

char nTime[20];
char nLati[20];
char nLong[20];
char nAlti[20];

int main(void)
{
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
    SendString("Init'd\n",0);
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
                    double dTime=atof(nTime);
                    double dLati=atof(nLati);
                    double dLong=atof(nLong);
                    double dAlti=atof(nAlti);
                    itob64(dTime,TIME);
                    itob64(dLati*10000,LATI);
                    itob64(dLong*10000,LONG);
                    itob64(dAlti*10,ALT);
                    //sprintf(SBDnormal,"%10f%5s",dAlti,ALT);
                    sprintf(SBDnormal,"%9s%9s%10s%5s%2s%2s%1s%1s%2s%1s%2s%66s%120s%120s",TIME,LATI,LONG,ALT,pr,AT,t,B,bv,I,dt,Aa,Bb,Cc);
                    SendString(SBDnormal,0);
                    SendString("\n",0);
                }
                GPScount=0;
            }
        }
    }

    /*while(1)
    {
        // time (GMT), day, latitude, longitude, altitude
        if (U1STAbits.OERR) // If there is an overflow, do not read, and reset
        {
            U1STAbits.OERR = 0;
        }
        else if (U1STAbits.URXDA) // If there are characters to be read in the receive buffer
        {
            receivedChar = U1RXREG; // Read the receive line of the PIC from the GPS
            while (U1STAbits.RIDLE == 0); // Waiting to receive all the characters

            if (receivedChar == '$') // If the char is $, it is the beginning of a new line
            {
                if (first == 0)      // If this is the first time going through
                {
                    // Take the NMEA string that we received last time and break it up
                    // to get the longitude, latitude, altitude, and time using
                    // the helper functions in GPS.h
                    LONG = longitude(GPSdata, GPScount); // Parses the NMEA string and extracts the longitude
                    // The helper function puts 'A' in the first location of the character
                    // array when it is not being used because the strings coming
                    // in from the GPS alternate, and we don't pull the longitude
                    // from the second string, but the function is called each time
                    // a new string comes in
                    if (*(LONG) != 'A')
                    {
                        // Places the longitude into the character array LONG
                        // The array LONG will be used in the packet that is transmitted
                        // over the rockblock
                        for (loopCounter = 0; loopCounter < 11; loopCounter++)
                            printf("%c", *(LONG + loopCounter));
                        printf("\r\n");
                    }
                    LATI = latitude(GPSdata, GPScount);
                    if (*(LATI) != 'A')
                    {
                        for (loopCounter = 0; loopCounter < 10; loopCounter++)
                            printf("%c", *(LATI + loopCounter));
                        printf("\r\n");
                    }
                    ALT = altitude(GPSdata, GPScount);
                    if (*(ALT) != 'A')
                    {
                        for (loopCounter = 0; loopCounter < 6; loopCounter++)
                            printf("%c", *(ALT + loopCounter));
                        printf("\r\n");
                    }
                    TIME = GMTTime(GPSdata, GPScount);
                    if (*(TIME) != 'A')
                    {
                        for (loopCounter = 0; loopCounter < 10; loopCounter++)
                            printf("%c", *(TIME + loopCounter));
                        printf("\r\n");
                    }
                }
                GPScount = 0; // Reset the GPS count
                first = 0;    // It is no longer the first time we are going through the function
            }

            if (GPScount < 80) // If it is not the end of the NMEA string
            {
                GPSdata[GPScount] = receivedChar; // Add the character to the buffer
                GPScount++;                       // Increment the barray count
            }

        }
    }
    ////////////////////////////End GPS Testing/////////////////////////////////

    /////////////////////////On board ADC Testing///////////////////////////////
    x = ReadADC(0);
    v = x / (1024) * 3.3;
    printf("%f  \n\r",v);
    ///////////////////////End on Board ADC Testing/////////////////////////////

    ////////////////////SPI Testing with External ADC///////////////////////////
    WriteSPI(0x8480);
    while(!SPI1STATbits.SPIRBF);
    unsigned int result = ReadSPI();

    WriteSPI(0x848A);
    while(!SPI1STATbits.SPIRBF);
    double result2 = ReadSPI();

    printf("%d   \n\r", result);
    printf("%f   \n\r", result2);
    printf("%f   \n\r", result2 * Mul_Factor);
    //////////////////////////End SPI Testing///////////////////////////////////

    // Might need a small delay here
    for (count = 0; count1 < 2; count1++)
    {
        coretimer = read_timer();
        sprintf(RUN, "%.1f", coretimer);  // Putting the number read from the core timer into the character array RUN

        // Concatenating each character array into the 340 byte character array, SBDnormal, that will hold all of them
        // Carriage return at the end is necessary to add, this gives the command to send the message in the array
        strncat(SBDnormal, RUN, 3);
        strncat(SBDnormal, TIME, 3);
        strncat(SBDnormal, LATI, 4);
        strncat(SBDnormal, LONG, 4);
        strncat(SBDnormal, ALT, 3);
        strncat(SBDnormal, pr, 2);
        strncat(SBDnormal, AT, 2);
        strncat(SBDnormal, t, 1);
        strncat(SBDnormal, B, 1);
        strncat(SBDnormal, bv, 2);
        strncat(SBDnormal, I, 1);
        strncat(SBDnormal, dt, 2);
        strncat(SBDnormal, Aa, 66);
        strncat(SBDnormal, Bb, 120);
        strncat(SBDnormal, Cc, 120);
        strncat(SBDnormal, "\r", 1); // Gives command to send

        for (i = 0; i < strlen(SBDnormal); ++i) // Put the character array into the packet
            packet[i] = SBDnormal[i];

        j = 0;
        for (; i < 255 + strlen(SBDnormal); ++i)
            packet[i] = j++;

        if (checkService() == 0);               // If there is service, send the message
            SendSMS(packet);
    }*/
    
    return 0;
}

// When called this function sends the current packet given to it over the rock
// block
char SendSMS(char* packet)
{
    int i;
    for (i = 0; i < 5; i++) {}
    char transmit[] = "AT+SBDWB=340\r"; // Sets number of bytes to 340 of the message
    char READY[] = "READY\r";
    char SBDIX[] = "AT+SBDIX\r";  // Transmits
    char zero[] = "0\r";
    char OK[] = "OK\r";
    char* reply;

    SendString(transmit, 0);

    while(ReceivedLine() == 0);
    GetString();

    while(ReceivedLine() == 0);
    GetString();

    if (strcmp(READY, reply) != 0)  // Message sent successfully
    {
        // Green light
        TRISB = 0xFE;
    }

    SendString(packet, 1);

    while(ReceivedLine() == 0);
    GetString();

    if (strcmp(zero, reply) != 0)   // Message sent but did not receive successfully
    {
        // Yellow light
        TRISB = 0xFD;
    }

    while(ReceivedLine() == 0);
    GetString();

    if (strcmp(OK, reply) != 0)     // Message could not be sent
    {
        // Red light
        TRISB = 0xFB;
    }

    SendString(SBDIX, 0);
    while(ReceivedLine() == 0);
    GetString();
    if (strcmp(SBDIX, reply) != 0)
    {

    }

    return 0;
}

char CheckString(char* message)
{
    char* temp = GetString();
    if(temp[0] == message[0])
        return 0;
    return 1;
}

double read_timer(void) // Reads the core timer to get the time since power on
{
    unsigned int core;
    core = ReadCoreTimer();   // Get the core timer count
    return (core * 2.0 / FSYS);
}
