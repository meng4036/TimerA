/*
 * demo_overflow.c
 *
 *  Created on: 2018年1月3日
 *      Author: meng4
 */

#include <msp430.h>
#include "demo_overflow.h"

void demo_overflow() {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

	P1DIR |= 0x01;                            // P1.0 output
	TACTL = TASSEL_2 + MC_2 + TAIE;           // SMCLK, contmode, interrupt

	__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

/* 溢出中断处理函数 */
/*
#pragma vector=TIMER0_A1_VECTOR
__interrupt void timer_a(void) {
	switch(TA0IV) {
		case 2: break;      //CCR1 not used
		case 4: break;      // CCR2 not used
		case 10: P1OUT ^= 0x01;
			break;
	}
}
*/
