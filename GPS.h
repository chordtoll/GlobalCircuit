/* *****************************************************************************
 *
 * Author: Lauren Turnbow
 * Functions to read the altitude, time, longitude, and latitude of the GPS
 * NMEA string, $GPGGA
 *
***************************************************************************** */

#ifndef _GPS_H    /* Guard against multiple inclusion */
#define _GPS_H

// time (GMT), day (given by rockblock message), latitude, longitude, altitude
// To get the longitude out of the GPGGA string
char* longitude(char* NMEA, int length)
{
    char returnedArray[11] = {0};
    if (NMEA[0] == '$' && NMEA[1] == 'G' && NMEA[2] == 'P'
            && NMEA[3] == 'G' && NMEA[4] == 'G' && NMEA[5] == 'A')
    {
        returnedArray[0] = NMEA[30];
        returnedArray[1] = NMEA[31];
        returnedArray[2] = NMEA[32];
        returnedArray[3] = NMEA[33];
        returnedArray[4] = NMEA[34];
        returnedArray[5] = NMEA[36];
        returnedArray[6] = NMEA[37];
        returnedArray[7] = NMEA[38];
        returnedArray[8] = NMEA[39];
        returnedArray[9] = NMEA[41];
        returnedArray[10] = '\0';
        return returnedArray;
    }
    returnedArray[0] = 'A'; // Set the beginning character to 'A' if we are not looking
    // at the GPGGA string
    return returnedArray;
}

char* latitude(char* NMEA, int length)
{
    char returnedArray[10] = {0};
    // Check to see if we are looking at the GPGGA string
    if (NMEA[0] == '$' && NMEA[1] == 'G' && NMEA[2] == 'P'
            && NMEA[3] == 'G' && NMEA[4] == 'G' && NMEA[5] == 'A')
    {
        returnedArray[0] = NMEA[18];
        returnedArray[1] = NMEA[19];
        returnedArray[2] = NMEA[20];
        returnedArray[3] = NMEA[21];
        returnedArray[4] = NMEA[23];
        returnedArray[5] = NMEA[24];
        returnedArray[6] = NMEA[25];
        returnedArray[7] = NMEA[26];
        returnedArray[8] = NMEA[28];
        returnedArray[9] = '\0';
        return returnedArray;
    }
    returnedArray[0] = 'A';
    return returnedArray;
}

char* altitude(char* NMEA, int length)
{
    char returnedArray[6] = {0}; // meters above sea level
    // Check to see if we are looking at the GPGGA string
    if (NMEA[0] == '$' && NMEA[1] == 'G' && NMEA[2] == 'P'
            && NMEA[3] == 'G' && NMEA[4] == 'G' && NMEA[5] == 'A')
    {
        returnedArray[0] = NMEA[52];
        returnedArray[1] = NMEA[53];
        returnedArray[2] = NMEA[54];
        returnedArray[3] = NMEA[55];
        returnedArray[4] = NMEA[56];
        returnedArray[5] = '\0';
        return returnedArray;
    }
    returnedArray[0] = 'A';
    return returnedArray;
}

char* GMTTime(char* NMEA, int length)
{
    char returnedArray[10] = {0};
    if (NMEA[0] == '$' && NMEA[1] == 'G' && NMEA[2] == 'P'
            && NMEA[3] == 'G' && NMEA[4] == 'G' && NMEA[5] == 'A')
    {
        returnedArray[0] = NMEA[7];
        returnedArray[1] = NMEA[8];
        returnedArray[2] = NMEA[9];
        returnedArray[3] = NMEA[10];
        returnedArray[4] = NMEA[11];
        returnedArray[5] = NMEA[12];
        returnedArray[6] = NMEA[14];
        returnedArray[7] = NMEA[15];
        returnedArray[8] = NMEA[16];
        returnedArray[9] = '\0';
        return returnedArray;
    }
    returnedArray[0] = 'A';
    return returnedArray;

}

void ParseNMEA(char *data, char* time, char *lati, char *latd, char *llon, char *lond, char *alti) {
    int field=0;
    int fieldstart=0;
    int idx=0;
    while (data[idx]) {
        if (data[idx]==',') {
            switch(field) {
                case 1:
                    time[idx-fieldstart]=0;
                    break;
                case 2:
                    lati[idx-fieldstart]=0;
                    break;
                case 4:
                    llon[idx-fieldstart]=0;
                    break;
                case 9:
                    alti[idx-fieldstart]=0;
                    break;
            }
            field++;
            fieldstart=idx+1;
        } else {
            switch(field) {
                case 1:
                    time[idx-fieldstart]=data[idx];
                    break;
                case 2:
                    lati[idx-fieldstart]=data[idx];
                    break;
                case 3:
                    *latd=data[idx];
                    break;
                case 4:
                    llon[idx-fieldstart]=data[idx];
                    break;
                case 5:
                    *lond=data[idx];
                    break;
                case 9:
                    alti[idx-fieldstart]=data[idx];
                    break;
            }
        }
        idx++;
    }
}

#endif

