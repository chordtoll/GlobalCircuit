/*
 * File:   ADC.c
 * Author: Ryan Price
 *
 * Created on August 16, 2018, 7:30 PM
 */
#include <proc/p32mx360f512l.h>
#include <stdint.h>
#include "ADC.h"
#include "SPI.h"

void InitPICADC() {
    AD1PCFGbits.PCFG0 = 0;//Enable analog on AN0
    AD1PCFGbits.PCFG12 = 0;
    AD1PCFGbits.PCFG13 = 0;

    TRISBbits.TRISB0 = 1; //AN0 is input


    AD1CHSbits.CH0SA = 0; //ADCA+=AN0
    AD1CHSbits.CH0NA = 0; //ADCA-=VR-

    AD1CON1bits.FORM = 0; //UINT32 output

    AD1CON1bits.SSRC = 7; //Clocked conversion
    AD1CON3bits.SAMC = 31;//31 clocks of sampling

    AD1CON2bits.VCFG = 0; //AVDD/AVSS reference

    AD1CON2bits.CSCNA = 0;//Auto scan disable

    AD1CON2bits.BUFM = 0; //16-bit buffer fill

    AD1CON2bits.ALTS = 0; //Do not alternate MUXES

    AD1CON3bits.ADRC = 0; //Use PBClk
    AD1CON3bits.ADCS = 0b00111111; //Set ADC clock divider

    AD1CON1bits.ADON=1;   //Turn on ADC
}

uint16_t ReadPICADC(uint8_t channel) {
    AD1CHSbits.CH0SA=channel; //select ADC channel to read
    AD1CON1bits.SAMP=1;       //start sampling
    while (!AD1CON1bits.DONE);//wait for sampling to finish
    return ADC1BUF0&0x3FF;    //return conversion result
}

uint16_t ReadExtADC_Aux(uint8_t channel)
{
	uint32_t bufferData = 0;
	uint16_t result = 0;
	uint16_t configReadBack = 0;

        CS1 = 1;
        CS2 = 1;

	switch (channel)
	{
		case 0:
                        CS1 = 0;
			WriteSPI1(ADC1_V1);         //start a conversion on ADC channel 0
			while(!SPI1STATbits.SPIRBF);//wait for a result
			bufferData = ReadSPI1();    //read in the result
                        CS1 = 1;
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 1:
                        CS1 = 0;
			WriteSPI1(ADC1_VD);         //start a conversion on ADC channel 1
			while(!SPI1STATbits.SPIRBF);//wait for a result
			bufferData = ReadSPI1();    //read in the result
                        CS1 = 1;
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 2:
                        CS1 = 0;
			WriteSPI1(ADC1_H1);         //start a conversion on ADC channel 2
			while(!SPI1STATbits.SPIRBF);//wait for a result
			bufferData = ReadSPI1();    //read in the result
                        CS1 = 1;
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 3:
                        CS2 = 0;
			WriteSPI1(ADC2_HD);         //start a conversion on ADC channel 3
			while(!SPI1STATbits.SPIRBF);//wait for a result
			bufferData = ReadSPI1();    //read in the result
                        CS2 = 1;
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 4:
                        CS2 = 0;
			WriteSPI1(ADC2_V2);         //start a conversion on ADC channel 4
			while(!SPI1STATbits.SPIRBF);//wait for a result
			bufferData = ReadSPI1();    //read in the result
                        CS2 = 1;
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 5:
                        CS2 = 0;
			WriteSPI1(ADC2_H2);         //start a conversion on ADC channel 5
			while(!SPI1STATbits.SPIRBF);//wait for a result
			bufferData = ReadSPI1();    //read in the result
                        CS2 = 1;
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
	}

        CS1 = 1;
        CS2 = 1;

}

uint16_t ReadExtADC(uint8_t channel) {
    WaitMS(1);
    ReadExtADC_Aux(channel);
    WaitMS(1);
    return ReadExtADC_Aux(channel);
}
