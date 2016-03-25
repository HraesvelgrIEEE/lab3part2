/*
 * File:   timer.c
 * Author: dhung
 *
 */

#include <xc.h>
#include "timer.h"

typedef unsigned short uint16_t;

void initTimers() {
    //initTimer1(); //Uneeded atm
    initTimer2();
}

void initTimer1() {
    TMR1 = 0; //Clear timer
    T1CONbits.TCKPS = 3; //Prescalar 256
    T1CONbits.TCS = 0; //Set oscillator
    PR1 = 390; //PR for ~10 ms, rounded from 389.625
    IEC0bits.T1IE = 1; //Enable
    IFS0bits.T1IF = 0; //Flag down
    IPC1bits.T1IP = 7; //Default priority
    T1CONbits.TON = 1; //Timer on
}

void initTimer2() {
    TMR2 = 0;
    T2CONbits.TCKPS = 0; //Prescalar 1
    T2CONbits.TCS = 0; 
    IFS0bits.T2IF = 0; //Flag down
}

//This timer will be used exclusively for PWM
void initTimer3(uint16_t prVal) {
    TMR3 = 0;
    T3CONbits.TCKPS = 0;
    T3CONbits.TCS = 0; //Use main (10 MHz) oscillator
    PR3 = prVal;
    IEC0bits.T3IE = 0; //We don't want the interrupt, just the actual timer
    IFS0bits.T3IF = 0; //Flag down
    //IPC3bits.T3IP = 7; //Priority, I don't think we need this
    T3CONbits.TON = 1; //Timer on
}

void delayMs(unsigned int delay) {
    int i = 0;
    for (i = 0; i != delay; ++i) {
        delayUs(1000); //delay 1 ms
    }
}

void delayUs(unsigned int delay){
    TMR2 = 0;
    PR2 = 14*delay;
    IFS0bits.T2IF = 0;
    T2CONbits.ON = 1;
    while (IFS0bits.T2IF == 0);
    T2CONbits.ON = 0;
}