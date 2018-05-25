/* 
 * File:   MS5607.h
 * Author: asent
 *
 * Created on May 25, 2018, 2:41 PM
 */

#ifndef MS5607_H
#define	MS5607_H

#define ALT_CMD_RESET 0x1E
#define ALT_CMD_D1    0x40
#define ALT_CMD_D2    0x50
#define ALT_CMD_ADC   0x00
#define ALT_CMD_PROM  0xA0

#define ALT_OSR_256   0x00
#define ALT_OSR_512   0x02
#define ALT_OSR_1024  0x04
#define ALT_OSR_2048  0x06
#define ALT_OSR_4096  0x08

char alt_start_pressure(char addr);
char alt_start_temperature(char addr);
int alt_read_adc(char addr);

#endif	/* MS5607_H */

