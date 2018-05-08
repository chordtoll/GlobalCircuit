/*******************************************************************************
 *
 * Functions to initialize the on board ADC
 *
 ******************************************************************************/

#ifndef _ADCConfig_H_
#define	_ADCConfig_H_

void initADC(void)
{
    AD1PCFGbits.PCFG0 = 0;          // Analog input in Analog mode
    AD1PCFGbits.PCFG1 = 0;
    TRISBbits.TRISB0 = 1;           // Pin set as input
    TRISBbits.TRISB1 = 1;

    AD1CHSbits.CH0NA = 0;           // Channel 0 negative input is VR-
    AD1CHSbits.CH0SA = 0;           // Channel 0 positive input is AN0
    AD1CON1bits.FORM = 0;           // Integer 16-bit output
    AD1CON1bits.SSRC = 0b111;       // Internal counter ends sampling and starts conversion
    AD1CSSL = 0;                    // No scanning required
    AD1CON2bits.VCFG = 0;           // Internal voltage references
    AD1CON2bits.CSCNA = 0;          // Do not scan inputs
    AD1CON2bits.BUFM = 0;           // Buffer configured as one 16-word buffer
    AD1CON2bits.ALTS = 0;           // Always use MUX A input multiplexer settings
    AD1CON3bits.ADRC = 0;           // Clock derived from PBclock
    AD1CON3bits.ADCS = 0b00111111;  // TAD = 2*TPB
    AD1CON3bits.SAMC = 0b11111;     // 31 TAD auto-sample time
    AD1CON1bits.ON = 1;             // A/D converter module is operating
}

int ReadADC(int ch)
{
    AD1CHSbits.CH0SA = ch;          // Select input channel
    AD1CON1bits.SAMP = 1;           // Start sampling
    while(!AD1CON1bits.DONE);       // Wait for conversion to complete
    return ADC1BUF0;                // Read conversion result
}

#endif