/*
 * demo_upmode.c
 *
 *  Created on: 2018Äê1ÔÂ3ÈÕ
 *      Author: meng4
 */

#include <msp430.h>

void demo_upmode() {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

	P1DIR |= 0x01;                            // P1.0 output
	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 = 50000;                             // CCR0 range 0 ~ 65535(2^16-1)
	TACTL = TASSEL_2 + MC_1;                  // SMCLK, continue mode

	__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	P1OUT ^= 0x01;                            // Toggle P1.0
}


