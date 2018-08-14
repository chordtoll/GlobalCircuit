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
    uint8_t yikes;
    uint16_t seq;
    uint32_t time;
    int32_t lat;
    int32_t lon;
    uint32_t alt;
    uint16_t horizL[12];
    uint16_t horizR[12];
    uint16_t horizD[12];
    uint16_t vertH;
    uint16_t vertL;
    uint16_t vertD;
    uint16_t compassX[12];
    uint16_t compassY[12];
    uint16_t cVertH[25];
    uint16_t cVertL[25];
};

struct __attribute__((packed)) s_packet_rare {
    uint8_t type;
    uint8_t yikes;
    uint16_t seq;
    uint32_t pressure;
    uint32_t temperature;
    uint16_t batvolt;
    uint16_t batcurr;
    uint16_t thermval;
    uint16_t vertH;
    uint16_t vertL;
    uint16_t vertD;
};

typedef union u_packet
{
    char bytes[340];
    struct s_packet_norm norm;
    struct s_packet_rare rare;
} packet_u;

#endif	/* PACKET_H */

