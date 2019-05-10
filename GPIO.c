
#include "proc\p32mx360f512l.h"
#include "Timing.h"
#include "yikes.h"
#include "GPIO.h"

#define CLK_PERIOD 10  //clock period in milliseconds

void InitGPIO() {
    TRISECLR=0x37;        //set port E pins 0-2, 4-5, 8 to output
    TRISESET=0xC8;        //set port E pins 3, 6-7 to input
    ODCECLR=0x1FF;        //disable open drain on port E pins 0-8
    TRISEbits.TRISE8 = 0;
    TRISDbits.TRISD8 = 0; //MOVE TO RD8

    OUT_TxEnable = 0;   //clear transmit enable
    PIC16_SLEEP = 0;       //keep PIC16 asleep
    GPS_EN = 1;
    
    TRISGbits.TRISG6=0; //set port C pins 1-3 to output
    TRISGbits.TRISG7=0;
    TRISGbits.TRISG8=0;
    
    ODCGbits.ODCG6=0;   //disable open drain on port C pins 1-3
    ODCGbits.ODCG7=0;
    ODCGbits.ODCG8=0;
    
    //PORTGbits.RG3=1;    //set a HIGH value on RC3
    TRISDSET=0x03;      //set port D pins 0-1 to input
    TRISDCLR=0xFC;      //set port D pins 2-7 to output
    ODCDCLR=0xFF;       //disable open drain on all port D pins
    PORTDCLR=0xC0;      //set a LOW value on RD2-3
    TRISDbits.TRISD9=0; //set port D pin 9 to output
    PORTDbits.RD9=0;    //set a LOW value on RD9
}
void TickClock()
{
    if(OUT_CLK0)                //if bit0 = 1
        OUT_CLK1 = !(OUT_CLK1); //change state of bit1
    OUT_CLK0 = !(OUT_CLK0);     //change state of bit0
}

void WakePIC16()
{
    PIC16_SLEEP = 1;
    WaitMS(100);
}

char ExchangeChar_GPIO(char c, char transmit) {
    uint8_t qByte;    //initialize quarter-byte counter
    char result = 0;  //data received from PIC16

    OUT_TxEnable = 1; //set transmit enable pin
    WakePIC16();
    if(!transmit)     //if we are not sending any information
    {
        WaitMS(CLK_PERIOD / 10); //wait for 1/10 clock period
        OUT_TxEnable = 0;        //clear transmit enable pin
    }
    OUT_CLK0 = 0;     //set clock to represent 0th quarter-byte
    OUT_CLK1 = 0;
    
    for(qByte=0; qByte < 4; qByte++)                                 //loop for 4 quarter-bytes
    {
        OUT_DATA0 = (c & (1 << (qByte * 2))) >> (qByte * 2);         //set 0th bit of current quarter-byte
        OUT_DATA1 = (c & (1 << (1 + qByte * 2))) >> (1 + qByte * 2); //set 1st bit of current quarter-byte
        WaitMS(CLK_PERIOD);                                          //wait for some clock time
        if(IN_TxEnable)                                              //if the PIC16 is sending information
        {
            //store the received information into the proper quarter-byte of result
            result += (IN_DATA0 << (qByte * 2)) + (IN_DATA1 << (1 + qByte * 2));
        }
        TickClock();                                                 //tick the clock to next quarter-byte
    }

    OUT_TxEnable = 0;                                                //clear transmit enable pin
    WaitMS(CLK_PERIOD / 10);                                         //wait for 1/10 of clock period
    OUT_CLK1 = 1;                                                    //set clock to the idle state (10)
    OUT_CLK0 = 0;

    return result;                                                   //return the data received by the PIC16
}
void SendString_GPIO(char *s) {
    for (;*s;s++)                  //loop up to the last character of the string
        ExchangeChar_GPIO(*s, 1);  //send the current character
}



void ChargeProbe(chgst_t state) {
    switch (state) {
        case NONE:
            PORTGbits.RG6=0;
            PORTGbits.RG7=0;
            PORTGbits.RG8=1;
            break;
        case UP:
            PORTGbits.RG6=1;
            PORTGbits.RG7=0;
            PORTGbits.RG8=0;
            break;
        case DOWN:
            PORTGbits.RG6=0;
            PORTGbits.RG7=1;
            PORTGbits.RG8=0;
            break;
        case GND:
            PORTGbits.RG6=0;
            PORTGbits.RG7=0;
            PORTGbits.RG8=0;
            break;
    }
}