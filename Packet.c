#include <proc/p32mx360f512l.h>
#include "Packet.h"

void Pack_Supervision(packet_u *pack, uint16_t sequence)
{
    switch (sequence%10) {
        case 0: //temperature
            (*pack).norm.sup._1u32=supTemperature;
            break;
        case 1: //pressure
            (*pack).norm.sup._1u32=supPressure;
            break;
        case 2: //PICADC channels 4 and 8
            (*pack).norm.sup._2u16.a=supIl0;
            (*pack).norm.sup._2u16.b=supIl1;
            break;
        case 3: //PICADC channels 10 and 5
            (*pack).norm.sup._2u16.a=supIl2;
            (*pack).norm.sup._2u16.b=supIh0;
            break;
        case 4: //PICADC channels 9 and 11
            (*pack).norm.sup._2u16.a=supIh1;
            (*pack).norm.sup._2u16.b=supIh2;
            //ChargeProbe(NONE);
            break;
        case 5: //PICADC channels 0 and 1
            (*pack).norm.sup._2u16.a=supT0;
            (*pack).norm.sup._2u16.b=supT1;
            break;
        case 6: //PICADC channel 3
            (*pack).norm.sup._2u16.a=supT2;
            (*pack).norm.sup._2u16.b=0;
            break;
    }
}

void Pack_Conductivity(packet_u *pack, uint16_t sequence, uint16_t *cVert1, uint16_t *cVert2)
{
    memcpy((*pack).norm.cVert1,cVert1+(sequence%10)*15,15*2);
    memcpy((*pack).norm.cVert2,cVert2+(sequence%10)*15,15*2);
}

void Pack_GPS(packet_u *pack, uint32_t time, uint32_t lat, uint32_t lon, uint32_t alt)
{
    (*pack).norm.time = time;
    (*pack).norm.lat = lat;
    (*pack).norm.lon = lon;
    (*pack).norm.alt = alt;
}

void Pack_Vert(packet_u *pack, uint16_t vert1, uint16_t vert2, uint16_t vertD)
{
    (*pack).norm.vert1 = vert1;
    (*pack).norm.vert2 = vert2;
    (*pack).norm.vertD = vertD;
}

void Pack_Horiz(packet_u *pack, uint16_t index, uint16_t h1, uint16_t h2, uint16_t hD)
{
    (*pack).norm.horiz1[index] = h1;
    (*pack).norm.horiz2[index] = h2;
    (*pack).norm.horizD[index] = hD;
}

void Pack_Mag(packet_u *pack, uint16_t index, uint16_t mx, uint16_t my)
{
    (*pack).norm.compassX[index] = mx;
    (*pack).norm.compassY[index] = my;
}
