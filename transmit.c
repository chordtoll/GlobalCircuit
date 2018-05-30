#include "transmit.h"
#include "proc\p32mx360f512l.h"
#define MAX_LINE 50
volatile char receive1 [MAX_LINE];
volatile char receive2 [MAX_LINE];
volatile char string1 [MAX_LINE];
volatile char string2 [MAX_LINE];
volatile char intIndex = 0;
volatile char receivedLine = 0;
volatile int bufferflag = 0;

void UARTInit()
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
    
    U1MODEbits.ON=1;
    U2MODEbits.ON=1;

    /*

    //Setting up UART1 Receive with Interrupts

    IEC0bits.U1RXIE = 1;     //Interrupt is enabled for UART1 receive
    IPC6bits.U1IP = 7;       //UART1 interrupt priority is 1
    IPC6bits.U1IS = 3;       //UART1 subpriority is 0
    U1STAbits.URXISEL = 0;   //Receive interrupt mode flag is set when character is received
    U1STAbits.URXEN = 1;     //Enable UART1 Receiver
    U1MODEbits.BRGH = 0;

    */

}

void InterruptInit()
{
    INTCONbits.MVEC = 1;  //enabling multivector interrupts
    IFS0bits.U1RXIF = 0; // clearing uart1 interrupt flag
    __asm__("EI");  // enable interrupts
}

void  __attribute__((vector(_UART_1_VECTOR), interrupt(IPL7SRS), nomips16)) UART1_ISR(void)
{
    char receivedChar = U1RXREG; //get char from uart1rec line

    //if char is a carriage return and the intIndex is
    if (receivedChar == '\r' && intIndex != 0  && intIndex < 47)
    {

        if (bufferflag == 0)
        {
            receive1[intIndex++] = '\r';
            receive1[intIndex] = 0;
        }

        if (bufferflag != 0)
        {
            receive2[intIndex++] = '\r';
            receive2[intIndex] = 0;
        }

        ++receivedLine;
        intIndex = 0;
        bufferflag = !bufferflag;

    }
    else if (receivedChar != '\n' && receivedChar != '\r'  && intIndex < 47)
    {
        if (bufferflag == 0)
        {
            receive1[intIndex++] = receivedChar;
        }

        if (bufferflag != 0)
        {
            receive2[intIndex++] = receivedChar;
        }
    }
    else if (intIndex == 47 && receivedChar != '\r')
    {
        if (bufferflag == 0)
        {
            receive1[intIndex++] = receivedChar;
            receive1[intIndex++] = '\r';
            receive1[intIndex] = 0;
        }

        if (bufferflag != 0)
        {
            receive2[intIndex++] = receivedChar;
            receive2[intIndex++] = '\r';
            receive2[intIndex] = 0;
        }
            ++receivedLine;
            intIndex = 0;
            bufferflag = !bufferflag;
    }

    IFS0bits.U1RXIF = 0; //clear interrupt flag status for UART1 receive

}

int RockInit()
{
    char transmit[] = "AT+CIER=1,0,1\r";
    char OK[] = "OK\r";
    char* reply = 0;

    while(ReceivedLine()) // clears the buffer
        GetString();

    do
    {
       SendString(transmit, 0);

        while(ReceivedLine() == 0);
        reply = GetString();



        if (strcmp(transmit, reply) == 0)
        {
            //SendString("Got the init string\r\n");
        }


        while(ReceivedLine() == 0);
        reply = GetString();

    } while (strcmp(OK, reply) != 0);


    return 0;
}

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

        SendString("AT\r", 0);

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
                SendString("AT&K0\r", 0);
            }
            else if(i == 1)
            {
                //SendString("AT+SBDWT=This message was successfully sent from PIC32MX360\r", 0);
                SendString("AT+SBDWT=", 0);
                SendString(message, 0);
                SendString("\r", 0);
            }
            else if(i == 2)
            {
                SendString("AT+SBDIX\r", 0);
            }
        }
    }

    return 0;
}

int checkService()
{
    char service[] = "+CIEV:1,1\r";//network service available
    char noservice[] = "+CIEV:1,0\r";//network service unavailable
    char* reply = 0;

    do
    {
        while(ReceivedLine() == 0);
        reply = GetString();

    } while (strcmp(service, reply) != 0);

    return 0;
}

void SendString(unsigned char* string, char checksum)
{
    int sum = 0;
    int i;

    if (checksum == 0)
    {
        while(*string != 0)
        {
            SendChar(*(string++));
        }
    }
    else
    {
        for (i = 0; i < 340; ++i)
        {
            SendChar(*string);
            sum += *(string++);
        }
        //sum = 0xFE01;
        SendChar(sum/256);
        SendChar(sum%256);
    }
}

void SendChar(char letter)
{
    while(U1STAbits.TRMT == 0){} //while transmitting don't do anything
    U1TXREG = letter;            //transmit first char
}

char ReceivedLine(void)
{
    return receivedLine;
}

volatile char* GetString(void)
{
    --receivedLine;

    if (bufferflag != 0)
    {

        return receive1;
    }

    return receive2;
}

//int strlen(char* string)
//{
//    int count = 0;
//    while(*(string++))
//        ++count;
//
//    return count;
//}

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
