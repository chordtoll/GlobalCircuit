/* 
 * File:   RockBlock.h
 * Author: asent
 *
 * Created on August 1, 2018, 2:44 PM
 */

#ifndef ROCKBLOCK_H
#define	ROCKBLOCK_H

#include <stdint.h>

#define RB_IDLE_SOFT_TIMEOUT 3000
#define RB_IDLE_FIRM_TIMEOUT 4500
#define RB_IDLE_HARD_TIMEOUT 6000

//#define BALLAST RBRXbuf[2]==0xF0&&RBRXbuf[3]==0x9F&&RBRXbuf[4]==0x92&&RBRXbuf[5]==0xB0
//#define CUTDOWN RBRXbuf[2]==0xE2&&RBRXbuf[3]==0x9C&&RBRXbuf[4]==0x82&&RBRXbuf[5]==0x20
//#define REQUEST RBRXbuf[6]==0xF0&&RBRXbuf[7]==0x9F&&RBRXbuf[8]==0x99&&RBRXbuf[9]==0x83
//#define CONFIRM RBRXbuf[6]==0xF0&&RBRXbuf[7]==0x9F&&RBRXbuf[8]==0x91&&RBRXbuf[9]==0x8D

#define BALLAST RBRXbuf[0]=='B'&&RBRXbuf[1]=='A'&&RBRXbuf[2]=='L'&&RBRXbuf[3]=='L'
#define CUTDOWN RBRXbuf[0]=='C'&&RBRXbuf[1]=='U'&&RBRXbuf[2]=='T'&&RBRXbuf[3]=='D'
#define REQUEST RBRXbuf[4]=='R'&&RBRXbuf[5]=='E'&&RBRXbuf[6]=='Q'&&RBRXbuf[7]=='U'
#define CONFIRM RBRXbuf[4]=='C'&&RBRXbuf[5]=='O'&&RBRXbuf[6]=='N'&&RBRXbuf[7]=='F'


typedef enum rb_state  {RB_INIT,SENT_ATEo,SENT_ATnKo,SENT_SBDMTA,SENT_SBDDo,RB_IDLE,BEGINSEND,SENT_SBDWB,SENDING_TXBUF,SENT_TXBUF,SENT_CSUM,SENT_SBDIX,SENT_SBDRB,SENT_ACKAT} rb_state_t;
typedef enum rb_status {RB_BUSY,RB_OK,RB_ERROR,RB_READY} rb_status_t;

rb_state_t _rb_state;   //current rockblock state
uint16_t _rb_buf_sindex;//rockblock string buffer index counter

char _rb_mos[8];   //mobile originated message status
char _rb_mom[8];   //mobile originated message sequence number
char _rb_mts[8];   //mobile terminated message status
char _rb_mtm[8];   //mobile terminated message sequence number
char _rb_mtl[8];   //mobile terminated message length
char _rb_mtq[8];   //number of mobile terminated messaged queued
uint8_t _rb_imos;  //integer parsed mos
uint16_t _rb_imom; //integer parsed mom
uint8_t _rb_imts;  //integer parsed mts
uint16_t _rb_imtm; //integer parsed mtm
uint16_t _rb_imtl; //integer parsed mtl
uint16_t _rb_imtq; //integer parsed mtq

//initializes rockblock
void InitRB();

//advance the rockblock state machine
void TickRB();

//send a passed string over the rockblock
void SendString_RB(char *msg);

#endif	/* ROCKBLOCK_H */

