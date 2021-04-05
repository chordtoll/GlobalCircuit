#ifndef ROCKBLOCK_H
#define	ROCKBLOCK_H

#include <stdint.h>
#include <stddef.h>
#include "Packet.h"

#define BUSY_TICK_MAX 600
#define PACKET_BUFFER_SIZE 3
#define PACKET_SIZE 340

#define BALLAST RBRXbuf[0]=='B'&&RBRXbuf[1]=='A'&&RBRXbuf[2]=='L'&&RBRXbuf[3]=='L'
#define CUTDOWN RBRXbuf[0]=='C'&&RBRXbuf[1]=='U'&&RBRXbuf[2]=='T'&&RBRXbuf[3]=='D'
#define REQUEST RBRXbuf[4]=='R'&&RBRXbuf[5]=='E'&&RBRXbuf[6]=='Q'&&RBRXbuf[7]=='U'
#define CONFIRM RBRXbuf[4]=='C'&&RBRXbuf[5]=='O'&&RBRXbuf[6]=='N'&&RBRXbuf[7]=='F'


typedef enum rb_status {RB_BUSY, RB_OK, RB_ERROR, RB_READY} rb_status_t;
typedef enum rb_seq {RB_INIT, RB_TRANS, RB_UPLINK, RB_SIG, RB_IDLE} rb_seq_t;
typedef enum rb_command_resp {RB_COMMAND_NEXT, RB_COMMAND_RESET, RB_COMMAND_HOLD} rb_command_resp_t;

char packet_buffer[PACKET_BUFFER_SIZE][PACKET_SIZE];

uint8_t num_stored_packets = 0;
uint8_t active_packet_index = 0;

rb_seq_t _rb_seq;
uint8_t _rb_command_ind;
uint16_t _rb_busy_ticks = 0;

uint16_t _rb_buf_sindex=0;//rockblock string buffer index counter
uint8_t uplink_count=0;
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

//get the RockBLOCK's serial number (AT+CGSN) MUST USE RB_ReadSerial AFTER THIS
rb_command_resp_t RB_GetSerial(); 

//parse the return of AT+CGSN, MUST BE USED AFTER RB_GETSERIAL
rb_command_resp_t RB_ReadSerial();

//characters are not echoed to the DTE (ATE0)
rb_command_resp_t RB_Echo_Off();

//characters are echoed to the DTE (ATE1)
rb_command_resp_t RB_Echo_On();

//Disables flow control (AT&K0)
rb_command_resp_t RB_FlowControl_Disable();

//Enables RTS/CTS flow control (AT&K3)
rb_command_resp_t RB_FlowControl_Enable();

//DTR is ignored (AT&D0)
rb_command_resp_t RB_DTR_Ignore();

//DTR is used with a 10 second delay (AT&D1)
rb_command_resp_t RB_DTR_10Sec();

//DTR is used immediately (AT&D2)
rb_command_resp_t RB_DTR_Def();

//DTR is used immediately, and resets to AT command profile 0 (AT&D3)
rb_command_resp_t RB_DTR_Reset();

//disable ring indication (AT+SBDMTA=0)
rb_command_resp_t RB_Ring_Disable();

//enable ring indication (AT+SBDMTA=1)
rb_command_resp_t RB_Ring_Enable();

//Clear mobile originated (Tx) buffer (AT+SBDD0)
rb_command_resp_t RB_Clear_TxBuff();

//Clear mobile terminated (Rx) buffer (AT+SBDD1)
rb_command_resp_t RB_Clear_RxBuff();

//Clear both buffers (AT+SBDD2)
rb_command_resp_t RB_Clear_BothBuff();

//start a message write
rb_command_resp_t RB_StartWrite();

//Write packet to RockBLOCK
rb_command_resp_t RB_WriteBuff();

//Checks status response after MO buffer is written to
rb_command_resp_t RB_CheckWriteStatus();

//Sends "AT+SBDI" to RB
rb_command_resp_t RB_Tx();

rb_command_resp_t RB_Rx();

//Sends "AT+CSQ" to rockblock, USE RB_ReadSig AFTER THIS CALL
rb_command_resp_t RB_CheckSig();

//reads AT+CSQ response from rockblock MUST USE AFTER RB_CheckSig
rb_command_resp_t RB_ReadSig();

//initializes rockblock
void InitRB();

//advance the rockblock state machine
void TickRB();

//triggers a signal check for the RockBLOCK
void CheckSig_RB();

//send the oldest packet in the packet buffer to the RockBLOCK
void SendPacket_RB();

//insert a packet into the message buffer for sending later.
void InsertPacketBuffer(char* msg);

rb_command_resp_t (* const _rb_init_funcs[])() = {RB_Echo_Off, RB_FlowControl_Disable, RB_DTR_Ignore, RB_Ring_Disable, RB_GetSerial, RB_ReadSerial, NULL};
rb_command_resp_t (* const _rb_trans_funcs[])() = {RB_StartWrite, RB_WriteBuff, RB_CheckWriteStatus, RB_Tx, RB_Rx, NULL};
rb_command_resp_t (* const _rb_sig_funcs[])() = {RB_CheckSig, RB_ReadSig, NULL};

#endif	/* ROCKBLOCK_H */

