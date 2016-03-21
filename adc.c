/* File:   adc.c
 * Author: dhung
 */

#include <xc.h>
#include "adc.h"

void initADC() {
    ANSELDbits.ANSD1 = 0; //Use RD1 as analog input
    AD1CON1bits.FORM = 0; // 16 unsigned integer
    AD1CON1bits.SSRC = 7; // Auto-convert mode
    AD1CON1bits.ASAM = 1; // Auto-sampling
    AD1CON2bits.VCFG = 0; // Use board reference voltages
    AD1CON2bits.CSCNA = 0; // Disable scanning
    AD1CON2bits.SMPI = 0; // Interrupt for each sample/convert sequence
    AD1CON2bits.ALTS = 0; // Only Mux A
    AD1CON3bits.ADRC = 0; // Use PBCLK
    AD1CON3bits.SAMC = 2; // 2 T_AD per sample
    AD1CON3bits.ADCS = 1; // T_AD = 4 * T_PBCLK
    AD1CHSbits.CH0NA = 0; // Use Vref- as negative reference
    AD1CHSbits.CH0SA = 24; // Scan AN24
    IFS0bits.AD1IF = 0; // Put down ADC flag
    IPC5bits.AD1IP = 7;
    IEC0bits.AD1IE = 1;
    AD1CON1bits.ADON = 1; // turn on the ADC
}