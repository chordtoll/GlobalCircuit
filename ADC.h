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
#define ADC1_V1_READ 0x93839383 //Correct readback: 128a or 128b
#define ADC1_V1_SET 0x13831383  //set MUXs for V1
//Single Shot start, AIN1=p, AIN3=n, Gain = +-4.096, single-shot conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC1_VD_READ 0xA383A383 //Correct readback: 228a or 228b
#define ADC1_VD_SET 0x23832383  //set MUXs for VD
//Single Shot start, AIN2=p, AIN3=n, Gain = +-4.096, single-shot conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC1_H1_READ 0xB383B383 //Correct readback: 328a or 328b
#define ADC1_H1_SET 0x33833383  //set MUXs for H1
//Single Shot start, AIN0=p, AIN3=n, Gain = +-4.096, single-shot conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC2_HD_READ 0x93839383 //Correct readback: 128a or 128b
#define ADC2_HD_SET 0x13831383  //set MUXs for HD
//Single Shot start, AIN1=p, AIN3=n, Gain = +-4.096, single-shot conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC2_V2_READ 0xA383A383 //Correct readback: 228a or 228b
#define ADC2_V2_SET 0x23832383  //set MUXs for V2
//Single Shot start, AIN2=p, AIN3=n, Gain = +-4.096, single-shot conversion, 128SPS, ADC Mode, Pullup, Write to config register
#define ADC2_H2_READ 0xB383B383 //Correct readback: 328a or 328b
#define ADC2_H2_SET 0x33833383  //set MUXs for H2
//initialize PIC ADC
void InitPICADC();

//trigger a read of a specific ADC channel and return the result
uint16_t ReadPICADC(uint8_t channel);

//starts an ADC conversion on the passed in channel, returns conversion result
uint16_t ReadExtADC(uint8_t channel);

#endif	/* ADC_H */

