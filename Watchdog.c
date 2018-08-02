#include "proc\p32mx360f512l.h"

void InitWatchdog() {
    WDTCONbits.ON=1;
}
void ResetWatchdog() {
    //WDTCONbits.WDTCLR=1;
}
