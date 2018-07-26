/* *****************************************************************************
 *
 * Author: Lauren Turnbow
 * Functions to read the altitude, time, longitude, and latitude of the GPS
 * NMEA string, $GPGGA
 *
***************************************************************************** */

#ifndef _GPS_H    /* Guard against multiple inclusion */
#define _GPS_H

void ParseNMEA(char *data, char* time, char *lati, char *latd, char *llon, char *lond, char *alti);

void InitGPS(void);

#endif

