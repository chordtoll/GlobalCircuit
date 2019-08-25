#include <proc/p32mx360f512l.h>
#include "GPS.h"
#include "GPIO.h"
#include "Yikes.h"
#include "packet.h"
#include <stdlib.h>

volatile char gpsbuf[84];
volatile uint8_t gpsbufi;
uint8_t locked = 0;
uint16_t days = 0;

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

void ParseDateRMC(char *data, uint8_t* day, uint8_t* month, uint16_t* year)
{
    uint8_t field = 0;                                                 //sentence field
    uint8_t index = 0;                                                 //character index
    if(strncmp(data, "$GPRMC", 6) == 0)                                //check for a GPRMC sentence
    {
        while(field != 9 && index < 80)                                //loop until date field is reached
        {
            if(data[index] == ',')                                     //if a field seperator is reached
                ++field;                                               //move to the next field
            ++index;                                                   //move to the next character
        }
        *day=(data[index] - '0')*10 + (data[index+1] - '0');           //calculate and store day
        *month=(data[index+2] - '0')*10 + (data[index+3] - '0');       //calculate and store month
        *year=2000 + (data[index+4] - '0')*10 + (data[index+5] - '0'); //calculate and store year
    }
}

void CheckPosFix(char *data)
{
    uint8_t field = 0;                  //sentence field
    uint8_t index = 0;                  //sentence field
    if(strncmp(data, "$GPGGA", 6) == 0) //check for a GPGGA sentence
    {
        while(field != 6 && index < 80) //loop until position field is fixed
        {
            if(data[index] == ',')      //if a field seperator is reached
                ++field;                //move to the next field
            ++index;                    //move to the next character
        }
        if(data[index] != '0')          //check if GPS is locked
            locked = 1;                 //set locked flag
    }
}

void InitGPS(void) {
    /*
     * GGA Interval: 1 second
     * GSA Interval: disabled
     * GSV Interval: disabled
     * GLL Interval: disabled
     * RMC Interval: 55 seconds
     * VTG Interval: disabled
     * ZDA Interval: disabled
     * Updates to SRAM
     */
    char InitString[16]={0xA0,0xA1,0x00,0x09,0x08,0x01,0x00,0x00,0x00,0x37,0x00,0x00,0x00,0x3E,0x0D,0x0A};
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
    GPS_S_EN = 1;                   //power off the GPS
    GPS_BAT_EN = 1;
    WaitS(2);                       //wait for 2 seconds
    GPS_S_EN = 0;                   //power on the GPS
    GPS_BAT_EN = 0;
    ResetWatchdog();
    WaitS(6);                       //wait for 6 seconds
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

uint16_t DaysSinceDate(uint8_t day1, uint8_t month1, uint16_t year1, uint8_t day2, uint8_t month2, uint16_t year2)
{
    uint16_t daycount = 0;                                                                   //counter for days
    uint16_t days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};          //number of days in each month
    while(!(day1 == day2 && month1 == month2))                                               //loop until day and month matches up
    {
        uint8_t leap = ((!(year2 % 4) && (year2 % 100 || !(year2 % 400))) && (month1 == 2)); //modifier for leap day in february
        ++day1;                                                                              //count another day
        ++daycount;
        if(day1 > days_in_month[month1] + leap)                                              //if at the end of a month
        {
            day1 = 1;                                                                        //move to the next month
            ++month1;
        }
    }
    while(year1 != year2)                                   //loop until years match up
    {
        if(!(year1 % 4) && (year1 % 100 || !(year1 % 400))) //if on a leap year
            daycount += 366;                                //add 366 days
        else                                                //if on a normal year
            daycount += 365;                                //add 365 days
        ++year1;                                            //move to the next year
    }
    return daycount;                                        //return the number of days
}

void ReadGPS(uint32_t* time, uint32_t* lat, uint32_t* lon, uint32_t* alt, uint8_t* sats) {
    char nTime[20];                                                               //time string
    char nLati[20];                                                               //latitude string
    char nLong[20];                                                               //longitude string
    char nAlti[20];                                                               //altitude string
    char nSats[10];                                                               //satellites string
    char nLatD;                                                                   //latitude direction
    char nLonD;                                                                   //longitude direction
    if (GPSnew) {                                                                 //if a new GPS reading is ready
        GPSnew=0;                                                                 //clear new flag
    if (strncmp(GPSGGAdata, "$GPGGA", 6) == 0) {                                  //if gps data is a GGA sentence
        ParseNMEA(GPSGGAdata, nTime, nLati, &nLatD, nLong, &nLonD, nAlti, nSats); //parse the data
        *time=atof(nTime);                                                        //parse time to an integer
#ifdef EPOCH_TIME
        *time = HMStoS(*time) + days*86400;                                   //set time to current Epoch time
#endif
        *lat=(atof(nLati)*10000);                                                 //parse latitude
        if (nLatD=='S')
            *lat|=0x80000000;
        *lon=(atof(nLong)*10000);                                                 //parse longitude
        if (nLonD=='W')
            *lon|=0x80000000;
        *sats=(atoi(nSats));                                                      //parse number of satellites
        *alt=(atof(nAlti)*10);                                                    //parse altitude
         if(!locked)                                                              //if GPS is not locked
            yikes.gpslock = 1;                                                    //set gpslock yikes flag
    }
    }

    locked = 0;                                                                   //reset locked flag
    WakeGPS();                                                                    //wake up the GPS flag
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
        if (strncmp(gpsbuf, "$GPGGA", 6) == 0) {
            CheckPosFix(gpsbuf);
            strcpy((char *)GPSGGAdata,(const char *)gpsbuf); //From this context, these buffers are not volatile, so we can discard that qualifier
            GPSnew=1;
        }
        else if (strncmp(gpsbuf, "$GPRMC", 6) == 0) {
            uint8_t day;
            uint8_t month;
            uint16_t year;
            ParseDateRMC(gpsbuf, &day, &month, &year);
            days = DaysSinceDate(1,1,1970,day,month,year);
        }

    } else {
        gpsbuf[gpsbufi++]=receivedChar;
    }
    IFS1bits.U2RXIF = 0; //clear interrupt flag status for UART1 receive
}