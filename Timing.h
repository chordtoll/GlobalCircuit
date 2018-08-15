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

uint64_t GetTimer();

void WaitTicks(uint64_t n);
void WaitUS(uint32_t n);
void WaitMS(uint32_t n);
void WaitS(uint32_t n);

void InitLoopDelay();
void DelayLoopMS(uint32_t n);

uint32_t __attribute__((nomips16)) ReadCoreTimer(void);
void __attribute__((nomips16)) WriteCoreTimer(uint32_t timer);
#endif	/* TIMING_H */

