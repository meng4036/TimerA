/*
 * demo_interrupt.c
 *
 *  Created on: 2018年1月6日
 *      Author: meng4
 */
#include <msp430.h>
#include "demo_interrupt.h"

/**
 * 演示比较模式下，触发比较-捕获模块产生的中断
 */

/* CCR0的中断处理函数 */
#ifdef _DEMO_INTERRUPT
#pragma vector=TIMER0_A0_VECTOR
#endif
__interrupt void CCR0_handler(void)
{
	P1OUT ^= BIT0;
}

/* 比较模式CCR0中断，TAR计数到CCR0的值时，触发中断 */
void demo_CCR0_interrupt(void) {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P1DIR |= BIT0;							  // 显示结果

	CCTL0 = CCIE;
	CCR0 = 0xFFFF >> 1;
	TACTL = TASSEL_2 + MC_1 + TAIE;

	__bis_SR_register(LPM0_bits + GIE);
}


/* CCR1和CCR2的中断处理函数 */
#ifdef _DEMO_INTERRUPT
#pragma vector=TIMER0_A1_VECTOR
#endif
__interrupt void CCR12_handler(void)
{
	switch(TA0IV) {
	case 2:                                  // CCR1
		P1OUT ^= BIT6;
		break;
	}
}

void demo_CCR1_interrupt(void) {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P1DIR |= BIT0;							  // CCR0 中断
	P1DIR |= BIT6;                            // CCR1 中断

	CCTL0 = CCIE;
	CCR0 = 0xFFFF >> 1;

	CCTL1 = CCIE;
	CCR1 = 0xFFFF >> 2;

	TACTL = TASSEL_1 + MC_1;

	__bis_SR_register(LPM0_bits + GIE);
}
