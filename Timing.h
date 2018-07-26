/*
 * File:   Timing.h
 * Author: asent
 *
 * Created on July 3, 2018, 3:07 PM
 */

#ifndef TIMING_H
#define	TIMING_H

volatile unsigned long long timer_accum;
volatile unsigned int tps=1;

unsigned long long get_timer();

void wait_ticks(long n);
void wait_us(int n);
void wait_ms(int n);
void wait_s(int n);

void loop_delay_init();
void loop_delay_ms(int n);
#endif	/* TIMING_H */

