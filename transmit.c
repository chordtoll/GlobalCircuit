#include <string.h>
#include "transmit.h"
#include "proc\p32mx360f512l.h"
#include "Timing.h"
#include "Yikes.h"
volatile char gpsbuf[84];
volatile uint8_t gpsbufi;

void InitUART()
{
    //Setting up UART1 Transmit
    U1BRG = 260;           //Initialize U1BRG for 19200 baud rate
    U2BRG = 520;           //Initialize U2BRG for 9600 baud rate

    U1MODEbits.PDSEL = 0;   //8 bit no parity
    U2MODEbits.PDSEL = 0;   //8 bit no parity

    U1MODEbits.STSEL = 0;   //1 stop bit
    U2MODEbits.STSEL = 0;   //1 stop bit

    U1MODEbits.UARTEN=1;
    U2MODEbits.UARTEN=1;

    U1STAbits.UTXEN=1;
    U2STAbits.UTXEN=1;

    U1STAbits.URXEN=1;
    U2STAbits.URXEN=1;

    IEC0bits.U1RXIE = 1;     //Interrupt is enabled for UART1 receive
    IEC1bits.U2RXIE = 1;     //Interrupt is enabled for UART1 receive

    IPC6bits.U1IP = 7;       //UART1 interrupt priority is 1
    IPC8bits.U2IP = 7;       //UART1 interrupt priority is 1

    IPC6bits.U1IS = 3;       //UART1 subpriority is 0
    IPC8bits.U2IS = 3;       //UART1 subpriority is 0

    U1STAbits.URXISEL = 0;   //Receive interrupt mode flag is set when character is received
    U2STAbits.URXISEL = 0;   //Receive interrupt mode flag is set when character is received

    U1MODEbits.ON = 1;
    U2MODEbits.ON = 1;

}

void InitInterrupt()
{
    INTCONbits.MVEC = 1;  //enabling multivector interrupts
    IFS0bits.U1RXIF = 0; // clearing uart1 interrupt flag
    IFS1bits.U2RXIF = 0; // clearing uart1 interrupt flag
    IFS0bits.T1IF   = 0; // clearing uart1 interrupt flag
    __asm__("EI");  // enable interrupts
}

void  __attribute__((vector(_UART_1_VECTOR), interrupt(IPL7SRS), nomips16)) UART1_ISR(void)
{
    char receivedChar = U1RXREG; //get char from uart1rec line
    
    if (_rb_status==RB_BUSY) {
        if (_rb_idx>=340)
            _rb_idx=0;
        _rb_cmdbuf[_rb_idx]=receivedChar;
        _rb_idx++;
        _rb_cmdbuf[_rb_idx]=0;
        if (rbstrcmp(_rb_cmdbuf,_rb_idx,"\n\rKO"))   { //Check if response is OK
            _rb_status=RB_OK;
            _rb_idx=0;
        }
        if (rbstrcmp(_rb_cmdbuf,_rb_idx,"\n\rYDAER")){ //Check if response is READY
            _rb_status=RB_READY;
            _rb_idx=0;
        }
        if (rbstrcmp(_rb_cmdbuf,_rb_idx,"\n\rRORRE")){ //Check if response is ERROR
            _rb_status=RB_ERROR;
            _rb_idx=0;
        }
    }
    IFS0bits.U1RXIF = 0; //clear interrupt flag status for UART1 receive
}

void  __attribute__((vector(_UART_2_VECTOR), interrupt(IPL7SRS), nomips16)) UART2_ISR(void)
{
    char receivedChar = U2RXREG; //get char from uart1rec line

    if (gpsbufi>=80) {
        gpsbufi=0;
    }
    if (receivedChar=='$') {
        gpsbufi=0;
        gpsbuf[gpsbufi++]=receivedChar;
    } else if (receivedChar==0x0A) {
        gpsbuf[gpsbufi++]=receivedChar;
        gpsbuf[gpsbufi++]=0;
        if (GPSready) {
            strcpy((char *)GPSdata,(const char *)gpsbuf); //From this context, these buffers are not volatile, so we can discard that qualifier
            GPSready=0;
            GPSnew=1;
        }
    } else {
        gpsbuf[gpsbufi++]=receivedChar;
    }
    IFS1bits.U2RXIF = 0; //clear interrupt flag status for UART1 receive
}

void SendString_UART1(unsigned char* string)
{
    while(*string != 0)
    {
        SendChar_UART1(*(string++));
    }
}

void SendBuffer_UART1(char *buf, uint16_t start, uint16_t len) {
    uint16_t i;
    for (i=start;i<len;i++) {
        SendChar_UART1(buf[i]);
    }
}

void SendNybble_UART1(char n) {
    if (n>9)
        SendChar_UART1('A'+n-10);
    else
        SendChar_UART1('0'+n);
}

void SendHex_UART1(char letter) {
    SendNybble_UART1((letter>>4)&0xF);
    SendNybble_UART1(letter&0xF);
}

void SendChar_UART1(char letter)
{
    while(U1STAbits.TRMT == 0){} //while transmitting don't do anything
    U1TXREG = letter;            //transmit first char
}

void SendString_UART2(unsigned char* string)
{
    while(*string != 0)
    {
        SendChar_UART2(*(string++));
    }
}

void SendChar_UART2(char letter)
{
    while(U2STAbits.TRMT == 0){} //while transmitting don't do anything
    U2TXREG = letter;            //transmit first char
}

char rbstrcmp(volatile char *s1,uint16_t s1i,const char *s2) {
    while (*s2) {
        if (s1i==0)
            return 0;
        s1i--;
        if (s1[s1i]!=*s2)
            return 0;
        s2++;
    }
    return 1;
}