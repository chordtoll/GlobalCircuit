#include "Stubs.h"
#include "MAG3310.h"
#include "MS5607.h"
#include "transmit.h"
#include "proc\p32mx360f512l.h"
#include <stdlib.h>

char chanidx[6];

void InitADC_S() {
    int i;
    //SendString_UART1("Init ADC ");
    for (i=0;i<6;i++) chanidx[i]=0;
}
int ReadADC_S(char channel) {
    chanidx[channel]++;
    //SendString_UART1("Read ADC");
    //SendChar_UART1('0'+channel);
    //SendChar_UART1(' ');
    return chanidx[channel]|(channel<<8);
}

int ReadPICADC_S(char channel) {
    //SendString_UART1("Read PIC ADC");
    //SendChar_UART1('0'+channel);
    //SendChar_UART1(' ');
    return ReadPICADC(channel);
}

void TriggerMagneto_S() {
    //SendString_UART1("Trigger magnetometer ");
    TriggerMagneto(MAG_ADDR);
}
void ReadMagneto_S(uint16_t* a, uint16_t* b, uint16_t* c) {
    //SendString_UART1("Read magnetometer ");
    ReadMagneto(MAG_ADDR,a,b,c);
    
}
void TriggerAltimeter_Pressure_S() {
    //SendString_UART1("Trigger alt pressure ");
    TriggerAltimeter_Pressure(ALT_ADDR);
}
void TriggerAltimeter_Temperature_S() {
    //SendString_UART1("Trigger alt tempterature ");
    TriggerAltimeter_Temperature(ALT_ADDR);
}
uint32_t ReadAltimeter_S() {
    uint32_t pressure;
    //SendString_UART1("Read alt ");
    ReadAltimeter_ADC(ALT_ADDR, &pressure);
    return 0x00ABCDEF;
    return pressure;
}

void ReadGPS_S(uint32_t* time, int32_t* lat, int32_t* lon, uint32_t* alt) {
    char nTime[20];
    char nLati[20];
    char nLong[20];
    char nAlti[20];
    char nLatD;
    char nLonD;
    //SendString_UART1("Read GPS ");
    if (GPSnew) {
        GPSnew=0;
    if (strncmp(GPSdata, "$GPGGA", 6) == 0) {
        ParseNMEA(GPSdata, nTime, nLati, &nLatD, nLong, &nLonD, nAlti);
//        SendString_UART1("\n");
//        SendString_UART1(GPSdata);
//        SendString_UART1("\nTIME ");
//        SendString_UART1(nTime);
//        SendString_UART1(" LAT ");
//        SendString_UART1(nLati);
//        SendString_UART1(" LON ");
//        SendString_UART1(nLong);
//        SendString_UART1(" ALT ");
//        SendString_UART1(nAlti);
//        SendChar_UART1('\n');
        *time=atof(nTime);
        *lat=(atof(nLati)*10000)*(nLatD=='S'?-1:1);
        *lon=(atof(nLong)*10000)*(nLonD=='W'?-1:1);
        *alt=(atof(nAlti)*10);
    }
    }
    GPSready=1;
}

void ChargeProbe_S(chgst_t state) {
    switch (state) {
        case NONE:
            //SendString_UART1("Charge probes OFF ");
            PORTEbits.RE0=0;
            PORTEbits.RE1=0;
            PORTEbits.RE2=0;
            break;
        case UP:
            //SendString_UART1("Charge probes ^^^ ");
            PORTEbits.RE0=0;
            PORTEbits.RE1=1;
            PORTEbits.RE2=1;
            break;
        case DOWN:
            //SendString_UART1("Charge probes vvv ");
            PORTEbits.RE0=1;
            PORTEbits.RE1=0;
            PORTEbits.RE2=1;
            break;
        case GND:
            //SendString_UART1("Charge probes === ");
            PORTEbits.RE0=1;
            PORTEbits.RE1=1;
            PORTEbits.RE2=1;
            break;
    }
}

void RockSend_S(char *packet) {
    //SendString_UART1("Send packet:");
    //SendString_UART1(packet);
    SendString_RB(packet);
}

char RockCheck_S() {
    //SendString_UART1("Check inbox ");
    return 0;
}
char RockReceive_S(char *packet) {
    //SendString_UART1("Receive packet ");
    return 1;
}
