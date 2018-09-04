#include <stdint.h>
#include "RockBlock.h"
#include "transmit.h"
#include "Yikes.h"
#include "Ballast.h"

char OKARR[340];
char _rb_reqsend;
uint16_t _rb_idletimer;

void ParseSBDIX(volatile char *cmdbuf,char *mos,char *mom,char *mts,char *mtm,char *mtl,char *mtq);

void InitRB() {
    _rb_state=RB_INIT;
    _rb_idx=0;
    _rb_status=RB_BUSY;
    _rb_cmdbuf[340]=0;
    _rb_reqsend=0;
    _rb_idletimer=0;
    OKARR[0]='D';
    OKARR[1]='O';
    OKARR[2]='N';
    OKARR[3]='E';
    OKARR[4]=0x00;
}

void TickRB() {
    uint16_t csum=0;
    uint16_t i;
    if (_rb_state==RB_IDLE)
        _rb_idletimer=0;
    else
        _rb_idletimer++;
    /*
    if (_rb_idletimer==RB_IDLE_SOFT_TIMEOUT)
        _rb_state=RB_INIT;
    if (_rb_idletimer==RB_IDLE_FIRM_TIMEOUT)
        _rb_state=RB_INIT;
    if (_rb_idletimer==RB_IDLE_HARD_TIMEOUT)
        while(1);*/
    switch (_rb_state){
        case RB_INIT:
            SendString_UART1("ATE0\r");
            _rb_state=SENT_ATEo;
            _rb_status=RB_BUSY;
            break;
        case SENT_ATEo:
            if (_rb_status==RB_OK) {
                SendString_UART1("AT&K0\r");
                _rb_state=SENT_ATnKo;
                _rb_status=RB_BUSY;
            }
            if (_rb_status==RB_ERROR) {
                yikes.rberror=1;
                _rb_state=RB_INIT;
            }
            break;
        case SENT_ATnKo:
            if (_rb_status==RB_OK) {
                SendString_UART1("AT+SBDMTA=0\r");
                _rb_state=SENT_SBDMTA;
                _rb_status=RB_BUSY;
            }
            if (_rb_status==RB_ERROR) {
                yikes.rberror=1;
                _rb_state=RB_INIT;
            }
            break;
        case SENT_SBDMTA:
            if (_rb_status==RB_OK) {
                SendString_UART1("AT+SBDD0\r");
                _rb_state=SENT_SBDDo;
                _rb_status=RB_BUSY;
            }
            if (_rb_status==RB_ERROR) {
                yikes.rberror=1;
                _rb_state=RB_INIT;
            }
            break;
        case SENT_SBDDo:
            if (_rb_status==RB_OK) {
                _rb_state=RB_IDLE;
            }
            if (_rb_status==RB_ERROR) {
                yikes.rberror=1;
                _rb_state=RB_INIT;
            }
            break;
        case RB_IDLE:
            if (_rb_reqsend)
            {
                _rb_reqsend=0;
                _rb_state=BEGINSEND;
            }
            break;
        case BEGINSEND:
            SendString_UART1("AT+SBDWB=340\r");
            _rb_state=SENT_SBDWB;
            _rb_status=RB_BUSY;
            break;
        case SENT_SBDWB:
            if (_rb_status==RB_READY) {
                SendBuffer_UART1((char *)RBTXbuf,0,340);
                _rb_state=SENT_TXBUF;
                _rb_status=RB_BUSY;
            }
            if (_rb_status==RB_ERROR) {
                yikes.rberror=1;
                _rb_state=RB_INIT;
            }
            break;
        case SENT_TXBUF:
            for (i=0;i<340;i++)
                csum+=(uint8_t) RBTXbuf[i];
            SendChar_UART1(csum>>8);
            SendChar_UART1(csum&0xFF);
            _rb_state=SENT_CSUM;
            break;
        case SENT_CSUM:
            if (_rb_status==RB_OK) {
                SendString_UART1("AT+SBDIX\r");
                _rb_state=SENT_SBDIX;
                _rb_status=RB_BUSY;
            }
            if (_rb_status==RB_ERROR) {
                yikes.rberror=1;
                _rb_state=RB_INIT;
            }
            break;
        case SENT_SBDIX:
            if (_rb_status==RB_OK) {
                ParseSBDIX(_rb_cmdbuf,_rb_mos,_rb_mom,_rb_mts,_rb_mtm,_rb_mtl,_rb_mtq);
                _rb_imtl=atoi(_rb_mtl);
                if (_rb_imtl>0) {
                    SendString_UART1("AT+SBDRB\r");
                    _rb_state=SENT_SBDRB;
                    _rb_status=RB_BUSY;
                } else {
                    SendString_UART1("AT\r");
                    _rb_state=SENT_ACKAT;
                    _rb_status=RB_BUSY;
                }
            }
            if (_rb_status==RB_ERROR) {
                yikes.rberror=1;
                _rb_state=RB_INIT;
            }
            break;
        case SENT_ACKAT:
            if (_rb_status==RB_OK) {
                _rb_state=RB_IDLE;
            }
            if (_rb_status==RB_ERROR) {
                yikes.rberror=1;
                _rb_state=RB_INIT;
            }
            break;
        case SENT_SBDRB:
            if (_rb_status==RB_OK) {
                volatile char *rbuf=_rb_cmdbuf;
                uint16_t csumc=0;
                uint16_t msglen=*rbuf++<<8;
                msglen|=*rbuf++;
                for (i=0;i<msglen;i++) {
                    RBRXbuf[i]=*rbuf++;
                    csumc+=RBRXbuf[i];
                }
                RBRXbuf[msglen]=0;
                uint16_t csumr=*rbuf++<<8;
                csumr|=*rbuf;
                //SendString_UART1("!");
                //SendBuffer_UART1(RBRXbuf,0,20);
                //SendString_UART1("!\r");
                if (RBRXbuf[2]=='T' && RBRXbuf[3]=='E' && RBRXbuf[4]=='S' && RBRXbuf[5]=='T') {
                    DeployBallast(0);
                    //SendString_RB(OKARR);
                    _rb_state=RB_IDLE;
                } else if (RBRXbuf[2]=='C' && RBRXbuf[3]=='U' && RBRXbuf[4]=='T') {
                    SendString_GPIO("\xE2\x9C\x82\x20");
                    //SendString_RB(OKARR);
                    _rb_state=RB_IDLE;
                } else {
                    SendString_UART1("AT\r");
                    _rb_state=SENT_ACKAT;
                    _rb_status=RB_BUSY;
                }
            }
            if (_rb_status==RB_ERROR) {
                yikes.rberror=1;
                _rb_state=RB_INIT;
            }
            break;
    }
}

void SendString_RB(char *msg) {
    memcpy((void *)RBTXbuf,msg,340);
    _rb_reqsend=1;
}

void ParseSBDIX(volatile char *cmdbuf,char *mos,char *mom,char *mts,char *mtm,char *mtl,char *mtq) {
    uint8_t field=0;
    uint16_t fieldstart=0;
    uint16_t idx=0;
    while (*cmdbuf!='+')
        cmdbuf++;
    while (cmdbuf[idx]) {
        if (cmdbuf[idx]==',' || cmdbuf[idx]=='\r' || cmdbuf[idx]==':') {
            switch(field) {
                case 1:
                    mos[idx-fieldstart]=0;
                    break;
                case 2:
                    mom[idx-fieldstart]=0;
                    break;
                case 3:
                    mts[idx-fieldstart]=0;
                    break;
                case 4:
                    mtm[idx-fieldstart]=0;
                    break;
                case 5:
                    mtl[idx-fieldstart]=0;
                    break;
                case 6:
                    mtq[idx-fieldstart]=0;
                    break;
            }
            field++;
            fieldstart=idx+1;
        } else {
            switch(field) {
                case 1:
                    mos[idx-fieldstart]=cmdbuf[idx];
                    break;
                case 2:
                    mom[idx-fieldstart]=cmdbuf[idx];
                    break;
                case 3:
                    mts[idx-fieldstart]=cmdbuf[idx];
                    break;
                case 4:
                    mtm[idx-fieldstart]=cmdbuf[idx];
                    break;
                case 5:
                    mtl[idx-fieldstart]=cmdbuf[idx];
                    break;
                case 6:
                    mtq[idx-fieldstart]=cmdbuf[idx];
                    break;
            }
        }
        idx++;
    }
}