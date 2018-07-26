#include <proc/p32mx360f512l.h>

void PrintResetReason() {
    SendString_UART1("Reset reason:");
    if (RCONbits.POR)
        SendString_UART1("Poweron ");
    if (RCONbits.BOR)
        SendString_UART1("Brownout ");
    if (RCONbits.IDLE)
        SendString_UART1("Idle ");
    if (RCONbits.SLEEP)
        SendString_UART1("Sleep ");
    if (RCONbits.WDTO)
        SendString_UART1("Watchdog ");
    if (RCONbits.SWR)
        SendString_UART1("Software ");
    if (RCONbits.EXTR)
        SendString_UART1("MCLR ");
    if (RCONbits.CMR)
        SendString_UART1("Config ");
    SendChar_UART1('\n');
    RCONCLR=0x1DF;
}
