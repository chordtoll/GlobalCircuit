/* 
 * File:   RockBlock.h
 * Author: asent
 *
 * Created on August 1, 2018, 2:44 PM
 */

#ifndef ROCKBLOCK_H
#define	ROCKBLOCK_H

#include <stdint.h>

#define RB_BEGIN_AT "+++\r"
#define RB_NO_FLOWC "AT&K0\r"
#define RB_NO_DTR "AT&D0\r"
#define RB_NO_RINGID "AT+SBDMTA=0\r"
#define RB_CLEARBUFF "AT+SBDD0\r"
#define RB_WRITE340 "AT+SBDWB=340\r"
#define RB_SEND "AT+SBDIX\r"
#define RB_RECEIVE "AT+SBDRB\r"
#define RB_RESET "ATZ0\r"
#define RB_READSIG "AT+CSQ\r"
#define RB_READIMEI "AT+CGSN\r"

#define RB_IDLE_SOFT_TIMEOUT 600
#define RB_IDLE_FIRM_TIMEOUT 1200
#define RB_IDLE_HARD_TIMEOUT 1800

#define BALLAST RBRXbuf[0]=='B'&&RBRXbuf[1]=='A'&&RBRXbuf[2]=='L'&&RBRXbuf[3]=='L'
#define CUTDOWN RBRXbuf[0]=='C'&&RBRXbuf[1]=='U'&&RBRXbuf[2]=='T'&&RBRXbuf[3]=='D'
#define REQUEST RBRXbuf[4]=='R'&&RBRXbuf[5]=='E'&&RBRXbuf[6]=='Q'&&RBRXbuf[7]=='U'
#define CONFIRM RBRXbuf[4]=='C'&&RBRXbuf[5]=='O'&&RBRXbuf[6]=='N'&&RBRXbuf[7]=='F'


typedef enum rb_state  {TX_REQUESTED, } rb_state_t;
typedef enum rb_status {RB_BUSY,RB_OK,RB_ERROR,RB_READY} rb_status_t;

rb_state_t _rb_state;   //current rockblock state
uint16_t _rb_buf_sindex;//rockblock string buffer index counter
uint8_t command_count=0;
uint16_t _rb_imei;

char _rb_mos[8];   //mobile originated message status
char _rb_mom[8];   //mobile originated message sequence number
char _rb_mts[8];   //mobile terminated message status
char _rb_mtm[8];   //mobile terminated message sequence number
char _rb_mtl[8];   //mobile terminated message length
char _rb_mtq[8];   //number of mobile terminated messaged queued

uint8_t _rb_sig;   //integer parsed signal strength
uint8_t _rb_imos;  //integer parsed mos
uint16_t _rb_imom; //integer parsed mom
uint8_t _rb_imts;  //integer parsed mts
uint16_t _rb_imtm; //integer parsed mtm
uint16_t _rb_imtl; //integer parsed mtl
uint16_t _rb_imtq; //integer parsed mtq

//sends "AT" to RB
void StartATRB();

/*sends "AT&Kn" to RB
0 - Disable
3 - Enable RTS/CTS
4 - Enable XON/XOFF
6 - Enable RTS/CTS and XON/OFF*/
void FlowControlRB(uint8_t option);

/*sends "AT&D0" to RB
0 - DTR is ignored
1 - DTR is used with a 10 second delay
2 - DTR is used immediately
3 - DTR is used immediately, and resets to AT command profile 0*/
void DtrRB(uint8_t option);

/*sends "AT+SBDMTA=n" to RB
0 - disable ring ID
1 - enable ring ID*/
void RingIdRB(uint8_t option);

/*sends "AT+SBDDn" to RB
0 - Clear mobile originated (Tx) buffer
1 - Clear mobile terminated (Rx) buffer
2 - Clear both buffers*/
void ClearBuffRB(uint8_t option);

//Sends "AT+SBDWB=340" to RB
void PrepBuffRB();

//Writes 34 characters of TXbuffer to RB
void WriteBuffRB();

//Writes checksum to RB
void WriteCsRB();

//Sends "AT+SBDIX" to RB
void TransactMessageRB();

//initializes rockblock
void InitRB();

//advance the rockblock state machine
void TickRB();

//send a passed string over the rockblock
void SendString_RB(char *msg);

//update _rb_sig with current signal strength
void CheckSig_RB();
#endif	/* ROCKBLOCK_H */

