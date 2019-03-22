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

volatile char GPSready;
volatile char GPSnew;
volatile char GPSdata[84];

//sorts data stored in the *data pointer into various other data pointers relevant to what the data represents
void ParseNMEA(char *data, char* time, char *lati, char *latd, char *llon, char *lond, char *alti);

//start the GPS
void InitGPS(void);

void ReadGPS_S(uint32_t* time, uint32_t* lat, uint32_t* lon, uint32_t* alt);

#endif

