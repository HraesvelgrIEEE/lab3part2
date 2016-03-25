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
volatile int direction, idle;
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
    
    //Calibrate pot
    printStringLCD("Calibrate Pot");
    
    int min = 500, max = 500;
    int i = 0;
    
    for (i = 0; i != 20; ++i) {
        if (pot < min) min = pot;
        if (pot > max) max = pot;
        delayMs(100);
    }
    
    float average = (float) (max - min) / 2.0 + min;
    //End calibrate
    
    direction = PWM_MOTOR_FORWARD;
    idle = 0;
    motorState = forward;
   
    float leftMotor, rightMotor;
    char buffer[16];
    
    while (1) {
        switch (state) {
            case wait:
                delayMs(1);
                state = colorado;
                break;
            case debounce:
                delayMs(5);
                state = waitRelease;
                break;
            case waitRelease:
                break;
            case colorado:
                nextState = setPWMs;
                sprintf(buffer, "Voltage: %.3fV", pot / 1023.0 * 3.3);
                clearLCD();
                moveCursorLCD(0, 1);
                printStringLCD(buffer);
                moveCursorLCD(0, 2);
                char motorStateStr[8];
                switch (motorState) {
                    case idle1:
                    case idle2:
                        sprintf(motorStateStr, "%s", "Idle");
                        break;
                    case forward:
                        sprintf(motorStateStr, "%s", "Forward");
                        break;
                    case reverse:
                        sprintf(motorStateStr, "%s", "Reverse");
                        break;
                }
                printStringLCD("State: "); printStringLCD(motorStateStr);
                state = nextState;
                delayMs(10);
                break;
            case setPWMs:
                nextState = colorado;
                leftMotor = 1 - 2 * (float) (average - pot) / average;
                rightMotor = 1 - 2 * (float) (pot - average) / average;
                setPWM(PWM_MOTOR_A, leftMotor, direction, idle);
                setPWM(PWM_MOTOR_B, rightMotor, direction, idle);
                state = nextState;
                delayMs(1);
                break;
        }
    }
}

__ISR(_ADC_VECTOR, IPL7SRS) _ADCInterrupt() {
    IFS0bits.AD1IF = 0;
    
    pot = ADC1BUF0;
    
    //if (state == wait) state = colorado;
}

__ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt() {
    IFS1bits.CNAIF = 0;
    
    if (SW1 == SWITCH_PRESSED) {
        nextState = debounce;
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