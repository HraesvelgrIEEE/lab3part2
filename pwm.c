/* File:   pwm.h
 * Author: dhung
 */

#include <xc.h>
#include "pwm.h"
#include "timer.h"

#define PWM_A_REG RPG1Rbits.RPG1R
#define PWM_A_CON OC3CONbits
#define PWM_A_R OC3R
#define PWM_A_RS OC3RS
#define PWM_A_DIR_TRIS TRISFbits.TRISF0
#define PWM_A_DIR_LAT LATFbits.LATF0

#define PWM_B_REG RPG0Rbits.RPG0R
#define PWM_B_CON OC4CONbits
#define PWM_B_R OC4R
#define PWM_B_RS OC4RS
#define PWM_B_DIR_TRIS TRISFbits.TRISF1
#define PWM_B_DIR_LAT LATFbits.LATF1

#define INPUT 1
#define OUTPUT 0

#define PWM_TIMER3_PRVAL 512 //FIXME

void initPWM() {
    initTimer3(PWM_TIMER3_PRVAL);
    
    //TODO: CONTROL THE ENABLE PINS
    
    //Direction switches
    PWM_A_DIR_TRIS = OUTPUT;
    PWM_B_DIR_TRIS = OUTPUT;
    
    //OC3:
    PWM_A_REG = 0b1011; //Map OC3 to RG1
    PWM_A_CON.ON = 0; //Disable OC3 during initialization
    PWM_A_R = 0x0000; //Init OCxR
    PWM_A_RS = 0x0000; //Init OCxRS
    PWM_A_CON.OC32 = 0; //Use 16 bit compare mode
    PWM_A_CON.OCTSEL = 1; //Use Timer3 as clock source
    PWM_A_CON.OCM = 6; //Set Output Compare Mode to PWM, fault pin disabled
    PWM_A_CON.ON = 1; //Enable OC3
    
    //OC4:
    PWM_B_REG = 0b1011;
    PWM_B_CON.ON = 0; //Disable OC4 during initialization
    PWM_B_R = 0x0000; //Init OCxR
    PWM_B_RS = 0x0000; //Init OCxRS
    PWM_B_CON.OC32 = 0; //Use 16 bit compare mode
    PWM_B_CON.OCTSEL = 1; //Use Timer3 as clock source
    PWM_B_CON.OCM = 6; //Set Output Compare Mode to PWM, fault pin disabled
    PWM_B_CON.ON = 1; //Enable OC4
}

void setPWM(int motorSelect, float dutyCycle, int dir, int idleFlag) {
    //idle is an override flag, disables OC because Garrett is finicky
    
    if (dutyCycle < 0) dutyCycle = 0;
    if (dutyCycle > 1) dutyCycle = 1;
//    
//    if (idleFlag == 1) {
//        PWM_A_REG = 0;
//        PWM_B_REG = 0;
//    }
//    
//    else {
       
    switch (motorSelect) {
        case PWM_MOTOR_A:
            if (idleFlag == 1) {
                //PWM_A_CON.ON = 0;
                PWM_A_REG = 0;
                PWM_A_DIR_LAT = 0;
            }
            else {
                PWM_A_REG = 0b1011;
                //PWM_A_CON.ON = 1;
                PWM_A_DIR_LAT = dir;
                if (dir == PWM_MOTOR_FORWARD)
                    PWM_A_RS = dutyCycle * PWM_TIMER3_PRVAL;
                else
                    PWM_A_RS = (1 - dutyCycle) * PWM_TIMER3_PRVAL;
            }
            break;
        case PWM_MOTOR_B:
            if (idleFlag == 1) {
                //PWM_B_CON.ON = 0;
                PWM_B_REG = 0;
                PWM_B_DIR_LAT = 0;
            }
            else {
                //PWM_B_CON.ON = 1;
                PWM_B_REG = 0b1011;
                PWM_B_DIR_LAT = dir;
                if (dir == PWM_MOTOR_FORWARD)
                    PWM_B_RS = dutyCycle * PWM_TIMER3_PRVAL;
                else
                    PWM_B_RS = (1 - dutyCycle) * PWM_TIMER3_PRVAL;
            }
            break;
        default:
            //Unrecognized motor selected, abort
            return;
    }
//    
//    }
}
