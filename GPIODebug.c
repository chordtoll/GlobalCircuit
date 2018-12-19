
#include "proc\p32mx360f512l.h"
#include "Timing.h"

#define OUT_DATA0 PORTEbits.RE0
#define OUT_DATA1 PORTEbits.RE1
#define OUT_TxEnable PORTEbits.RE2
#define OUT_CLK0 PORTEbits.RE4
#define OUT_CLK1 PORTEbits.RE5
#define IN_TxEnable PORTEbits.RE3
#define IN_DATA0 PORTEbits.RE6
#define IN_DATA1 PORTEbits.RE7
#define CLK_PERIOD 10000        //clock period in microseconds

void InitGPIO() {
    TRISECLR=0x37;      //set port E pins 0-2, 4-5 to output
    TRISESET=0xC8;      //set port E pins 3, 6-7 to input
    ODCECLR=0x1FF;      //disable open drain on port E pins 0-8
    OUT_TxEnable = 0;   //clear transmit enable
    
    TRISCbits.TRISC1=0; //set port C pins 1-3 to output
    TRISCbits.TRISC2=0;
    TRISCbits.TRISC3=0;
    
    ODCCbits.ODCC1=0;   //disable open drain on port C pins 1-3
    ODCCbits.ODCC2=0;
    ODCCbits.ODCC3=0;
    
    PORTCbits.RC3=1;    //set a HIGH value on RC3
    TRISDSET=0x03;      //set port D pins 0-1 to input
    TRISDCLR=0xFC;      //set port D pins 2-7 to output
    ODCDCLR=0xFF;       //disable open drain on all port D pins
    PORTDCLR=0xC0;      //set a LOW value on RD2-3
    TRISDbits.TRISD9=0; //set port D pin 9 to output
    PORTDbits.RD9=0;    //set a LOW value on RD9
}
void TickClock_GPIO()
{
    if(OUT_CLK0)                //if bit0 = 1
        OUT_CLK1 = !(OUT_CLK1); //change state of bit1

    OUT_CLK0 = !(OUT_CLK0);     //change state of bit0
}
char SendChar_GPIO(char c, char transmit) {
    /*PORTE=PORTE&0xFEFF;   //clear RE8
    PORTE=(PORTE&0xFF00)|c; //encode passed character into RE0-7
    PORTE=PORTE|0x100;      //set RE8*/
    
    uint8_t qByte;    //initialize quarter-byte counter
    char result = 0;  //data recieved from PIC16

    if(transmit)
        OUT_TxEnable = 1; //set transmit enable pin
    else
        OUT_TxEnable = 0;
    OUT_CLK0 = 0;     //set clock to represent 0th quarter-byte
    OUT_CLK1 = 0;
    

    for(qByte=0; qByte < 4; qByte++)                                 //loop for 4 quarter-bytes
    {
        OUT_DATA0 = (c & (1 << (qByte * 2))) >> (qByte * 2);         //set 0th bit of current quarter-byte
        OUT_DATA1 = (c & (1 << (1 + qByte * 2))) >> (1 + qByte * 2); //set 1st bit of current quarter-byte
        WaitUS(CLK_PERIOD);                                          //wait for some clock time
        if(IN_TxEnable)                                              //if the PIC16 is sending information
        {
            //store the recieved information into the proper quarter-byte of result
            result += (IN_DATA0 << (qByte * 2)) + (IN_DATA1 << (1 + qByte * 2));
        }
        TickClock_GPIO();                                            //tick the clock to next quarter-byte
    }


    OUT_CLK0 = 0;
    OUT_CLK1 = 0;
    if(transmit > 1)
        OUT_TxEnable = 0;
    WaitUS(CLK_PERIOD);
    OUT_CLK1 = 1;

    return result;                                                   //return the data recieved by the PIC16
}
void SendString_GPIO(char *s) {
    for (;*(s+1);s++)          //loop up to the last character of the string
        SendChar_GPIO(*s, 1);  //send the current character

    SendChar_GPIO(*s, 2);      //send the final character and end transmission
}
