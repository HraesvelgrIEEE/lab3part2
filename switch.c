/*
 * File:   switch.c
 * Author: Garrett
 *
 * Created on September 19, 2015, 10:46 AM
 */

#include <xc.h>
#include "switch.h"

#define INPUT 1
#define ENABLED 1

void initSwitches(){   
    initSW1();
    //initSW2();
}
void initSW1(){ // to be used as the start/stop switch
    TRISAbits.TRISA7 = INPUT;           // Configure switch as input
    CNCONAbits.ON = 1;                  // Enable overall interrupt
    CNENAbits.CNIEA7 = ENABLED;         // Enable pin CN
    CNPUAbits.CNPUA7 = ENABLED;         // Enable pull-up resistor
    IFS1bits.CNAIF = 0;                 // Put down the flag
    IPC8bits.CNIP = 7;                  // Configure interrupt priority
    IEC1bits.CNAIE = ENABLED;           // Enable interrupt for A pins
}

void initSW2(){ // to be used as the start/stop switch
    TRISGbits.TRISG13 = INPUT;           // Configure switch as input    
    CNCONGbits.ON = 1;                  // Enable overall interrupt    
    CNENGbits.CNIEG13 = ENABLED;         // Enable pin CN
    CNPUGbits.CNPUG13 = ENABLED;         // Enable pull-up resistor    
    IFS1bits.CNGIF = 0;                 // Put down the flag    
    IPC8bits.CNIP = 7;                  // Configure interrupt priority
    IEC1bits.CNGIE = ENABLED;           // Enable interrupt for D pins? G pins?
}
