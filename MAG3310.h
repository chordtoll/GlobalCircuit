/* 
 * File:   MAG3310.h
 * Author: asent
 *
 * Created on May 27, 2018, 5:58 PM
 */

#ifndef MAG3310_H
#define	MAG3310_H

#define MAG_ADDR     0x0E
#define MAG_REG_DRS  0x00
#define MAG_REG_VXM  0x01
#define MAG_REG_VXL  0x02
#define MAG_REG_VYM  0x03
#define MAG_REG_VYL  0x04
#define MAG_REG_VZM  0x05
#define MAG_REG_VZL  0x06
#define MAG_REG_WHO  0x07
#define MAG_REG_SYM  0x08
#define MAG_REG_OXM  0x09
#define MAG_REG_OXL  0x0A
#define MAG_REG_OYM  0x0B
#define MAG_REG_OYL  0x0C
#define MAG_REG_OZM  0x0D
#define MAG_REG_OZL  0x0E
#define MAG_REG_TMP  0x0F
#define MAG_REG_CR1  0x10
#define MAG_REG_CR2  0x11

#define MAG_CR1_ACT  0x01
#define MAG_CR1_TIM  0x02
#define MAG_CR1_FRD  0x04
#define MAG_CR1_OS0  0x00
#define MAG_CR1_OS1  0x08
#define MAG_CR1_OS2  0x10
#define MAG_CR1_OS3  0x18
#define MAG_CR1_DR0  0x00
#define MAG_CR1_DR1  0x20
#define MAG_CR1_DR2  0x40
#define MAG_CR1_DR3  0x60
#define MAG_CR1_DR4  0x80
#define MAG_CR1_DR5  0xA0
#define MAG_CR1_DR6  0xC0
#define MAG_CR1_DR7  0xE0

#define MAG_CR2_AMR  0x80
#define MAG_CR2_RAW  0x20
#define MAG_CR2_MRS  0x10


char mag_reset(char addr);
char mag_start(char addr);
char mag_check(char addr);
char mag_read(char addr, short* x, short* y, short* z);

#endif	/* MAG3310_H */

