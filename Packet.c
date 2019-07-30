#include <proc/p32mx360f512l.h>
#include "Packet.h"

void Pack_Supervision(packet_u *pack)
{
            (*pack).norm.sup0._1u32=supTemperature;
            
            (*pack).norm.sup1._1u32=supPressure;
            
            (*pack).norm.sup2._2u16.a=supIl0;
            (*pack).norm.sup2._2u16.b=supIl1;

            (*pack).norm.sup3._2u16.a=supIl2;
            (*pack).norm.sup3._2u16.b=supIh0;

            (*pack).norm.sup4._2u16.a=supIh1;
            (*pack).norm.sup4._2u16.b=supIh2;

            (*pack).norm.sup5._2u16.a=supT0;
            (*pack).norm.sup5._2u16.b=supT1;

            (*pack).norm.sup6._2u16.a=supT2;
            (*pack).norm.sup6._2u16.b=supTmag;

            (*pack).norm.sup7._2u16.a=supText;
            (*pack).norm.sup7._2u16.b=supTRB;
}

void Pack_Conductivity(packet_u *pack, uint16_t sequence, uint16_t *cVert1, uint16_t *cVert2)
{
    #define COND_INTERWEAVE
    int i;
    for(i = 0; i < 15; ++i)
    {
        #ifdef COND_INTERWEAVE
            (*pack).norm.cVert1[i] = cVert1[i*10+(sequence%10)];
            (*pack).norm.cVert2[i] = cVert2[i*10+(sequence%10)];
        #else
            (*pack).norm.cVert1[i] = cVert1[i+15*(sequence%10)];
            (*pack).norm.cVert2[i] = cVert2[i+15*(sequence%10)];
        #endif

    }
}

void Pack_GPS(packet_u *pack, uint32_t time, uint32_t Ctime, uint32_t lat, uint32_t lon, uint32_t alt)
{
    (*pack).norm.time = time;
    (*pack).norm.Ctime = Ctime;
    (*pack).norm.lat = lat;
    (*pack).norm.lon = lon;
    (*pack).norm.alt = alt;
}

void Pack_Vert(packet_u *pack, uint16_t index, uint16_t vert1, uint16_t vert2, uint16_t vertD)
{
    (*pack).norm.vert1[index] = vert1;
    (*pack).norm.vert2[index] = vert2;
    (*pack).norm.vertD[index] = vertD;
}

void Pack_Horiz(packet_u *pack, uint16_t index, uint16_t h1, uint16_t h2, uint16_t hD)
{
    (*pack).norm.horiz1[index] = h1;
    (*pack).norm.horiz2[index] = h2;
    (*pack).norm.horizD[index] = hD;
}

void Pack_Mag(packet_u *pack, uint16_t index, uint16_t mx, uint16_t my, uint16_t mz)
{
    (*pack).norm.compassX[index] = mx;
    (*pack).norm.compassY[index] = my;
    (*pack).norm.compassZ[index] = mz;
}
