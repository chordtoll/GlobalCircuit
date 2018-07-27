/* 
 * File:   Packet.h
 * Author: asent
 *
 * Created on July 26, 2018, 12:47 PM
 */

#include <stdint.h>

#ifndef PACKET_H
#define	PACKET_H

struct __attribute__((packed)) s_packet_norm {
    uint8_t type;
    uint32_t time;
    int64_t lat;
    int64_t lon;
    uint16_t alt;
    uint16_t horizL[12];
    uint16_t horizR[12];
    uint16_t horizD[12];
    uint16_t vertH;
    uint16_t vertL;
    uint16_t vertD;
    uint16_t compassX[12];
    uint16_t compassY[12];
    uint8_t end;
};

struct __attribute__((packed)) s_packet_rare {
    uint8_t type;
    uint32_t pressure    : 24;
    uint32_t temperature : 24;
    uint16_t batvolt     : 12;
    uint16_t batcurr     : 12;
    uint16_t thermval    : 12;
    uint16_t vertD[60];
    uint8_t end;
};

typedef union u_packet
{
    char bytes[256];
    struct s_packet_norm norm;
    struct s_packet_rare rare;
} packet_u;

#endif	/* PACKET_H */

