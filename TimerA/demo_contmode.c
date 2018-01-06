/*
 * demo_upmode.c
 *
 *  Created on: 2018��1��2��
 *      Author: meng4
 */
/**
 * TimerA 16λ������TAR(�����Ĵ���)
 *  TAR��ŵ�ǰ�ļ���ֵ��������ΧΪ0 ~ 65535 (0xffff)��
 *
 * ��ʾTimerA�Ŀ��ƼĴ���TACTL
 *  15-10    9     8     7    6    5    4    3    2    1     0
 *  δ��       SSEL1 SSEL2  ID1  ID0  MC1  MC0  δ��    CLR  TAIE TAIFG
 *
 * ��TimerA�Ŀ��ƼĴ���TACTL�У�ͨ��SSEL1, SSEL0ʱ��Դ����λ��ѡ����ʱ�������Ƶ����ʱ��Դ
 *  SSEL1  SSEL0   ����ʱ��Դ
 *    0      0      TACLK    �ⲿ���������ź�	(TASSEL_0)
 *    0      1      ACLK     ����ʱ��                 (TASSEL_1)
 *    1      0      SMCLK    ��ϵͳʱ��             (TASSEL_2)
 *    1      1      INCLK    ��MCU�ĵ�               (TASSEL_3)
 *
 * ��TimerA�Ŀ��ƼĴ���TACTL�У�ͨ��ID1, ID0�����Ƶ����λ�����÷�Ƶģʽ
 *  ID1  ID0
 *   0    0     ����Ƶ     (ID_0)
 *   0    1     2��Ƶ       (ID_1)
 *   1    0     4��Ƶ       (ID_2)
 *   1    1     8��Ƶ       (ID_3)
 *
 * ��TimerA�Ŀ��ƼĴ���TACTL�У�ͨ��MC1, MC0����ģʽ����λ�����ü���ģʽ
 *  MC1  MC0
 *   0    0    ֹͣģʽ             (MC_0)
 *   0    1    ������ģʽ         (MC_1)
 *   1    0    ��������ģʽ     (MC_2)
 *   1    1    ��������ģʽ     (MC_3)
 *
 * ֹͣģʽ����ʱ����ͣ�����������Ĵ����е�ֵ���´������󣬽��ż���
 *
 * ������ģʽ����0x0000��ʼ��������TACCR0��ֵ����һ�������󣬴�0���¿�ʼ����������Ϊ(TACCR0+1)
 *  ��������������TACCR0ʱ������Ƚ��жϼĴ���CCTL0��CCIFG��־��λ��
 *  ���磬TACCR0=4������ֵ����Ϊ0,1,2,3,4,0,1,2,...
 *  ����ģʽ�������ṩƵ�ʹ̶����������PWM��
 *
 * ��������ģʽ����0x0000��ʼ��������0xFFFF,����󣬴�0x0000��ʼ���¼�������������Ϊ2^16 = 65,536 �Ρ�
 *  ������󣬻ص�0x0000���¼���ʱ��TAIFG��λ(TAIFGΪ������)���������жϣ�ֻ��TAIE=1ʱ�Ų����жϡ�
 *  ���ּ���ģʽ�����ڲ������룬���ṩƵ�ʲ��̶��������
 *
 * ��������ģʽ����0x0000��ʼ��������TACCR0��ֵ���ټ�����0x0000����������Ϊ(2*TACCR0)
 *  ��������������TACCR0ʱ������ȽϿ��ƼĴ���CCTL0��CCIF0��λ��������0ʱ��TAIFG��λ��
 *  ���磬TACCR0=3������ֵ����Ϊ0,1,2,3,2,1,0,1,2,...
 *  ����ģʽ����������PWM��
 *
 * ��TimerA�Ŀ��ƼĴ���TACTL�У�CLRΪ��ʱ�����λ
 *  CLR��λʱ����ʱ���������Ƶ����λ����ʱ������һ����Ч�����ؿ�ʼ������CLR��Ӳ���Զ���λ��
 *
 * ��TimerA�Ŀ��ƼĴ���TACTL�У�TAIEΪ�жϿ���λ
 *  0  ��ֹ����������ж�
 *  1  ��������������ж�
 *
 * ��TimerA�Ŀ��ƼĴ���TACTL�У�TAIEΪ����������жϱ��λ
 *  ������ģʽ������������TACCR0������0ʱ��TAIFG��λ (ֻ�������0���Ż���λ)
 *  ��������ģʽ�� ����������0xffff�����»ص�0ʱ��TAIFG��λ
 *  ��������ģʽ����������������TACCR0��������0ʱ��TAIFG��λ
 */

#include <msp430.h>
#include "demo_contmode.h"

void demo_contmode() {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P1DIR |= 0x01;                            // P1.0 output
	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 |= 125000;
	TACTL = TASSEL_2 + ID_3 + MC_2;                  // SMCLK, continue mode

	__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

// Timer A0 interrupt service routine
/*
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	P1OUT ^= 0x01;                            // Toggle P1.0
	CCR0 += 125000;                            // Add Offset to CCR0
}

*/