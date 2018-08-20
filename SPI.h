/*
 * File:   SPI.h
 * Author: Ryan Price
 *
 * Created on August 16, 2018, 7:30 PM
 */

//Defines

//ADC channel configs
//Single Shot start, AIN0=p, AIN3=n, Gain = +-4.096, Continuous conversion, 8SPS, ADC Mode, Pullup, Write to config register
#define ADC0_VP 0x920A //Correct readback: 120a or 120b

//Single Shot start, AIN2=p, AIN3=n, Gain = +-4.096, Continuous conversion, 8SPS, ADC Mode, Pullup, Write to config register
#define ADC0_HP 0xB20A //Correct readback: 0320a or 032b

//Single Shot start, AIN1=p, AIN3=n, Gain = +-4.096, Continuous conversion, 8SPS, ADC Mode, Pullup, Write to config register
#define ADC0_VD 0xA20A //Correct readback: 220a or 220b

//Single Shot start, AIN1=p, AIN3=n, Gain = +-4.096, Continuous conversion, 8SPS, ADC Mode, Pullup, Write to config register
#define ADC1_VN 0xA20A //Correct readback: 220a or 220b

//Single Shot start, AIN2=p, AIN3=n, Gain = +-4.096, Continuous conversion, 8SPS, ADC Mode, Pullup, Write to config register
#define ADC1_HN 0xB20A //Correct readback: 0320a or 032b

//Single Shot start, AIN0=p, AIN3=n, Gain = +-4.096, Continuous conversion, 8SPS, ADC Mode, Pullup, Write to config register
#define ADC1_HD 0x920A //Correct readback: 020a or 020b

//Chip select is the pins latch register bit
#define CS1 LATBbits.LATB2
#define CS2 LATGbits.LATG9

//Function declarations
void Delay ( void );
//SPI1
void InitSPI1(void);
double WriteReadSPI1(unsigned short i);
unsigned int ReadSPI1();
void WriteSPI1(unsigned int data_out);

//SPI2
void InitSPI2(void);
double WriteReadSPI2(unsigned short i);
unsigned int ReadSPI2();
void WriteSPI2(unsigned int data_out);