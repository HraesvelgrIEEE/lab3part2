/* 
 * File:   switch.h
 * Author: Garrett
 *
 * Created on September 19, 2015, 10:46 AM
 */

#ifndef SWITCH_H
#define	SWITCH_H

#define SW1 PORTAbits.RA7
#define SWITCH_PRESSED 0
#define SWITCH_NPRESSED 1

void initSwitches();
void initSW1();
void initSW2();

#endif	/* SWITCH_H */

