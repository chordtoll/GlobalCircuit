
#include "proc\p32mx360f512l.h"

void InitGPIO() {
    TRISECLR=0x1FF;
    ODCECLR=0x1FF;
    TRISCbits.TRISC1=0;
    TRISCbits.TRISC2=0;
    TRISCbits.TRISC3=0;
    ODCCbits.ODCC1=0;
    ODCCbits.ODCC2=0;
    ODCCbits.ODCC3=1;
}
void SendChar_GPIO(char c) {
    PORTE=PORTE&0xFEFF;
    PORTE=(PORTE&0xFF00)|c;
    PORTE=PORTE|0x100;
}
void SendString_GPIO(char *s) {
    for (;*s;s++)
        SendChar_GPIO(*s);
}
