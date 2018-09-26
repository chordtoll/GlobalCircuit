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

#define PACKET_VERSION 2

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
    uint8_t version;
    uint8_t yikes;
    uint16_t seq;
    uint32_t time;
    uint32_t lat;
    uint32_t lon;
    uint32_t alt;
    uint16_t horiz1[12];
    uint16_t horiz2[12];
    uint16_t horizD[12];
    uint16_t vert1;
    uint16_t vert2;
    uint16_t vertD;
    uint16_t compassX[12];
    uint16_t compassY[12];
    uint16_t cVert1[15];
    uint16_t cVert2[15];
    sup_t sup;
};

typedef union u_packet
{
    char bytes[340];
    struct s_packet_norm norm;
} packet_u;

#endif	/* PACKET_H */

