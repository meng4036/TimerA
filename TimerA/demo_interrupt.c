/*
 * demo_interrupt.c
 *
 *  Created on: 2018��1��6��
 *      Author: meng4
 */
#include <msp430.h>
#include "demo_interrupt.h"

/**
 * ��ʾ�Ƚ�ģʽ�£������Ƚ�-����ģ��������ж�
 */

/* CCR0���жϴ����� */
#ifdef _DEMO_INTERRUPT
#pragma vector=TIMER0_A0_VECTOR
#endif
__interrupt void CCR0_handler(void)
{
	P1OUT ^= BIT0;
}

/* �Ƚ�ģʽCCR0�жϣ�TAR������CCR0��ֵʱ�������ж� */
void demo_CCR0_interrupt(void) {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P1DIR |= BIT0;							  // ��ʾ���

	CCTL0 = CCIE;
	CCR0 = 0xFFFF >> 1;
	TACTL = TASSEL_2 + MC_1 + TAIE;

	__bis_SR_register(LPM0_bits + GIE);
}


/* CCR1��CCR2���жϴ����� */
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
	P1DIR |= BIT0;							  // CCR0 �ж�
	P1DIR |= BIT6;                            // CCR1 �ж�

	CCTL0 = CCIE;
	CCR0 = 0xFFFF >> 1;

	CCTL1 = CCIE;
	CCR1 = 0xFFFF >> 2;

	TACTL = TASSEL_1 + MC_1;

	__bis_SR_register(LPM0_bits + GIE);
}
