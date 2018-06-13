
#include "proc\p32mx360f512l.h"

void IOInit() {
    TRISECLR=0x1FF;
    ODCECLR=0x1FF;
}
void IOWriteChar(char c) {
    int i;
    PORTECLR=0x1FF;
    PORTESET=0xFF&c;
    for (i=100;i;i--);
    PORTESET=0x100;
}
void IOWriteString(char *s) {
    while (s) {
        IOWriteChar(*(s++));
    }
}
