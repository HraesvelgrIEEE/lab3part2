/* File: main.c
 * Author: dhung 
 */

#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "interrupt.h"
#include "lcd.h"
#include "timer.h"
#include "pwm.h"
#include "adc.h"
#include "switch.h"

typedef enum stateTypeEnum {wait, debounce, waitRelease, colorado, setPWMs} stateType; 
typedef enum motorStateTypeEnum {forward, idle1, reverse, idle2} motorStateType;

volatile stateType state, nextState;
volatile motorStateType motorState;
volatile short direction, idle;
volatile int pot;

int main(void) {
    //Initialize
    enableInterrupts(); 
    SYSTEMConfigPerformance(10000000);
    //Initialize stuff that I actually understand
    initLCD();
    initTimers();
    initADC();
    initPWM();
    initSwitches();
    
    direction = PWM_MOTOR_FORWARD;
    idle = 0;
    motorState = forward;
   
    int leftMotor, rightMotor;
    char buffer[5];
    
    while (1) {
        switch (state) {
            case wait:
                
                break;
            case debounce:
                delayMs(5);
                state = waitRelease;
                break;
            case waitRelease:
                break;
            case colorado:
                itoa(pot, buffer, 10);
                printStringLCD(buffer);
                state = setPWMs;
                break;
            case setPWMs:
                leftMotor = 1 - float(511 - pot) / 511.0;
                rightMotor = 1 - float(pot - 511) / 511.0;
                setPWM(PWM_MOTOR_A, leftMotor, PWM_MOTOR_FORWARD);
                setPWM(PWM_MOTOR_B, rightMotor, PWM_MOTOR_FORWARD);
                state = wait;
                break;
        }
    }
}

__ISR(_ADC_VECTOR, IPL7SRS) _ADCInterrupt() {
    IFS0bits.AD1IF = 0;
    
    pot = ADC1BUF0;
    
    state = colorado;
}

__ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt() {
    IFS1bits.CNAIF = 0;
    
    if (SW1 == SWITCH_PRESSED) {
        state = debounce;
    }
    
    if (SW1 == SWITCH_NPRESSED && state == waitRelease) {
        switch (motorState) {
            case forward:
                motorState = idle1;
                idle = 1;
                break;
            case idle1:
                motorState = reverse;
                direction = PWM_MOTOR_REVERSE;
                idle = 0;
                break;
            case reverse:
                motorState = idle2;
                idle = 1;
                break;
            case idle2:
                motorState = forward;
                direction = PWM_MOTOR_FORWARD;
                idle = 0;
                break;
        }
        state = setPWMs;
    }
}