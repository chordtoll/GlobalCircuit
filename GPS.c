#include <proc/p32mx360f512l.h>
#include "GPS.h"
#include "GPIO.h"
#include "Yikes.h"
#include "packet.h"
#include <stdlib.h>

volatile char gpsbuf[84];
volatile uint8_t gpsbufi;
uint8_t locked = 0;
uint8_t days = 0;
uint32_t lasttime = 0;

void ParseNMEA(char *data, char* time, char *lati, char *latd, char *llon, char *lond, char *alti, char *sats) {
    uint8_t field=0;          //selects which field is currently being updated
    uint16_t fieldstart=0;    //starting offset of each field
    uint16_t idx=0;           //data offset
    while (data[idx]) {       //loop until all of data has been sorted
        if (data[idx]==',') { //if the current data is only a seperator,
            switch(field) {   //set the current field's corresponding position to 0
                case 1:
                    time[idx-fieldstart]=0;
                    break;
                case 2:
                    lati[idx-fieldstart]=0;
                    break;
                case 4:
                    llon[idx-fieldstart]=0;
                    break;
                case 7:
                    sats[idx-fieldstart]=0;
                    break;
                case 9:
                    alti[idx-fieldstart]=0;
                    break;
            }
            field++;          //move to the next field
            fieldstart=idx+1; //update the field offset
        } else {              //if the current data actually is data
            switch(field) {   //store the data into the correct field at the correct position
                case 1:
                    time[idx-fieldstart]=data[idx];
                    break;
                case 2:
                    lati[idx-fieldstart]=data[idx];
                    break;
                case 3:
                    *latd=data[idx];
                    break;
                case 4:
                    llon[idx-fieldstart]=data[idx];
                    break;
                case 5:
                    *lond=data[idx];
                    break;
                case 7:
                    sats[idx-fieldstart]=data[idx];
                    break;
                case 9:
                    alti[idx-fieldstart]=data[idx];
                    break;
            }
        }
        idx++;              //move to the next point of data
    }
}

void CheckPosFix(char *data)
{
    uint8_t field = 0;
    uint8_t index = 0;
    if(strncmp(data, "$GPGGA", 6) == 0)
    {
        while(field != 6 && index < 80)
        {
            if(data[index] == ',')
                ++field;
            ++index;
        }
        if(data[index] == '1')
            locked = 1;
    }
}

void InitGPS(void) {
    /*
     * GGA Interval: 1 second
     * GSA Interval: disabled
     * GSV Interval: disabled
     * GLL Interval: disabled
     * RMC Interval: disabled
     * VTG Interval: disabled
     * ZDA Interval: disabled
     * Updates to SRAM
     */
    char InitString[16]={0xA0,0xA1,0x00,0x09,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x0D,0x0A};
    uint8_t i;

    PORTDbits.RD14 = 0; //pull PPS pin low to prevent GPS starting in debug mode

//comment out if mosfet is not being used for GPS reset
#define MOSFET_GPS_RESET

//if mosfet is not being used for GPS reset, reset is active low
#ifndef MOSFET_GPS_RESET
    PORTDbits.RD9=1; //take GPS out of reset
#endif

//if mosfet is being used for GPS reset, reset is active high
#ifdef MOSFET_GPS_RESET
    //PORTDbits.RD9=0; //take GPS out of reset
#endif
    ResetWatchdog();
    GPS_S_EN = 1;
    GPS_BAT_EN = 1;
    WaitS(2);
    GPS_S_EN = 0;
    GPS_BAT_EN = 0;
    ResetWatchdog();
    WaitS(6);
    ResetWatchdog();
    for (i=0;i<16;i++) {            //send initialization string to GPS
        U2TXREG=InitString[i];
        while (U2STAbits.TRMT == 0);//wait for the full message to be sent
    }
}

void SleepGPS()
{
    //GPS_S_EN = 1;
}

void WakeGPS()
{
    //GPS_S_EN = 0;
}

uint32_t HMStoS(uint32_t time)
{
    uint32_t hours = time / 10000;
    uint32_t minutes = (time - (hours*10000)) / 100;
    uint32_t seconds = time - (hours*10000) - (minutes*100);
    return hours*3600 + minutes*60 + seconds;
}

void ReadGPS(uint32_t* time, uint32_t* lat, uint32_t* lon, uint32_t* alt, uint8_t* sats) {
    char nTime[20];
    char nLati[20];
    char nLong[20];
    char nAlti[20];
    char nSats[10];
    char nLatD;
    char nLonD;
    if (GPSnew) {
        GPSnew=0;
    if (strncmp(GPSdata, "$GPGGA", 6) == 0) {
        ParseNMEA(GPSdata, nTime, nLati, &nLatD, nLong, &nLonD, nAlti, nSats);
        *time=atof(nTime);
#ifdef EPOCH_TIME
        *time = HMStoS(*time) + (DAYS_SINCE_EPOCH + days)*86400;
        if(*time < lasttime)
        {
            ++days;
            *time = *time + 86400;
        }
        lasttime = *time;
#endif
        *lat=(atof(nLati)*10000);
        if (nLatD=='S')
            *lat|=0x80000000;
        *lon=(atof(nLong)*10000);
        if (nLonD=='W')
            *lon|=0x80000000;
        *sats=(atoi(nSats));
        *alt=(atof(nAlti)*10);
         if(!locked)
            yikes.gpslock = 1;
    }
    }

    locked = 0;
    WakeGPS();
}

void  __attribute__((vector(_UART_2_VECTOR), interrupt(IPL7SRS), nomips16)) UART2_ISR(void)
{
    if(U2STAbits.OERR)
        U2STAbits.OERR = 0;
    char receivedChar = U2RXREG; //get char from uart1rec line
    if (gpsbufi>=80) {
        gpsbufi=0;
    }
    if (receivedChar=='$') {
        gpsbufi=0;
        gpsbuf[gpsbufi++]=receivedChar;
    } else if (receivedChar==0x0A) {
        gpsbuf[gpsbufi++]=receivedChar;
        gpsbuf[gpsbufi++]=0;
        CheckPosFix(gpsbuf);
        strcpy((char *)GPSdata,(const char *)gpsbuf); //From this context, these buffers are not volatile, so we can discard that qualifier
        GPSnew=1;
    } else {
        gpsbuf[gpsbufi++]=receivedChar;
    }
    IFS1bits.U2RXIF = 0; //clear interrupt flag status for UART1 receive
}