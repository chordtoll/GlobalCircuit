/*
 * File:   MAG3310.h
 * Author: asent
 *
 * Created on May 27, 2018, 5:58 PM
 */

#include <stdint.h>

#ifndef MAG3310_H
#define	MAG3310_H

//ADDRESSES
#define MAG_ADDR     0x0E   //magnetometer
#define MAG_REG_DRS  0x00   //DR_STATUS register
#define MAG_REG_VXM  0x01   //X data MSB 
#define MAG_REG_VXL  0x02   //X data LSB
#define MAG_REG_VYM  0x03   //Y data MSB
#define MAG_REG_VYL  0x04   //Y data LSB
#define MAG_REG_VZM  0x05   //Z data MSB
#define MAG_REG_VZL  0x06   //Z data LSB
#define MAG_REG_WHO  0x07   //WHO_AM_I register
#define MAG_REG_SYM  0x08   //SYSMOD register
#define MAG_REG_OXM  0x09   //X offset correction MSB
#define MAG_REG_OXL  0x0A   //X offset correction LSB
#define MAG_REG_OYM  0x0B   //Y offset correction MSB
#define MAG_REG_OYL  0x0C   //Y offset correction LSB
#define MAG_REG_OZM  0x0D   //Z offset correction MSB
#define MAG_REG_OZL  0x0E   //Z offset correction LSB
#define MAG_REG_TMP  0x0F   //DIE_TEMP register
#define MAG_REG_CR1  0x10   //CTRL_REG1 register
#define MAG_REG_CR2  0x11   //CTRL_REG2 register

//CTRL_REG1 bits
#define MAG_CR1_ACT  0x01   //Operating mode selection (0: STANDBY | 1: ACTIVE)
#define MAG_CR1_TIM  0x02   //Trigger immediate mesurement (0: Normal operation | 1: Trigger)
#define MAG_CR1_FRD  0x04   //Fast read selection (0: full 16-bit value reads | 1: 8-bit value reads)
#define MAG_CR1_OS0  0x00   //Oversampling ratio value 0
#define MAG_CR1_OS1  0x08   //Oversampling ratio value 1
#define MAG_CR1_OS2  0x10   //Oversampling ratio value 2
#define MAG_CR1_OS3  0x18   //Oversampling ratio value 3
#define MAG_CR1_DR0  0x00   //Data rate value 0
#define MAG_CR1_DR1  0x20   //Data rate value 1
#define MAG_CR1_DR2  0x40   //Data rate value 2
#define MAG_CR1_DR3  0x60   //Data rate value 3
#define MAG_CR1_DR4  0x80   //Data rate value 4
#define MAG_CR1_DR5  0xA0   //Data rate value 5
#define MAG_CR1_DR6  0xC0   //Data rate value 6
#define MAG_CR1_DR7  0xE0   //Data rate value 7

//CTRL_REG2 bits
#define MAG_CR2_AMR  0x80   //Automatic magnetic sensor reset (0: DISABLED | 1: ENABLED)   
#define MAG_CR2_RAW  0x20   //Data output correction (0: values are corrected | 1: values are given raw)
#define MAG_CR2_MRS  0x10   //Magnetic sensor reset (0: reset cycle not active | 1: reset cycle busy/active)


//initializes magnetometer
uint8_t InitMagneto(uint8_t addr);

//triggers the magnetometer for a reading immediately
uint8_t TriggerMagneto(uint8_t addr);

//check the magnetometer to see if a conversion has been finished since it was last read
uint8_t CheckMagneto(uint8_t addr);

//read the data from the magnetometer
uint8_t ReadMagneto(uint8_t addr, uint16_t* x, uint16_t* y, uint16_t* z);

//read temperature from the magnetometer
uint8_t ReadMagTemp(uint8_t addr, uint8_t *temp);
#endif	/* MAG3310_H */

