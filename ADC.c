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
			WriteSPI1(ADC1_V1);
			while(!SPI1STATbits.SPIRBF);
			bufferData = ReadSPI1();
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 1:
			WriteSPI1(ADC1_VD);
			while(!SPI1STATbits.SPIRBF);
			bufferData = ReadSPI1();
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 2:
			WriteSPI1(ADC1_H1);
			while(!SPI1STATbits.SPIRBF);
			bufferData = ReadSPI1();
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 3:
			WriteSPI2(ADC2_HD);
			while(!SPI2STATbits.SPIRBF);
			bufferData = ReadSPI2();
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 4:
			WriteSPI2(ADC2_V2);
			while(!SPI2STATbits.SPIRBF);
			bufferData = ReadSPI2();
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
		case 5:
			WriteSPI2(ADC2_H2);
			while(!SPI2STATbits.SPIRBF);
			bufferData = ReadSPI2();
			result = bufferData >> 16;
			configReadBack = bufferData;
			return result;
			break;
	}

}
