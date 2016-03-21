/* 
 * File:   timer.h
 * Author: dhung
 *
 */

#ifndef INITTIMER_H
#define	INITTIMER_H

void initTimers();

void initTimer1();
void initTimer2();
void initTimer3(uint16_t prVal, uint16_t prescalar = 0);

void delayMs(unsigned int delay);
void delayUs(unsigned int delay);

#endif	/* INITTIMER_H */

