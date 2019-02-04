/*
 * File:   Timing.h
 * Author: asent
 *
 * Created on July 3, 2018, 3:07 PM
 */

#ifndef TIMING_H
#define	TIMING_H

#include <stdint.h>

#define TPS_DEFAULT 40000000
#define TPS_MAX     60000000
#define TPS_MIN     30000000

volatile uint64_t timer_accum;
volatile uint32_t tps=TPS_DEFAULT;
volatile uint8_t ctt_valid;

uint64_t GetCoreTimer();

//wait for a passed number of clock ticks
void WaitTicks(uint64_t n);

//wait for a passed number of microseconds
void WaitUS(uint32_t n);

//wait for a passed number of milliseconds
void WaitMS(uint32_t n);

//wait for a passed number of seconds
void WaitS(uint32_t n);

//sets the loop delay time to the current system time
void InitLoopDelay();

//delay for the difference of the time passed and the target time passed
void DelayLoopMS(uint32_t n);

uint32_t __attribute__((nomips16)) ReadCoreTimer(void);
void __attribute__((nomips16)) WriteCoreTimer(uint32_t timer);
#endif	/* TIMING_H */

