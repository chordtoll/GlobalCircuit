/*
 * File:   SPI.h
 * Author: Ryan Price
 *
 * Created on August 16, 2018, 7:30 PM
 */

//Defines

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