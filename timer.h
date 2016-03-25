/* 
 * File:   timer.h
 * Author: dhung
 *
 */

#ifndef INITTIMER_H
#define	INITTIMER_H

typedef unsigned short uint16_t;

void initTimers();

void initTimer1();
void initTimer2();
void initTimer3(uint16_t prVal);

void delayMs(unsigned int delay);
void delayUs(unsigned int delay);

#endif	/* INITTIMER_H */

