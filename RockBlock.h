/* 
 * File:   RockBlock.h
 * Author: asent
 *
 * Created on August 1, 2018, 2:44 PM
 */

#ifndef ROCKBLOCK_H
#define	ROCKBLOCK_H

#include <stdint.h>

typedef enum rb_state  {RB_INIT,SENT_ATEo,SENT_ATnKo,SENT_SBDMTA,SENT_SBDDo,RB_IDLE,BEGINSEND,SENT_SBDWB,SENT_TXBUF,SENT_CSUM,SENT_SBDIX,SENT_SBDRB,SENT_ACKAT} rb_state_t;
typedef enum rb_status {RB_BUSY,RB_OK,RB_ERROR,RB_READY} rb_status_t;

rb_state_t  _rb_state;

char _rb_mos[8];
char _rb_mom[8];
char _rb_mts[8];
char _rb_mtm[8];
char _rb_mtl[8];
char _rb_mtq[8];
uint8_t _rb_imos;
uint16_t _rb_imom;
uint8_t _rb_imts;
uint16_t _rb_imtm;
uint16_t _rb_imtl;
uint16_t _rb_imtq;

void InitRB();
void TickRB();
void SendString_RB(char *msg);

#endif	/* ROCKBLOCK_H */

