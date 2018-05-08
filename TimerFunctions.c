/*********************************************************************
 * Functions to read the core timer of the pic
 ********************************************************************/
/* This module is built for the generic library in addition to the
   device-specific libraries. */
#ifndef __32MXGENERIC__
#define __32MXGENERIC__
#endif
#include <xc.h>
#include "TimerFunctions.h"

unsigned int __attribute__((nomips16)) ReadCoreTimer(void)
{
    unsigned int timer;

    // get the count reg
    asm volatile("mfc0   %0, $9" : "=r"(timer));

    return timer;
}

////////////////////////////////////////////////////////////////////////////////
void __attribute__((nomips16)) WriteCoreTimer(unsigned int timer)
{
    /* get the count reg */
    asm volatile("mtc0   %0, $9": "+r"(timer));

}


