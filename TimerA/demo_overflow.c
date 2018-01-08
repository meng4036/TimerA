/*
 * demo_upmode.c
 *
 *  Created on: 2018年1月2日
 *      Author: meng4
 */
#include <msp430.h>
#include "demo_overflow.h"

/**
 * 演示TimerA的计数模式 - 连续计数模式
 *
 * TACTL控制寄存器
 *  15-10    9     8     7    6    5    4    3    2    1     0
 *  未用       SSEL1 SSEL2  ID1  ID0  MC1  MC0  未用    CLR  TAIE TAIFG
 *
 * 在TimerA的控制寄存器TACTL中，通过MC1, MC0计数模式控制位，配置计数模式
 *  MC1  MC0
 *   0    0    停止模式             (MC_0)
 *   0    1    增计数模式         (MC_1)
 *   1    0    连续计数模式     (MC_2)
 *   1    1    增减计数模式     (MC_3)
 *
 * 在TimerA的控制寄存器TACTL中，TAIE为计数器溢出中断控制位
 *  0  禁止计数器溢出中断
 *  1  允许计数器溢出中断
 *
 * 在TimerA的控制寄存器TACTL中，TAIFG为计数器溢出中断标记位
 *  增计数模式：当计数器由TACCR0计数到0时，TAIFG置位 (只有溢出后到0，才会置位)
 *  连续计数模式： 计数器计数0xffff后重新回到0时，TAIFG置位
 *  增减计数模式：当计数器计数由TACCR0减计数到0时，TAIFG置位
 */

/* 计数溢出中断处理函数 */
#ifdef _DEMO_OVERFLOW
#pragma vector=TIMER0_A1_VECTOR
__interrupt void overflow_handler(void)
{
	switch(TA0IV) {
	case 10:
		P1OUT ^= 0x40;
		break;
	}
}
#endif

/* 演示增计数模式的溢出,计数到TACCR0 */
void up_mode_overflow() {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P1DIR |= BIT6;                            // P1.6 output

	CCR0 = 0xFFFF >> 2;						// 最大计数值的1/2
	TACTL = TASSEL_1 + MC_1 + TAIE;

	__bis_SR_register(LPM0_bits + GIE);
}

/* 演示连续计数模式的溢出,计数到0xFFFF */
void cont_mode_overflow() {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P1DIR |= BIT6;                            // P1.6 output

	TACTL = TASSEL_1 + MC_2 + TAIE;           // ACLK, continue mode, enable interrupt

	__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

/* 演示增减计数模式的溢出，计数到TACCR0，再减到0 */
void updown_mode_overflow() {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P1DIR |= BIT6;                            // P1.6 output

	CCR0 = 0xFFFF >> 1;					      // ACLK 频率的1/2
	TACTL = TASSEL_1 + MC_3 + TAIE;           // ACLK, continue mode, enable interrupt

	__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}
