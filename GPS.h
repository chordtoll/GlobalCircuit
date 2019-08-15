/* *****************************************************************************
 *
 * Author: Lauren Turnbow
 * Functions to read the altitude, time, longitude, and latitude of the GPS
 * NMEA string, $GPGGA
 *
***************************************************************************** */

#ifndef _GPS_H    /* Guard against multiple inclusion */
#define _GPS_H

#include <stdint.h>

volatile char GPSnew;
volatile char GPSGGAdata[84];

//sorts data stored in the *data pointer into various other data pointers relevant to what the data represents
void ParseNMEA(char *data, char* time, char *lati, char *latd, char *llon, char *lond, char *alti, char* sats);

//start the GPS33
void InitGPS(void);

void ReadGPS(uint32_t* time, uint32_t* lat, uint32_t* lon, uint32_t* alt, uint8_t* sats);

//put the GPS to sleep
void SleepGPS();

//wake the GPS up (hot start time: 1 second)
void WakeGPS();

#endif

