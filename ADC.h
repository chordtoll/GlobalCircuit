/*
 * File:   ADC.h
 * Author: Ryan Price
 *
 * Created on August 16, 2018, 7:30 PM
 */
#ifndef ADC_H
#define	ADC_H

//ADC channel configs
//Single Shot start, AIN0=p, AIN3=n, Gain = +-4.096, Continuous conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC1_V1 0xA28A //Correct readback: 128a or 128b

//Single Shot start, AIN1=p, AIN3=n, Gain = +-4.096, Continuous conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC1_VD 0x928A //Correct readback: 228a or 228b

//Single Shot start, AIN2=p, AIN3=n, Gain = +-4.096, Continuous conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC1_H1 0xB28A //Correct readback: 328a or 328b

//Single Shot start, AIN0=p, AIN3=n, Gain = +-4.096, Continuous conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC2_HD 0xB28A //Correct readback: 128a or 128b

//Single Shot start, AIN1=p, AIN3=n, Gain = +-4.096, Continuous conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC2_V2 0xA28A //Correct readback: 228a or 228b

//Single Shot start, AIN2=p, AIN3=n, Gain = +-4.096, Continuous conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC2_H2 0x928A //Correct readback: 328a or 328b

uint16_t ReadADC(uint8_t channel);

#endif	/* PICADC_H */

