#include "transmit.h"
#include "proc\p32mx360f512l.h"
#include "Timing.h"
volatile char gpsbuf[84];
volatile char gpsbufi;

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
    __asm__("EI");  // enable interrupts
}

void  __attribute__((vector(_UART_1_VECTOR), interrupt(IPL7SRS), nomips16)) UART1_ISR(void)
{
    IFS0bits.U1RXIF = 0; //clear interrupt flag status for UART1 receive
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

}

void  __attribute__((vector(_UART_2_VECTOR), interrupt(IPL7SRS), nomips16)) UART2_ISR(void)
{
    char receivedChar = U2RXREG; //get char from uart1rec line
    if (gpsbufi<=80) {
        gpsbufi=0;
    }
    if (receivedChar=='$') {
        // BEGIN TIMING CRITICAL DO NOT SPLIT
        unsigned int ctt=ReadCoreTimer();
        WriteCoreTimer(0);
        timer_accum+=ctt;
        // END   TIMING CRITICAL DO NOT SPLIT
        tps*=9;
        tps+=ctt;
        tps/=10;
        gpsbufi=0;
        gpsbuf[gpsbufi++]=receivedChar;
    } else if (receivedChar==0x0A) {
        gpsbuf[gpsbufi++]=receivedChar;
        gpsbuf[gpsbufi++]=0;
        if (GPSready) {
            strcpy(GPSdata,gpsbuf);
            GPSready=0;
            GPSnew=1;
        }
    } else {
        gpsbuf[gpsbufi++]=receivedChar;
    }
    IFS1bits.U2RXIF = 0; //clear interrupt flag status for UART1 receive

}

//int RockInit()
//{
//    char transmit[] = "AT+CIER=1,0,1\r";
//    char OK[] = "OK\r";
//    char* reply = 0;
//
//    while(ReceivedLine()) // clears the buffer
//        GetString();
//
//    do
//    {
//       SendString(transmit, 0);
//
//        while(ReceivedLine() == 0);
//        reply = GetString();
//
//
//
//        if (strcmp(transmit, reply) == 0)
//        {
//            //SendString("Got the init string\r\n");
//        }
//
//
//        while(ReceivedLine() == 0);
//        reply = GetString();
//
//    } while (strcmp(OK, reply) != 0);
//
//
//    return 0;
//}

int HackBusyWait(unsigned char time)
{
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char k = 0;
    unsigned char l = 0;
    unsigned char m = 0;

    for(i = 0; i < time; ++i)
    {
        for(j = 0; j < time; ++j)
        {
            for(k = 0; k < time; ++k)
            {
                for(l = 0; l < time; ++l)
                {
                    m = l;
                }
            }
        }
    }
    return m;
}

int HackRockSend(unsigned char * message)
{
    char oldestReceivedChar = 255;
    char olderReceivedChar = 255;
    char oldReceivedChar = 255;
    char receivedChar = 255;
    char timeToGo = 0;
    int i = 0;

    //static char sentYet = 0;
    //static char prevHour = 0;
    //static char prevMin = 0;

    //if((sentYet == 0) || (message[7] != prevMin) || (message[8] != prevHour) )
    {
    //    sentYet = 1;
    //    prevMin = message[7];
    //    prevHour = message[8];

        SendString_UART1("AT\r");

        for(i = 0; i < 4; ++i)
        {
            timeToGo = 0;

            while(timeToGo == 0) //Wait for us to get the go ahead to continue
            {
                //Wait for response to be over
                if (U1STAbits.OERR) // If there is an overflow, do not read, and reset
                {
                    U1STAbits.OERR = 0;
                }
                if (U1STAbits.URXDA)  //If data available from Rockblock
                {
                    oldestReceivedChar = olderReceivedChar;
                    olderReceivedChar = oldReceivedChar;
                    oldReceivedChar = receivedChar;
                    receivedChar = U1RXREG; // Read the receive line of the PIC from the GPS

                    if(oldestReceivedChar == 'O' && olderReceivedChar == 'K' && oldReceivedChar == '\r' && receivedChar == '\n' )
                    {
                        timeToGo = 1;
                    }

                }
            }


            if(i == 0)
            {
                SendString_UART1("AT&K0\r");
            }
            else if(i == 1)
            {
                //SendString("AT+SBDWT=This message was successfully sent from PIC32MX360\r", 0);
                SendString_UART1("AT+SBDWT=");
                SendString_UART1(message);
                SendString_UART1("\r");
            }
            else if(i == 2)
            {
                SendString_UART1("AT+SBDIX\r");
            }
        }
    }

    return 0;
}

//int checkService()
//{
//    char service[] = "+CIEV:1,1\r";//network service available
//    char noservice[] = "+CIEV:1,0\r";//network service unavailable
//    char* reply = 0;
//
//    do
//    {
//        while(ReceivedLine() == 0);
//        reply = GetString();
//
//    } while (strcmp(service, reply) != 0);
//
//    return 0;
//}

void SendString_UART1(unsigned char* string)
{
    while(*string != 0)
    {
        SendChar_UART1(*(string++));
    }
}

void SendBuffer_UART1(char *buf, int start, int len) {
    int i;
    for (i=start;i<len;i++) {
        SendChar_UART1(buf[i]);
    }
}

void SendHex_UART1(char letter) {
    SendNybble_UART1((letter>>4)&0xF);
    SendNybble_UART1(letter&0xF);
}

void SendNybble_UART1(char n) {
    if (n>9)
        SendChar_UART1('A'+n-10);
    else
        SendChar_UART1('0'+n);
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

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

char rbstrcmp(const char *s1,int s1i,const char *s2) {
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