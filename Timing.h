/*
 * File:   Timing.h
 * Author: asent
 *
 * Created on July 3, 2018, 3:07 PM
 */

#ifndef TIMING_H
#define	TIMING_H

volatile unsigned long long timer_accum;
volatile unsigned int tps=40000000;

unsigned long long GetTimer();

void WaitTicks(long n);
void WaitUS(int n);
void WaitMS(int n);
void WaitS(int n);

void InitLoopDelay();
void DelayLoopMS(int n);

unsigned int __attribute__((nomips16)) ReadCoreTimer(void);
void __attribute__((nomips16)) WriteCoreTimer(unsigned int timer);
#endif	/* TIMING_H */

