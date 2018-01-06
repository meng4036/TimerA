/*
 * cctlx.c
 *
 *  Created on: 2018��1��3��
 *      Author: meng4
 */
/**
 * TACCRx ����-�ȽϼĴ���
 *  TACCRx����趨�ļ���ֵ���趨��ΧΪ0 ~ 65535 (0xffff)��
 *  �ڱȽ�ģʽ����������TAR�е�ֵ���ﵽTACCRx���趨ֵ���ᴥ�����¶���
 *     �����ڲ��ź�EQUx
 *     ����CCIFGxλ�������󲶻�-�Ƚ��ж�
 *     ����ź�OUTx�����趨�ı�
 *
 * TACCRx ����-�ȽϿ��ƼĴ���
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
 *  ��λ-��λ(mode 7)����λ-��λ(mode 3): ��TAR��ֵ�ﵽTACCRx���趨ֵ��ִ�е�һ��������
 *   ��TAR��ֵ���»ص�0��ִ�еڶ���������
 *
 *  ��ת-��λ(mode 2)�ͷ�ת-��λ(mode 6): ��TAR��ֵ�ﵽTACCRx���趨ֵ��ִ�е�һ��������
 *   ��TAR��ֵ�ﵽTACCR0���趨ֵ��ִ�еڶ���������
 *
 * ��TACCRx�У�OUTλΪ�������λ��ֻ�������ģʽΪmode 0��Ч
 * 	0    ����͵�ƽ
 * 	1    ����ߵ�ƽ
 *
 * ��TACCRx�У�����жϱ�־λ��COV
 *
 * ��TACCRx�У�����-�Ƚ��жϱ�־λ��CCIF
 *  ����ģʽ���Ĵ���TACCRx�����˶�ʱ��TAR��ֵ��λ
 *  �Ƚ�ģʽ����ʱ��TAR��ֵ���ڼĴ���TACCRx��ֵ��λ
 *
 * ��TACCRx�У�����ģʽͨ������ģʽ����λCAPTMOD�趨
 *  CAPTMOD1 CAPTMOD0
 *     0        0       ���ò���ģʽ
 *     0        1       �����ز���ģʽ
 *     1        0       �½��ز���ģʽ
 *     1        1       �������½��ض�����ģʽ
 *
 * ��TACCRx�У�����ģʽ����˵�ѡ��ͨ��CCISIλ�趨
 *  CCIS1 CCIS0
 *    0     0     ѡ��CCIxA����
 *    0     1     ѡ��CCIxB����
 *    1     0     ѡ��GND����
 *    1     1     ѡ��VCC����
 *  CCIxA��CCIxB��Դ�ڶ�ʱ��ģ����ⲿ��
 *   CCIxA���ӵ��ⲿ����TAx����CCIxB���ӵ�MCU�ڲ�������ģ�顣
 *
 * ��TACCRx�У������ź��붨ʱ������ʱ���ź��Ƿ�ͬ����ͨ��SCSΪ�趨
 *   0   �첽����
 *   1   ͬ������
 *
 */

#include <msp430.h>

void cctl_ccr() {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P1DIR |= 0x01;                            // P1.0 output

	CCTL1 = CCIE;                             // CCR1 interrupt enabled
	CCR1 = 1000;
	TACTL = TASSEL_2 + MC_2;                  // SMCLK, Contmode

	__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}


void output_pwm() {
	WDTCTL = WDTPW + WDTHOLD;
	P1DIR |= 0x0c;
	P1SEL |= 0x0c;

	CCR0 = 100 - 1;
	CCTL1 = OUTMOD_7;
	CCR1 = 20;

	CCTL2 = OUTMOD_7;
	CCR2 = 80;

	TACTL = TASSEL_1 + MC_1;

	__bis_SR_register(LPM3_bits);
}
/*
#pragma vector=TIMER0_A1_VECTOR
__interrupt void timer_ccr() {
	switch (TA0IV) {
		case 2: {
			P1OUT ^= 0x01;
			CCR1 += 1000;
			break;
		}
		case 4: {
			break;
		}
		case 10: {
			break;
		}
	}
}

*/
