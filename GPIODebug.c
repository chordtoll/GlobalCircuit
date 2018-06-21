
#include "proc\p32mx360f512l.h"

void IOInit() {
    TRISECLR=0x1FF;
    ODCECLR=0x1FF;
}
void IOWriteChar(char c) {
    int i;
    PORTE=PORTE&0xFEFF;
    PORTE=(PORTE&0xFF00)|c;
    PORTE=PORTE|0x100;
}
void IOWriteString(char *s) {
    for (;*s;s++)
        IOWriteChar(*s);
}
