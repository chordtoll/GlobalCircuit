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

    AD1CON2bits.VCFG = 3; //Vref+/Vref- reference

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

void SetExtADC(uint8_t channel)
{
    switch (channel)
	{
		case 0:
                        CS1 = 0;
			WriteSPI1(ADC1_V1_V);           //switch multiplexors to V1
			while(!SPI1STATbits.SPIRBF);    //wait for a result
			ReadSPI1();                     //read in the result
                        CS1 = 1;
			break;
		case 1:
                        CS1 = 0;
			WriteSPI1(ADC1_VD_V);        //switch multiplexors to VD
			while(!SPI1STATbits.SPIRBF);    //wait for a result
			ReadSPI1();                     //read in the result
                        CS1 = 1;
			break;
		case 2:
                        CS1 = 0;
			WriteSPI1(ADC1_H1_V);        //switch multiplexors to H1
			while(!SPI1STATbits.SPIRBF);    //wait for a result
			ReadSPI1();                     //read in the result
                        CS1 = 1;
			break;
		case 3:
                        CS2 = 0;
			WriteSPI1(ADC2_HD_V);            //switch multiplexors to HD
			while(!SPI1STATbits.SPIRBF);     //wait for a result
			ReadSPI1();                      //read in the result
                        CS2 = 1;
			break;
		case 4:
                        CS2 = 0;
			WriteSPI1(ADC2_V2_V);            //switch multiplexors to V2
			while(!SPI1STATbits.SPIRBF);     //wait for a result
			ReadSPI1();                      //read in the result
                        CS2 = 1;
			break;
		case 5:
                        CS2 = 0;
			WriteSPI1(ADC2_H2_V);            //switch multiplexors to H2
			while(!SPI1STATbits.SPIRBF);     //wait for a result
			ReadSPI1();                      //read in the result
                        CS2 = 1;
                        break;
                case 6:
                        CS1 = 0;
                        WriteSPI1(ADC1_T);
                        while(!SPI1STATbits.SPIRBF);
                        ReadSPI1();
                        CS1 = 1;
                        break;
                case 7:
                        CS2 = 0;
                        WriteSPI1(ADC2_T);
                        while(!SPI1STATbits.SPIRBF);
                        ReadSPI1();
                        CS2 = 1;
                        break;
	}
        CS1 = 1;
        CS2 = 1;
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
			WriteSPI1(ADC1_V1_V);            //start a conversion for V1
			while(!SPI1STATbits.SPIRBF);     //wait for a result
			bufferData = ReadSPI1();         //read in the result
                        CS1 = 1;
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 1:
                        CS1 = 0;
			WriteSPI1(ADC1_VD_V);            //start a conversion for VD
			while(!SPI1STATbits.SPIRBF);     //wait for a result
			bufferData = ReadSPI1();         //read in the result
                        CS1 = 1;
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 2:
                        CS1 = 0;
			WriteSPI1(ADC1_H1_V);            //start a conversion for H1
			while(!SPI1STATbits.SPIRBF);     //wait for a result
			bufferData = ReadSPI1();         //read in the result
                        CS1 = 1;
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 3:
                        CS2 = 0;
			WriteSPI1(ADC2_HD_V);            //start a conversion for HD
			while(!SPI1STATbits.SPIRBF);     //wait for a result
			bufferData = ReadSPI1();         //read in the result
                        CS2 = 1;
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 4:
                        CS2 = 0;
			WriteSPI1(ADC2_V2_V);            //start a conversion for V2
			while(!SPI1STATbits.SPIRBF);     //wait for a result
			bufferData = ReadSPI1();         //read in the result
                        CS2 = 1;
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 5:
                        CS2 = 0;
			WriteSPI1(ADC2_H2_V);            //start a conversion for H2
			while(!SPI1STATbits.SPIRBF);     //wait for a result
			bufferData = ReadSPI1();         //read in the result
                        CS2 = 1;
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 6:
                        CS1 = 0;
			WriteSPI1(ADC1_T);               //start a conversion for H2
			while(!SPI1STATbits.SPIRBF);     //wait for a result
			bufferData = ReadSPI1();         //read in the result
                        CS2 = 1;
			result = bufferData >> 18;
			configReadBack = bufferData;
			return result;
			break;
		case 7:
                        CS2 = 0;
			WriteSPI1(ADC2_T);               //start a conversion for H2
			while(!SPI1STATbits.SPIRBF);     //wait for a result
			bufferData = ReadSPI1();         //read in the result
                        CS2 = 1;
			result = bufferData >> 18;
			configReadBack = bufferData;
			return result;
			break;
	}

        CS1 = 1;
        CS2 = 1;

}

uint16_t ReadExtADC(uint8_t channel) {
    SetExtADC(channel);             //set multiplexors to passed channel
    WaitMS(20);                     //wait to allow change to take place
    return ReadExtADC_Aux(channel); //make conversion on passed channel
}

uint16_t ReadExtADCTemp(uint8_t chip)
{
    return ReadExtADC(6 + chip);
}