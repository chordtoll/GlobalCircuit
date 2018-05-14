/* 
 * File:   conversion.h
 * Author: asent
 *
 * Created on May 14, 2018, 3:56 PM
 */

#ifndef CONVERSION_H
#define	CONVERSION_H

void itob64(long val, char* b64) {
    int i=0;
    char cval;
    while (val) {
        cval=val%64;
        if (cval<26) {
            b64[i]=cval+'A'-0;
        } else if (cval<52) {
            b64[i]=cval+'a'-26;
        } else if (cval<62) {
            b64[i]=cval+'0'-52;
        } else if (cval==62) {
            b64[i]='+';
        } else if (cval==63) {
            b64[i]='/';
        }
        val/=64;
        i++;
    }
    b64[i]=0;
}

#endif	/* CONVERSION_H */

