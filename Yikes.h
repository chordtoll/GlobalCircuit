/* 
 * File:   Yikes.h
 * Author: asent
 *
 * Created on August 1, 2018, 8:43 PM
 */

#ifndef YIKES_H
#define	YIKES_H

#include <proc/p32mx360f512l.h>

typedef union {
  struct {
    uint8_t reset    :1; //PIC32 was reset
    uint8_t looprate :1; //main loop took longer than the target time
    uint8_t gpstick  :1; //GPS ticking has gone out of expected bounds
    uint8_t rberror  :1; //RockBlock ran into an error
    uint8_t rbtime   :1; //RockBlock timed out
    uint8_t gpslock  :1; //GPS did not lock within a minute
    uint8_t idk      :2; //¯\_(ツ)_/¯
  };
  struct {
    uint8_t byte:8; //full flag byte
  };
} yikes_t;

yikes_t yikes; //initialize an instance of a yikes

#endif	/* YIKES_H */

