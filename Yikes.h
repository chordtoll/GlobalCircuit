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
    uint8_t reset   :1;
    uint8_t looprate:1;
    uint8_t gpstick :1;
    uint8_t rberror :1;
    uint8_t idk     :4;
  };
  struct {
    uint8_t byte:8;
  };
} yikes_t;

yikes_t yikes;

#endif	/* YIKES_H */

