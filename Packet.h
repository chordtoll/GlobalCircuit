/* 
 * File:   Packet.h
 * Author: asent
 *
 * Created on July 26, 2018, 12:47 PM
 */

#include <stdint.h>

#ifndef PACKET_H
#define	PACKET_H

//SUPERVISION VALUES
uint32_t supTemperature=0; //temperature
uint32_t supPressure=0;    //pressure
uint16_t supVbatP;         //PICADC channel 4
uint16_t supVbatN;         //PICADC channel 8
uint16_t supVbat3V6;       //PICADC channel 10
uint16_t supI1;            //PICADC channel 5
uint16_t supI2;            //PICADC channel 9
uint16_t supT1;            //PICADC channel 0
uint16_t supT2;            //PICADC channel 1
uint8_t supTmag;           //magnetometer temperature
uint16_t supText;          //external payload temperature
uint16_t supTRB;           //RockBlock temperature
uint16_t supTadc1;         //adc1 temperature
uint16_t supTadc2;         //adc2 temperature

//PACKET CHANGELOG:
//1->2:Add rbtime field to yikes byte
//2->3:Add vertical fields to match horizontal fields
//     Add CompassZ field
//     Add supTmag, supTadc1, and supTadc2 to SUP field on sequence 6 and 7
//3->4:Add Ballast and Cutdown status fields
//     Conductivity values now sent in an interweaved pattern
//4->5:Add Ctime field
//     Add conpol yikes bit
//5->6:Add fields for all supervision values
//     Add GPSSats field
//     Add RBSig field
//     Add epochdays field
//     Add commcount field
//     Add RBIMEI field

#define PACKET_VERSION 6

#define PAYLOAD_ID 0
#define EPOCH_TIME

typedef union u_sup {
    uint32_t _1u32;
    struct __attribute__((packed)) {
        uint16_t a;
        uint16_t b;
    } _2u16;
    struct __attribute__((packed)) {
        uint8_t a;
        uint8_t b;
        uint8_t c;
        uint8_t d;
    } _4u8;
} sup_t;

struct __attribute__((packed)) s_packet_norm {
    uint8_t version;        //Increments with each change to packet structure
    uint8_t yikes;          //Error code bits
    uint16_t seq;           //Sequence ID- 0 on boot, increments each packet
    uint32_t time;          //GPS time
    uint32_t lat;           //GPS latitude
    uint32_t lon;           //GPS longitude
    uint32_t alt;           //GPS altitude
    uint16_t horiz1[12];    //Horizontal probe 1 electric field- raw adc value, 1 per 5 seconds
    uint16_t horiz2[12];    //Horizontal probe 2 electric field- raw adc value, 1 per 5 seconds
    uint16_t horizD[12];    //Horizontal delta electric field- raw adc value
    uint16_t vert1[12];     //Vertical probe 1 electric field- raw adc value
    uint16_t vert2[12];     //Vertical probe 2 electric field- raw adc value
    uint16_t vertD[12];     //Vertical delta electric field- raw adc value
    uint16_t compassX[12];  //Magnetic field x component- raw compass value, 1 per 5 seconds
    uint16_t compassY[12];  //Magnetic field y component- raw compass value, 1 per 5 seconds
    uint16_t compassZ[12];  //Magnetic field z component- raw compass value, 1 per 5 seconds
    uint16_t cVert1[15];    //Vertical probe 1 conductivity data- raw adc value
    uint16_t cVert2[15];    //Vertical probe 2 conductivity data- raw adc value,
    uint8_t ballast;        //Ballast status flags
    uint8_t cutdown;        //Cutdown status flags
    uint32_t Ctime;         //GPS time of conductivity measurement
    uint8_t GPSSats;        //number of sattelites the GPS is using
    uint8_t RBSig;          //signal strength of Rockblock
    uint8_t commcount;      //number of commands sent to the RockBLOCK
    sup_t sup0;             //supervision values
    sup_t sup1;
    sup_t sup2;
    sup_t sup3;
    sup_t sup4;
    sup_t sup5;
    sup_t sup6;
    sup_t sup7;
    uint16_t RBIMEI;        //rockblock IMEI
};

typedef union u_packet
{
    char bytes[340];
    struct s_packet_norm norm;
} packet_u;


//update SUP field of packet with supervision value, rotated based on sequence number
void Pack_Supervision(packet_u *pack);

//update conductivity fields of packet with 1/10 of conductivity data, slice based on sequence
void Pack_Conductivity(packet_u *pack, uint16_t sequence, uint16_t *cVert1, uint16_t *cVert2);

//update GPS fields of packet with GPS data
void Pack_GPS(packet_u *pack, uint32_t time, uint32_t Ctime, uint32_t lat, uint32_t lon, uint32_t alt, uint8_t gSats);

//update vertical probe fields of packet with vertical probe data
void Pack_Vert(packet_u *pack, uint16_t index, uint16_t vert1, uint16_t vert2, uint16_t vertD);

//update given index of horizontal probe fields of packet with horizontal probe data
void Pack_Horiz(packet_u *pack, uint16_t index, uint16_t h1, uint16_t h2, uint16_t hD);

//update given index of magnetometer fields of packet with magnetometer data
void Pack_Mag(packet_u *pack, uint16_t index, uint16_t mx, uint16_t my, uint16_t mz);


#endif	/* PACKET_H */

