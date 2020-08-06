#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "RockBlock.h"
#include "UART.h"
#include "Yikes.h"
#include "Ballast.h"
#include "Cutdown.h"

char _rb_reqsend;        //flag indicating that a request to send data was made
uint8_t _rb_reqSigCheck; //flag indicating that a request to check signal strength was made
uint16_t _rb_idletimer;  //timer that keeps track of number of ticks RB has been busy consecutively

void ParseSBDI(volatile char *cmdbuf,char *mos,char *mom,char *mts,char *mtm,char *mtl,char *mtq);
uint8_t ParseCSQ(volatile char *cmdbuf);
uint16_t ParseSN(volatile char *cmdbuf);

void InitRB() {
    _rb_seq = RB_INIT;    //set rockblock to initialization state
    _rb_status = RB_OK;
    _rb_command_ind = 0;  
    _rb_idx = 0;          //set rockblock character index to 0
    _rb_cmdbuf[340]=0;    //clear the rockblock command buffer
    _rb_reqsend=0;        //clear reqsend flag
    _rb_reqSigCheck=0;    //clear signal check flag
    _rb_idletimer=0;      //reset idle timer

    uint8_t i;
    for(i = 0; i < PACKET_BUFFER_SIZE; ++i)
    {
    }
}

void ShiftPacketBuffer()
{
    uint8_t i;
    free(packet_buffer[0]);
    --num_stored_packets;
    for(i = 1; i < PACKET_BUFFER_SIZE; ++i)
    {
        packet_buffer[i-1] = packet_buffer[i];
    }
}

void CheckSig_RB()
{
    if(_rb_seq == RB_IDLE)
    {
        _rb_command_ind = 0;
        _rb_seq = RB_SIG;
    }
}

rb_command_resp_t RB_GetSerial()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT+CGSN\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_ReadSerial()
{
    if(_rb_status == RB_OK)
    {
        _rb_imei = ParseSN(_rb_cmdbuf);
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_Echo_Off()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("ATE0\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
    
}

rb_command_resp_t RB_Echo_On()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("ATE1\r");
        _rb_status = RB_COMMAND_NEXT;
        return 1;
    }
    else if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
    
}

rb_command_resp_t RB_FlowControl_Disable()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT&K0\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_FlowControl_Enable()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT&K3\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_DTR_Ignore()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT&D0\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
    
}

rb_command_resp_t RB_DTR_10Sec()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT&D1\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_DTR_Def()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT&D2\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_DTR_Reset()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT&D3\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_Ring_Disable()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT+SBDMTA=0\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_Ring_Enable()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT+SBDMTA=1\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_Clear_TxBuff()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT+SBDD0\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_Clear_RxBuff()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT+SBDD1\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_Clear_BothBuff()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT+SBDD2\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_CheckSig() 
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT+CSQ\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_ReadSig()
{
    if(_rb_status == RB_OK)
    {
        _rb_sig = ParseCSQ(_rb_cmdbuf);
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror = 1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_Rx()
{
    if(_rb_status == RB_OK)
    {
        ParseSBDI(_rb_cmdbuf,_rb_mos,_rb_mom,_rb_mts,_rb_mtm,_rb_mtl,_rb_mtq);
        _rb_imtl=atoi(_rb_mtl);
        _rb_imos = atoi(_rb_mos);
        if(_rb_imos == 1)
        {
            ShiftPacketBuffer();
        }
        if (_rb_imtl>0) 
        {
            SendString_UART1("AT+SBDRB\r");
            _rb_status = RB_BUSY;
            _rb_seq = RB_UPLINK;
        }
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror = 1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_WriteBuff()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT+SBDWB=340\r");
        _rb_status = RB_BUSY;
    }
    else if(_rb_status == RB_READY)
    {
        uint16_t csum = 0;
        uint16_t i;
        SendBuffer_UART1(packet_buffer[0], 0, 340);
        for(i=0;i<340;i++)
            csum+=(uint8_t) packet_buffer[0][i];
        SendChar_UART1(csum>>8);
        SendChar_UART1(csum&0xFF);
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    else if(_rb_status == RB_ERROR)
    {
        yikes.rberror = 1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_CheckWriteStatus()
{
    if(_rb_status == RB_OK)
    {
        volatile char *rbuf = _rb_cmdbuf;
        while(*rbuf == '\r' || *rbuf == '\n')
            ++rbuf;
        char boop = *rbuf;
        if(*rbuf == '0')
        {
            return RB_COMMAND_NEXT;
        }
        else
        {
            yikes.rberror = 1;
            return RB_COMMAND_RESET;
        }
    }
    else if(_rb_status == RB_ERROR)
    {
        yikes.rberror = 1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_Tx()
{
    if(_rb_status == RB_OK)
    {
        SendString_UART1("AT+SBDI\r");
        _rb_status = RB_BUSY;
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror=1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

rb_command_resp_t RB_ParseUplink()
{
    if(_rb_status == RB_OK)
    {
        volatile char *rbuf=_rb_cmdbuf + 2; //create an instance of the message buffer
        uint16_t msglen =_rb_imtl;     //pull the message length from the message buffer
        uint16_t i;
        for (i=0;i<msglen;i++) {        //loop through the message
            RBRXbuf[i]=*rbuf++;         //retrieve the message from the message buffer
        }
        RBRXbuf[msglen]=0;              //add a 0 terminator to the end of the message
        if(!ballast_rq && BALLAST && REQUEST)          //if a ballast request has come in
        {
            ballast_rq = 1;                            //set ballast_rq flag
        }
        else if(ballast_rq >= 3 && ballast_rq <= 12 && BALLAST && CONFIRM && ((RBRXbuf[8] >= '0' && RBRXbuf[8] <= '9') || (RBRXbuf[8] >= 'A' && RBRXbuf[8] <= 'F') || (RBRXbuf[8] >= 'a' && RBRXbuf[8] <= 'f'))) //if a ballast acknowledge is expected and was received
        {
            if(RBRXbuf[8] <= '9')
                DeployBallast(RBRXbuf[8] - '0');                          //begin ballast deployment
            else if (RBRXbuf[8] <= 'F')
                DeployBallast(RBRXbuf[8] - 'A' + 10);
            else
                DeployBallast(RBRXbuf[8] - 'a' + 10);
            ballast_rq = 0;                            //clear ballast_rq flag
        }
        else if(!cutdown_rq && CUTDOWN && REQUEST)     //if a cutdown request has come in
        {
            cutdown_rq = 1;                            //set cutdown_rq flag
        }
        else if(cutdown_rq >= 3 && cutdown_rq <= 12 && CUTDOWN && CONFIRM) //if a cutdown acknowledge is expected as was received
        {
             InitiateCutdown();                        //initiate a cutdown sequence
             cutdown_rq = 0;                           //clear the cutdown_rq flag
        }
        return RB_COMMAND_NEXT;
    }
    if(_rb_status == RB_ERROR)
    {
        yikes.rberror = 1;
        return RB_COMMAND_RESET;
    }
    return RB_COMMAND_HOLD;
}

void TickRB()
{
    switch(_rb_seq)
    {
        case RB_INIT:
            if(_rb_init_funcs[_rb_command_ind])
            {
                switch((*_rb_init_funcs[_rb_command_ind])())
                {
                    case RB_COMMAND_NEXT:
                        ++_rb_command_ind;
                        break;
                        
                    case RB_COMMAND_RESET:
                        _rb_command_ind = 0;
                        if(++_rb_errors == ERROR_LIMIT)
                        {
                            while(1);
                        }
                        break;
                        
                    case RB_COMMAND_HOLD:
                        break;
                }
            }
            else
            {
                _rb_command_ind = 0;
                _rb_seq = RB_IDLE;
            }
            break;
            
        case RB_TRANS:
            if(_rb_trans_funcs[_rb_command_ind])
            {
                switch((*_rb_trans_funcs[_rb_command_ind])())
                {
                    case RB_COMMAND_NEXT:
                        ++_rb_command_ind;
                        break;
                        
                    case RB_COMMAND_RESET:
                        if(++_rb_errors == ERROR_LIMIT)
                        {
                            while(1);
                        }
                        if(num_stored_packets > 0)
                            _rb_command_ind = 0;
                        else
                            _rb_seq = RB_IDLE;
                        break;
                        
                    case RB_COMMAND_HOLD:
                        break;
                }
            }
            else
            {
                _rb_command_ind = 0;
                _rb_seq = RB_IDLE;
            }
            break;
            
        case RB_UPLINK:
            switch(RB_ParseUplink())
            {
                case RB_COMMAND_NEXT:
                    _rb_command_ind = 0;
                    _rb_seq = RB_IDLE;
                    break;
                    
                case RB_COMMAND_RESET:
                    _rb_command_ind = 0;
                    if(++_rb_errors == ERROR_LIMIT)
                    {
                        while(1);
                    }
                    break;
                    
                case RB_COMMAND_HOLD:
                    break;
            }
            break;

        case RB_SIG:
            if(_rb_sig_funcs[_rb_command_ind])
            {
                switch((*_rb_sig_funcs[_rb_command_ind])())
                {
                    case RB_COMMAND_NEXT:
                        ++_rb_command_ind;
                        break;

                    case RB_COMMAND_RESET:
                        _rb_command_ind = 0;
                        if(++_rb_errors == ERROR_LIMIT)
                        {
                            while(1);
                        }
                        break;

                    case RB_COMMAND_HOLD:
                        break;
                }
                break;
            }
            else
            {
                _rb_command_ind = 0;
                _rb_seq = RB_IDLE;
            }

        case RB_IDLE:
            _rb_errors = 0;
            _rb_busy_ticks = 0;
            if(_rb_status == RB_ERROR)
            {
                _rb_seq = RB_INIT;
                _rb_command_ind = 0;
                yikes.rberror = 1;
            }
            break;
    }
    if(++_rb_busy_ticks == BUSY_TICK_MAX)
    {
        while(1);
    }
}

void SendPacket_RB() {
    if(num_stored_packets > 0)
    {
        _rb_seq = RB_TRANS;                   //request to send the message
    }
}

uint8_t ParseCSQ(volatile char *cmdbuf)
{
    while(*cmdbuf!=':')                        //move through the command buffer until a ':' is found
        ++cmdbuf;
    while(!(*cmdbuf >= '0' && *cmdbuf <= '9')) //move through the command buffer until a number is found
        ++cmdbuf;
        return (*cmdbuf) - '0';                //return the signal strength
}

uint16_t ParseSN(volatile char *cmdbuf)
{
    uint8_t i;                               //looping variable
    uint16_t scale = 1000;                   //scalar for adding to the SN
    uint16_t sn = 0;                         //serial number reading
    while(*cmdbuf<'0' || *cmdbuf>'9')        //loop through characters until a number is reached
        ++cmdbuf;
    for(i=0; i < 11; ++i)                    //loop through the next 11 characters
        ++cmdbuf;
    while(scale)                             //loop until 4 digits have been read
    {
        sn += (*cmdbuf++ - '0') * scale;
        scale /= 10;
    }
    return sn;                               //return the last 4 digits of the serial number
}

void ParseSBDI(volatile char *cmdbuf,char *mos,char *mom,char *mts,char *mtm,char *mtl,char *mtq) {
    uint8_t field=0;       //counter for which field is currently being read in
    uint16_t fieldstart=0; //starting index of current field
    uint16_t idx=0;        //current index
    while (*cmdbuf!='+')   //loop until initiating character is reached
        cmdbuf++;          //move to the next character
    while (cmdbuf[idx]) {  //loop until terminating character is reached
        if (cmdbuf[idx]==',' || cmdbuf[idx]=='\r' || cmdbuf[idx]==':') { //if a separating character is read
            switch(field) {               
                case 1:                   //if we are looking at the mos field,
                    mos[idx-fieldstart]=0;//add a terminating character to the end of the mos field
                    break;
                case 2:                   //if we are looking at the mom field,
                    mom[idx-fieldstart]=0;//add a terminating character to the end of the mom field
                    break;
                case 3:                   //if we are looking at the mts field,
                    mts[idx-fieldstart]=0;//add a terminating character to the end of the mts field
                    break;
                case 4:                   //if we are looking at the mtm field,
                    mtm[idx-fieldstart]=0;//add a terminating character to the end of the mtm field
                    break;
                case 5:                   //if we are looking at the mtl field,
                    mtl[idx-fieldstart]=0;//add a terminating character to the end of the mtl field
                    break;
                case 6:                   //if we are looking at the mtq field,
                    mtq[idx-fieldstart]=0;//add a terminating character to the end of the mtq field
                    break;
            }
            field++;          //move to the next field
            fieldstart=idx+1; //update the fieldstart index
        } else {                                     //if a data character is read
            switch(field) {
                case 1:                              //if we are looking at the mos field,
                    mos[idx-fieldstart]=cmdbuf[idx]; //add the data character to the mos field
                    break;
                case 2:                              //if we are looking at the mom field,
                    mom[idx-fieldstart]=cmdbuf[idx]; //add the data character to the mom field
                    break;
                case 3:                              //if we are looking at the mts field,
                    mts[idx-fieldstart]=cmdbuf[idx]; //add the data character to the mts field
                    break;
                case 4:                              //if we are looking at the mtm field,
                    mtm[idx-fieldstart]=cmdbuf[idx]; //add the data character to the mtm field
                    break;
                case 5:                              //if we are looking at the mtl field,
                    mtl[idx-fieldstart]=cmdbuf[idx]; //add the data character to the mtl field
                    break;
                case 6:                              //if we are looking at the mtq field,
                    mtq[idx-fieldstart]=cmdbuf[idx]; //add the data character to the mtq field
                    break;
            }
        }
        idx++; //move to the next character
    }
}

void InsertPacketBuffer(char* msg)
{
    char* new = malloc(340);                       //allocate space for new packet
    memcpy(new, msg, 340);                         //move message into new space
    if(num_stored_packets == PACKET_BUFFER_SIZE)   //if packet buffer is full,
    {
        ShiftPacketBuffer();                       //drop oldest packet
    }
    packet_buffer[num_stored_packets++] = new;     //add new packet to end of buffer
}
