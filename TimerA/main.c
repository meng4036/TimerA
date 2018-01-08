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
#include "demo_overflow.h"
#include "demo_interrupt.h"
#include "demo_output.h"
//#include "cctlx.h"

#define _DEMO_OUTPUT_MODE

int main()
{
	//up_mode_overflow();
	//cont_mode_overflow();
	//updown_mode_overflow();

	//demo_CCR1_interrupt();
	//demo_output_mode4();
	//demo_output_mode7();
	demo_output_mode3();
}
