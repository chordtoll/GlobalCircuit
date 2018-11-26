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

uint16_t ReadADC(uint8_t channel)
{
	uint32_t bufferData = 0;
	uint16_t result = 0;
	uint16_t configReadBack = 0;
	switch (channel)
	{
		case 0:
			WriteSPI1(ADC1_V1);         //start a conversion on ADC channel 0
			while(!SPI1STATbits.SPIRBF);//wait for a result
			bufferData = ReadSPI1();    //read in the result
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 1:
			WriteSPI1(ADC1_VD);         //start a conversion on ADC channel 1
			while(!SPI1STATbits.SPIRBF);//wait for a result
			bufferData = ReadSPI1();    //read in the result
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 2:
			WriteSPI1(ADC1_H1);         //start a conversion on ADC channel 2
			while(!SPI1STATbits.SPIRBF);//wait for a result
			bufferData = ReadSPI1();    //read in the result
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 3:
			WriteSPI2(ADC2_HD);         //start a conversion on ADC channel 3
			while(!SPI2STATbits.SPIRBF);//wait for a result
			bufferData = ReadSPI2();    //read in the result
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 4:
			WriteSPI2(ADC2_V2);         //start a conversion on ADC channel 4
			while(!SPI2STATbits.SPIRBF);//wait for a result
			bufferData = ReadSPI2();    //read in the result
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 5:
			WriteSPI2(ADC2_H2);         //start a conversion on ADC channel 5
			while(!SPI2STATbits.SPIRBF);//wait for a result
			bufferData = ReadSPI2();    //read in the result
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
	}

}
