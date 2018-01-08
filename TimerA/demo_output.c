/*
 * demo_output.c
 *
 *  Created on: 2018��1��7��
 *      Author: meng4
 */
 /**
 *  TACCRx ����-�ȽϿ��ƼĴ���
 *  |  15 14   |  13 12 | 11 |  10  | 9 |  8  |  7 6 5  |  4  |  3  |  2  |  1  |  0  |
 *  | CAPTMOD  | CCISI  | SCS| SCCIx|   | CAP | OUTMOD  |CCIE | CCI | OUT | COV |CCIF |
 *  TimerA�ж������Ƚ�ͨ����ÿ��ͨ�������Լ��Ŀ��ƼĴ���CCTLX��
 *
 * ��TACCRx�У�ͨ��CAPλѡ�����ģʽ���ǱȽ�ģʽ
 *   0   �Ƚ�ģʽ
 *   1   ����ģʽ
 *
 * ��TACCRx�У��жϿ���λ��CCIE
 *   0   ��ֹ�ж�
 *   1   �����ж�
 *
 * ��TACCRx�У�OUTMODλ��3λ���������ģʽ��
 *  OUTMOD2 OUTMOD1 OUTMOD0
 *     0       0       0      ֱ�����       (OUTMOD_0)
 *     0       0       1      ��λ               (OUTMOD_1)
 *     0       1       0      ��ת-��λ     (OUTMOD_2)
 *     0       1       1      ��λ-��λ     (OUTMOD_3)
 *     1       0       0      ��ת               (OUTMOD_4)
 *     1       0       1      ��λ               (OUTMOD_5)
 *     1       1       0      ��ת-��λ     (OUTMOD_6)
 *     1       1       1      ��λ-��λ     (OUTMOD_7)
 *
 *  ��������TAR�е�ֵ���ﵽTACCRx���趨ֵ���ᴥ�����״̬�ĸı䡣���ģʽ�������״̬��θı䡣
 *
 *  ֱ�����(mode 0)�����״ֱ̬����TACCTLx���������λOUT���ơ�����ģʽ���������ó�ʼ�����
 *
 *	����(mode 4): ���״̬������ת�����磺0->1, 1->0
 *
 *	��λ(mode 1)�͸�λ(mode 5): ��λ�����״̬��Ϊ1�� ��λ�����״̬��Ϊ0
 *
 *  ��λ-��λ(mode 7)����λ-��λ(mode 3): ����ģʽ�£���TAR��ֵ�ﵽTACCRx���趨ֵ��ִ�е�һ��������
 *   ��TAR��ֵ���»ص�0��ִ�еڶ���������
 *
 *  ��ת-��λ(mode 2)�ͷ�ת-��λ(mode 6): ������ģʽ�£���TAR��ֵ�ﵽTACCRx���趨ֵ��ִ�е�һ��������
 *   ��TAR��ֵ�ﵽTACCR0���趨ֵ��ִ�еڶ���������
 *
 * ��TACCRx�У�OUTλΪ�������λ��ֻ�������ģʽΪmode 0��Ч
 * 	0    ����͵�ƽ
 * 	1    ����ߵ�ƽ
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
