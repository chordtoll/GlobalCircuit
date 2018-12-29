/* 
 * File:   PICADC.h
 * Author: asent
 *
 * Created on July 25, 2018, 10:01 PM
 */

#ifndef PICADC_H
#define	PICADC_H


//initialize PIC ADC
void InitPICADC();

//trigger a read of a specific ADC channel and return the result
uint16_t ReadPICADC(uint8_t channel);

#endif	/* PICADC_H */

