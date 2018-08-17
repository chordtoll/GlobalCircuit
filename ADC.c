/*
 * File:   ADC.c
 * Author: Ryan Price
 *
 * Created on August 16, 2018, 7:30 PM
 */
#include <proc/p32mx360f512l.h>
#include "ADC.h"
#include "SPI.h"

uint16_t ReadADC(uint8_t channel)
{
	unsigned short bufferData = 0;
	unsigned short result = 0;
	unsigned short configReadBack = 0;
	switch (channel)
	{
		case 0:
			WriteSPI1(ADC0_VP);
			while(!SPI1STATbits.SPIRBF);
			bufferData = ReadSPI1();
			result = bufferData >> 16;
			configReadBack = result;
			return result;
			break;
		case 1:
			WriteSPI1(ADC0_VD);
			while(!SPI1STATbits.SPIRBF);
			bufferData = ReadSPI1();
			result = bufferData >> 16;
			configReadBack = result;
			return result;
			break;
		case 2:
			WriteSPI1(ADC0_HP);
			while(!SPI1STATbits.SPIRBF);
			bufferData = ReadSPI1();
			result = bufferData >> 16;
			configReadBack = result;
			return result;
			break;
		case 3:
			WriteSPI2(ADC1_HD);
			while(!SPI2STATbits.SPIRBF);
			bufferData = ReadSPI2();
			result = bufferData >> 16;
			configReadBack = result;
			return result;
			break;
		case 4:
			WriteSPI2(ADC1_VN);
			while(!SPI2STATbits.SPIRBF);
			bufferData = ReadSPI2();
			result = bufferData >> 16;
			configReadBack = result;
			return result;
			break;
		case 5:
			WriteSPI2(ADC1_HN);
			while(!SPI2STATbits.SPIRBF);
			bufferData = ReadSPI2();
			result = bufferData >> 16;
			configReadBack = result;
			return result;
			break;
	}

}