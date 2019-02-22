
#include "proc\p32mx360f512l.h"
#include "Timing.h"
#include "yikes.h"
#include "GPIODebug.h"

#define OUT_DATA0 PORTEbits.RE0     //bit 0 of Tx data
#define OUT_DATA1 PORTEbits.RE1     //bit 1 of Tx data
#define OUT_TxEnable PORTEbits.RE2  //Tx transmission flag
#define OUT_CLK0 PORTEbits.RE4      //bit 0 of Tx clock
#define OUT_CLK1 PORTEbits.RE5      //bit 1 of Tx clock
#define IN_TxEnable PORTEbits.RE3   //Rx transmission flag
#define IN_DATA0 PORTEbits.RE6      //bit 0 of Rx data
#define IN_DATA1 PORTEbits.RE7      //bit 1 of Rx data

#define CLK_PERIOD 10   //clock period in milliseconds

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
char ExchangeChar_GPIO(char c, char transmit) {
    uint8_t qByte;    //initialize quarter-byte counter
    char result = 0;  //data received from PIC16

    OUT_TxEnable = 1; //set transmit enable pin
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
        TickClock_GPIO();                                            //tick the clock to next quarter-byte
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

void InitiateCutdown() {
    SendString_GPIO("CUT");               //send a cutdown command to the PIC16
    while(!IN_TxEnable){}                 //wait for the PIC16 to respond
    if(ExchangeChar_GPIO(0,0) == '?')     //if a confirmation was received
    {
        //yikes.cutdown0 = 1;
        //yikes.cutdown1 = 1;
        cutdown_ip = 1;
    }
    else
    {
        cutdown_ip = 2;
    }
}

char CheckCutdown() {
    if(cutdown_ip == 1)      //check to see if cutdown was started previously, and PIC16 has finished the cutdown
    {
        switch(ExchangeChar_GPIO(0,0)) //read in the status character from the PIC16
        {
            case 'K':               //if character was 'K' (success)
                //yikes.cutdown0 = 0; //clear all cutdown flags
                //yikes.cutdown1 = 0;
                //packet.norm.cutdown = 0b10; //indicate success on cutdown flags
                cutdown_ip = 0;               //cutdown sequence complete
                return 1;
                break;

            case 'U':               //if character was 'U' (unexpected response)
                //yikes.cutdown0 = 1; //set yikes flags to 01
                //yikes.cutdown1 = 0;
                //packet.norm.cutdown = 0b1000; //indicate failure due to unexpected response on cutdown flag
                cutdown_ip = 0;     //cutdown sequence complete
                return 0b10000;
                break;

            case 'N':               //if character was 'N' (no response)
                //yikes.cutdown0 = 0; //set yikes flags to 10
                //yikes.cutdown1 = 1;
                //packet.norm.cutdown = 0b10000; //indicate failure due to no response in cutdown
                cutdown_ip = 0;     //clear "cutdown in progress" flag
                return 0b1000;
                break;

            default:                //if none of the above characters were received
                //yikes.cutdown0 = 1; //maintain "cutdown in progress" flag states
                //yikes.cutdown1 = 1;
                //packet.norm.cutdown = 1; //indicate that cutdown is still in progress on cutdown flag
                cutdown_ip =1;
                return 0b10;
                break;
        }
    }
    else if(cutdown_ip == 2)
    {
        cutdown_ip = 0;
        return 0b100;
    }
    return 0;
}
