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
    uint8_t cutdown0 :1; //00: cutdown not in progress | 01: unexpected message returned | 10: no message returned | 11: cutdown in progress
    uint8_t cutdown1 :1;
    uint8_t idk      :1; //¯\_(ツ)_/¯
  };
  struct {
    uint8_t byte:8; //full flag byte
  };
} yikes_t;

yikes_t yikes; //initialize an instance of a yikes

#endif	/* YIKES_H */

