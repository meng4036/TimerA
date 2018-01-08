/*
 * demo_output.c
 *
 *  Created on: 2018年1月7日
 *      Author: meng4
 */
 /**
 *  TACCRx 捕获-比较控制寄存器
 *  |  15 14   |  13 12 | 11 |  10  | 9 |  8  |  7 6 5  |  4  |  3  |  2  |  1  |  0  |
 *  | CAPTMOD  | CCISI  | SCS| SCCIx|   | CAP | OUTMOD  |CCIE | CCI | OUT | COV |CCIF |
 *  TimerA有多个捕获比较通道，每个通道都有自己的控制寄存器CCTLX。
 *
 * 在TACCRx中，通过CAP位选择控制模式还是比较模式
 *   0   比较模式
 *   1   捕获模式
 *
 * 在TACCRx中，中断控制位是CCIE
 *   0   禁止中断
 *   1   允许中断
 *
 * 在TACCRx中，OUTMOD位有3位，设置输出模式，
 *  OUTMOD2 OUTMOD1 OUTMOD0
 *     0       0       0      直接输出       (OUTMOD_0)
 *     0       0       1      置位               (OUTMOD_1)
 *     0       1       0      反转-复位     (OUTMOD_2)
 *     0       1       1      置位-复位     (OUTMOD_3)
 *     1       0       0      反转               (OUTMOD_4)
 *     1       0       1      复位               (OUTMOD_5)
 *     1       1       0      反转-置位     (OUTMOD_6)
 *     1       1       1      复位-置位     (OUTMOD_7)
 *
 *  当计数器TAR中的值，达到TACCRx的设定值，会触发输出状态的改变。输出模式决定输出状态如何改变。
 *
 *  直接输出(mode 0)：输出状态直接由TACCTLx的输出控制位OUT控制。这种模式常用于设置初始输出。
 *
 *	触发(mode 4): 输出状态发生反转。例如：0->1, 1->0
 *
 *	置位(mode 1)和复位(mode 5): 置位将输出状态变为1， 复位将输出状态变为0
 *
 *  复位-置位(mode 7)和置位-复位(mode 3): 在增模式下，当TAR的值达到TACCRx的设定值，执行第一个动作；
 *   当TAR的值重新回到0，执行第二个动作。
 *
 *  反转-复位(mode 2)和反转-置位(mode 6): 在连续模式下，当TAR的值达到TACCRx的设定值，执行第一个动作；
 *   当TAR的值达到TACCR0的设定值，执行第二个动作。
 *
 * 在TACCRx中，OUT位为输出控制位，只有在输出模式为mode 0有效
 * 	0    输出低电平
 * 	1    输出高电平
 */
#include <msp430.h>
#include "demo_output.h"

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0() {
	P1OUT ^= BIT0;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1() {
	switch (TAIV) {
	case 2:
		CCR1 += 16;
		break;
	case 4:
		CCR2 += 100;
		break;
	case 10:
		P1OUT ^= BIT0;
		break;
	}
}

void demo_output_mode0() {
	WDTCTL = WDTPW + WDTHOLD;

	P1DIR |= BIT0 + BIT1 + BIT2 + BIT3;
	P1SEL |= BIT1 + BIT2 + BIT3;

	CCTL0 = OUTMOD_4 + CCIE;
	CCTL1 = OUTMOD_4 + CCIE;
	CCTL2 = OUTMOD_4 + CCIE;

	TACTL = TASSEL_1 + MC_2 + TAIE;

	__bis_SR_register(LPM3_bits);
}

void demo_output_mode4() {
	WDTCTL = WDTPW + WDTHOLD;
	P1DIR |= BIT0;
	P1DIR |= BIT1;
	P1SEL |= BIT1;

	CCTL0 = OUTMOD_4;

	TACTL = TASSEL_1 + MC_2 + TAIE;

	__bis_SR_register(LPM3_bits);
}

void demo_output_mode7() {
	WDTCTL = WDTPW + WDTHOLD;
	P1DIR |= BIT0;
	P1DIR |= BIT2;
	P1SEL |= BIT2;

	CCR0 = 0xFFFF >> 1;
	CCTL1 = OUTMOD_7;
	CCR1 = 0xFFFF >> 3;

	TACTL = TASSEL_1 + MC_1 + TAIE;

	__bis_SR_register(LPM3_bits + GIE);
}

void demo_output_mode3() {
	WDTCTL = WDTPW + WDTHOLD;
		P1DIR |= BIT0;
		P1DIR |= BIT2;
		P1SEL |= BIT2;

		CCTL0 = CCIE;
		CCR0 = 0xFFFF >> 1;
		CCTL1 = OUTMOD_3;
		CCR1 = 0xFFFF >> 3;

		TACTL = TASSEL_1 + MC_3 + TAIE;

		__bis_SR_register(LPM0_bits + GIE);
}
