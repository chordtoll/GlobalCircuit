#include <proc/p32mx360f512l.h>


void InitPICADC() {
    AD1PCFGbits.PCFG0 = 0;//Enable analog on AN0

    TRISBbits.TRISB0 = 1; //AN0 is input

    AD1CHSbits.CH0SA = 0; //ADCA+=AN0
    AD1CHSbits.CH0NA = 0; //ADCA-=VR-

    AD1CON1bits.FORM = 0; //UINT32 output

    AD1CON1bits.SSRC = 7; //Clocked conversion
    AD1CON3bits.SAMC = 31;//31 clocks of sampling

    AD1CON2bits.VCFG = 0; //AVDD/AVSS reference

    AD1CON2bits.CSCNA = 0;//Auto scan disable

    AD1CON2bits.BUFM = 0; //16-bit buffer fill

    AD1CON2bits.ALTS = 0; //Do not alternate MUXES

    AD1CON3bits.ADRC = 0; //Use PBClk
    AD1CON3bits.ADCS = 0b00111111; //Set ADC clock divider

    AD1CON1bits.ADON=1;   //Turn on ADC
}

uint16_t ReadPICADC(uint8_t channel) {
    AD1CHSbits.CH0SA=channel; //select ADC channel to read
    AD1CON1bits.SAMP=1;       //start sampling
    while (!AD1CON1bits.DONE);//wait for sampling to finish
    return ADC1BUF0&0x3FF;    //return conversion result
}
