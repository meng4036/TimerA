/**
 * MSP430g2553的TimerA模块具有两个16位的定时器：Timer0_A(TA0), Timer1_A(TA1)。
 * 两个定时器都有三个捕捉/比较寄存器，具有输入捕捉，输出比较功能。
 *
 * 在M430G2553头文件中有如下定义：
 * #define TIMER0_A1_VECTOR (8 * 2u)  // 0xFFF0 Timer0_A CC1, TA0
 * #define TIMER0_A0_VECTOR (9 * 2u)  // 0xFFF2 Timer0_A CC0
 * #define TIMER1_A1_VECTOR (12 * 2u) // 0xFFF8 Timer1_A CC1-4, TA1
 * #define TIMER1_A0_VECTOR (13 * 2u) // 0xFFFA Timer1_A CC0
 * 可见，每个定时器都有两个中断向量。
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



