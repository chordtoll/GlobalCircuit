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
#define ADC1_V1 0x92839283 //Correct readback: 128a or 128b

//Single Shot start, AIN1=p, AIN3=n, Gain = +-4.096, Continuous conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC1_VD 0xA283A283 //Correct readback: 228a or 228b

//Single Shot start, AIN2=p, AIN3=n, Gain = +-4.096, Continuous conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC1_H1 0xB283B283 //Correct readback: 328a or 328b

//Single Shot start, AIN0=p, AIN3=n, Gain = +-4.096, Continuous conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC2_HD 0x92839283 //Correct readback: 128a or 128b

//Single Shot start, AIN1=p, AIN3=n, Gain = +-4.096, Continuous conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC2_V2 0xA283A283 //Correct readback: 228a or 228b

//Single Shot start, AIN2=p, AIN3=n, Gain = +-4.096, Continuous conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC2_H2 0xB283B283 //Correct readback: 328a or 328b

//initialize PIC ADC
void InitPICADC();

//trigger a read of a specific ADC channel and return the result
uint16_t ReadPICADC(uint8_t channel);

//starts an ADC conversion on the passed in channel, returns conversion result
uint16_t ReadExtADC(uint8_t channel);

#endif	/* ADC_H */

