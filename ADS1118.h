/*
 * File:   ADS1118.h
 * Author: ctheriau
 *
 * Created on March 29, 2018, 12:25 PM
 */

#ifndef ADS1118_H
#define	ADS1118_H

#ifdef	__cplusplus
extern "C" {
#endif

// Single-shot conversion start
#define     CONVERT         (0b001 << 0x0F)    // Bit 15

// Input mux config (POSref_NEGref)
#define     AIN0_AIN1       (0b000 << 0x0C)   // Bit 14:12 (default)
#define     AIN0_AIN3       (0b001 << 0x0C)   // Bit 14:12
#define     AIN1_AIN3       (0b010 << 0x0C)   // Bit 14:12
#define     AIN2_AIN3       (0b011 << 0x0C)   // Bit 14:12
#define     AIN0_GND        (0b100 << 0x0C)   // Bit 14:12
#define     AIN1_GND        (0b101 << 0x0C)   // Bit 14:12
#define     AIN2_GND        (0b110 << 0x0C)   // Bit 14:12
#define     AIN3_GND        (0b111 << 0x0C)   // Bit 14:12
//enum ads_mux_setting {  AN0_AN1,  AN0_AN3, AN1_AN3, AN2_AN3,
  //                              AN0_GND, AN1_GND, AN2_GND, AN3_GND} ads_mux_setting;

// Programmable gain amplifier config
#define     FSR_6p144       (0b000 << 0x09)   // Bit 11:09
#define     FSR_4p096       (0b001 << 0x09)   // Bit 11:09
#define     FSR_2p048       (0b010 << 0x09)   // Bit 11:09 (default)
#define     FSR_1p024       (0b011 << 0x09)   // Bit 11:09
#define     FSR_0p512       (0b100 << 0x09)   // Bit 11:09
#define     FSR_0p256a      (0b101 << 0x09)   // Bit 11:09
#define     FSR_0p256b      (0b110 << 0x09)   // Bit 11:09
#define     FSR_0p256c      (0b111 << 0x09)   // Bit 11:09
//enum  {  FSR_6_144,  FSR_4_096, FSR_2_048, FSR_1_024,
 //                               FSR_0_512, FSR_0_256a, FSR_0_256b, FSR_0_256c}ads_amp_setting;

// Operating mode
#define     CONTINUOUS      (0b000 << 0x08)   // Bit 8
#define     SINGLE_SHOT     (0b001 << 0x08)   // Bit 8 (default)

// Data rate
#define     SPS_8           (0b000 << 0x05)   // Bit 5
#define     SPS_16          (0b001 << 0x05)   // Bit 5
#define     SPS_32          (0b010 << 0x05)   // Bit 5
#define     SPS_64          (0b011 << 0x05)   // Bit 5
#define     SPS_128         (0b100 << 0x05)   // Bit 5 (default)
#define     SPS_250         (0b101 << 0x05)   // Bit 5
#define     SPS_475         (0b110 << 0x05)   // Bit 5
#define     SPS_860         (0b111 << 0x05)   // Bit 5
//enum  {    SP_8,  SP_16, SP_32, SP_64,
       //                         SP_128, SP_250, SP_475, SP_860}ads_data_rate;

// Temperature sensor mode
#define     TEMP_SENS       (0b001 << 0x04)   // Bit 4

// Pull-up enable
#define     PUP_EN          (0b001 << 0x03)   // Bit 3

// Write configuration settings
#define     WRITE_CONFIG    (0b001 << 0x01)   // Bit 2:1


typedef struct ads_config_data {
    unsigned int  result;
    unsigned char mux_config;
    unsigned char gain;
    unsigned char mode;
    unsigned char data_rate;
    unsigned char temp_mode;
    unsigned char pup_en;
} ads_config_data;

//ads_config_data ADSConfigToStruct(unsigned int configReg);


#ifdef	__cplusplus
}
#endif

#endif	/* ADS1118_H */

