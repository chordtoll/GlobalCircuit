/*
 * File:   ADC.h
 * Author: Ryan Price
 *
 * Created on August 16, 2018, 7:30 PM
 */
#ifndef ADC_H
#define	ADC_H

//Chip select is the pins latch register bit
#define CS1 LATBbits.LATB2
#define CS2 LATGbits.LATG9

//ADC channel configs
//Single Shot start, AIN0=p, AIN3=n, Gain = +-4.096, single-shot conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC1_V1_V 0x93839383 //Correct readback: 128a or 128b

//Single Shot start, AIN1=p, AIN3=n, Gain = +-4.096, single-shot conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC1_VD_V 0xA383A383 //Correct readback: 228a or 228b

//Single Shot start, AIN2=p, AIN3=n, Gain = +-4.096, single-shot conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC1_H1_V 0xB383B383 //Correct readback: 328a or 328b

//Single Shot start, AIN0=p, AIN3=n, Gain = +-4.096, single-shot conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC2_HD_V 0x93839383 //Correct readback: 128a or 128b

//Single Shot start, AIN1=p, AIN3=n, Gain = +-4.096, single-shot conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC2_V2_V 0xA383A383 //Correct readback: 228a or 228b

//Single Shot start, AIN2=p, AIN3=n, Gain = +-4.096, single-shot conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC2_H2_V 0xB383B383 //Correct readback: 328a or 328b

#define ADC1_T 0x83938393

#define ADC2_T 0x83938393
//initialize PIC ADC
void InitPICADC();

//trigger a read of a specific ADC channel and return the result
uint16_t ReadPICADC(uint8_t channel);

//starts an ADC conversion on the passed in channel, returns conversion result
uint16_t ReadExtADC(uint8_t channel);

uint16_t ReadExtADCTemp(uint8_t chip);
#endif	/* ADC_H */

