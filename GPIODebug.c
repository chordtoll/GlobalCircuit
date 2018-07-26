
#include "proc\p32mx360f512l.h"

void InitIO() {
    TRISECLR=0x1FF;
    ODCECLR=0x1FF;
}
void SendChar_GPIO(char c) {
    int i;
    PORTE=PORTE&0xFEFF;
    PORTE=(PORTE&0xFF00)|c;
    PORTE=PORTE|0x100;
}
void SendString_GPIO(char *s) {
    for (;*s;s++)
        SendChar_GPIO(*s);
}
