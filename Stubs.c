#include "Stubs.h"
#include "MAG3310.h"
#include "MS5607.h"

char chanidx[6];

void InitADC_S() {
    int i;
    SendString_UART1("Init ADC ");
    for (i=0;i<6;i++) chanidx[i]=0;
}
void SampleADC_S(char channel) {
    SendString_UART1("Sample ADC");
    SendChar_UART1('0'+channel);
    SendChar_UART1(' ');
    chanidx[channel]++;
}
int ReadADC_S(char channel) {
    SendString_UART1("Read ADC");
    SendChar_UART1('0'+channel);
    SendChar_UART1(' ');
    return chanidx[channel]|(channel<<8);
}

int ReadPICADC_S(char channel) {
    SendString_UART1("Read PIC ADC");
    SendChar_UART1('0'+channel);
    SendChar_UART1(' ');
    return ReadPICADC(channel);
}

void TriggerMagneto_S() {
    SendString_UART1("Trigger magnetometer ");
    TriggerMagneto(MAG_ADDR);
}
void ReadMagneto_S(uint16_t* a, uint16_t* b, uint16_t* c) {
    SendString_UART1("Read magnetometer ");
    ReadMagneto(MAG_ADDR,a,b,c);
    
}
void TriggerAltimeter_Pressure_S() {
    SendString_UART1("Trigger alt pressure ");
    TriggerAltimeter_Pressure(ALT_ADDR);
}
void TriggerAltimeter_Temperature_S() {
    SendString_UART1("Trigger alt tempterature ");
    TriggerAltimeter_Temperature(ALT_ADDR);
}
uint32_t ReadAltimeter_S() {
    uint32_t pressure;
    SendString_UART1("Read alt ");
    ReadAltimeter_ADC(ALT_ADDR, &pressure);
    return 0x00ABCDEF;
    return pressure;
}

void ReadGPS_S(double* time, double* latitude, double* longitude, double* altitude) {
    SendString_UART1("Read GPS ");
}

void ChargeProbe_S(chgst_t state) {
    switch (state) {
        case NONE:
            SendString_UART1("Charge probes OFF ");
            break;
        case UP:
            SendString_UART1("Charge probes ^^^ ");
            break;
        case DOWN:
            SendString_UART1("Charge probes vvv ");
            break;
        case GND:
            SendString_UART1("Charge probes === ");
            break;
    }
}

void RockSend_S(char *packet) {
    SendString_UART1("Send packet:");
    SendString_UART1(packet);
    SendChar_UART1(' ');
}

char RockCheck_S() {
    SendString_UART1("Check inbox ");
    return 0;
}
char RockReceive_S(char *packet) {
    SendString_UART1("Receive packet ");
    return 1;
}
