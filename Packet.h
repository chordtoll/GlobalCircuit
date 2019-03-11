/* 
 * File:   Packet.h
 * Author: asent
 *
 * Created on July 26, 2018, 12:47 PM
 */

#include <stdint.h>

#ifndef PACKET_H
#define	PACKET_H

//PACKET CHANGELOG:
//1->2:Add rbtime field to yikes byte

#define PACKET_VERSION 255

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
    uint16_t vert1;         //Vertical probe 1 electric field- raw adc value
    uint16_t vert2;         //Vertical probe 2 electric field- raw adc value
    uint16_t vertD;         //Vertical delta electric field- raw adc value
    uint16_t compassX[12];  //Magnetic field x component- raw compass value, 1 per 5 seconds
    uint16_t compassY[12];  //Magnetic field y component- raw compass value, 1 per 5 seconds
    uint16_t cVert1[15];    //Vertical probe 1 conductivity data- raw adc value
    uint16_t cVert2[15];    //Vertical probe 2 conductivity data- raw adc value,
    sup_t sup;              //Supervision Field
    uint8_t ballast;        //Ballast status flags
    uint8_t cutdown;        //Cutdown status flags
};

typedef union u_packet
{
    char bytes[340];
    struct s_packet_norm norm;
} packet_u;

#endif	/* PACKET_H */

