/**
 * MSP430g2553��TimerAģ���������16λ�Ķ�ʱ����Timer0_A(TA0), Timer1_A(TA1)��
 * ������ʱ������������׽/�ȽϼĴ������������벶׽������ȽϹ��ܡ�
 *
 * ��M430G2553ͷ�ļ��������¶��壺
 * #define TIMER0_A1_VECTOR (8 * 2u)  // 0xFFF0 Timer0_A CC1, TA0
 * #define TIMER0_A0_VECTOR (9 * 2u)  // 0xFFF2 Timer0_A CC0
 * #define TIMER1_A1_VECTOR (12 * 2u) // 0xFFF8 Timer1_A CC1-4, TA1
 * #define TIMER1_A0_VECTOR (13 * 2u) // 0xFFFA Timer1_A CC0
 * �ɼ���ÿ����ʱ�����������ж�������
 *
 */

#include <msp430.h> 

//#include "demo_contmode.h"
//#include "demo_upmode.h"
//#include "demo_overflow.h"
#include "cctlx.h"

int main(void)
{
	//demo_overflow();
	//cctl_ccr();
	output_pwm();
}


