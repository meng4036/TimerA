/*
 * demo_upmode.c
 *
 *  Created on: 2018��1��2��
 *      Author: meng4
 */
#include <msp430.h>
#include "demo_overflow.h"

/**
 * ��ʾTimerA�ļ���ģʽ - ��������ģʽ
 *
 * TACTL���ƼĴ���
 *  15-10    9     8     7    6    5    4    3    2    1     0
 *  δ��       SSEL1 SSEL2  ID1  ID0  MC1  MC0  δ��    CLR  TAIE TAIFG
 *
 * ��TimerA�Ŀ��ƼĴ���TACTL�У�ͨ��MC1, MC0����ģʽ����λ�����ü���ģʽ
 *  MC1  MC0
 *   0    0    ֹͣģʽ             (MC_0)
 *   0    1    ������ģʽ         (MC_1)
 *   1    0    ��������ģʽ     (MC_2)
 *   1    1    ��������ģʽ     (MC_3)
 *
 * ��TimerA�Ŀ��ƼĴ���TACTL�У�TAIEΪ����������жϿ���λ
 *  0  ��ֹ����������ж�
 *  1  �������������ж�
 *
 * ��TimerA�Ŀ��ƼĴ���TACTL�У�TAIFGΪ����������жϱ��λ
 *  ������ģʽ������������TACCR0������0ʱ��TAIFG��λ (ֻ�������0���Ż���λ)
 *  ��������ģʽ�� ����������0xffff�����»ص�0ʱ��TAIFG��λ
 *  ��������ģʽ����������������TACCR0��������0ʱ��TAIFG��λ
 */

/* ��������жϴ����� */
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

/* ��ʾ������ģʽ�����,������TACCR0 */
void up_mode_overflow() {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P1DIR |= BIT6;                            // P1.6 output

	CCR0 = 0xFFFF >> 2;						// ������ֵ��1/2
	TACTL = TASSEL_1 + MC_1 + TAIE;

	__bis_SR_register(LPM0_bits + GIE);
}

/* ��ʾ��������ģʽ�����,������0xFFFF */
void cont_mode_overflow() {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P1DIR |= BIT6;                            // P1.6 output

	TACTL = TASSEL_1 + MC_2 + TAIE;           // ACLK, continue mode, enable interrupt

	__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

/* ��ʾ��������ģʽ�������������TACCR0���ټ���0 */
void updown_mode_overflow() {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P1DIR |= BIT6;                            // P1.6 output

	CCR0 = 0xFFFF >> 1;					      // ACLK Ƶ�ʵ�1/2
	TACTL = TASSEL_1 + MC_3 + TAIE;           // ACLK, continue mode, enable interrupt

	__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}
