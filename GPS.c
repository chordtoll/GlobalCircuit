#include <proc/p32mx360f512l.h>

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

void GPS_init(void) {
    char InitString[16]={0xA0,0xA1,0x00,0x09,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x0D,0x0A};
    int i;
    for (i=0;i<16;i++) {
        U2TXREG=InitString[i];
        while (U2STAbits.TRMT == 0);
    }
}
