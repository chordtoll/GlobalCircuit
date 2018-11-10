
#include "proc\p32mx360f512l.h"

void InitGPIO() {
    TRISECLR=0x1FF;     //set port E pins 0-8 to output
    ODCECLR=0x1FF;      //disable open drain on port E pins 0-8
    
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
void SendChar_GPIO(char c) {
    PORTE=PORTE&0xFEFF;     //clear RE11
    PORTE=(PORTE&0xFF00)|c; //encode passed character into RE0-7
    PORTE=PORTE|0x100;      //set RE8
}
void SendString_GPIO(char *s) {
    for (;*s;s++)          //loop through characters of the string
        SendChar_GPIO(*s); //send the current character
}
