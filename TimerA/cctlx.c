/*
 * cctlx.c
 *
 *  Created on: 2018年1月3日
 *      Author: meng4
 */
/**
 * TACCRx 捕获-比较寄存器
 *  TACCRx存放设定的计数值，设定范围为0 ~ 65535 (0xffff)。
 *  在比较模式，当计数器TAR中的值，达到TACCRx的设定值，会触发以下动作
 *     设置内部信号EQUx
 *     设置CCIFGx位，并请求捕获-比较中断
 *     输出信号OUTx根据设定改变
 *
 * TACCRx 捕获-比较控制寄存器
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
 *  复位-置位(mode 7)和置位-复位(mode 3): 当TAR的值达到TACCRx的设定值，执行第一个动作；
 *   当TAR的值重新回到0，执行第二个动作。
 *
 *  反转-复位(mode 2)和反转-置位(mode 6): 当TAR的值达到TACCRx的设定值，执行第一个动作；
 *   当TAR的值达到TACCR0的设定值，执行第二个动作。
 *
 * 在TACCRx中，OUT位为输出控制位，只有在输出模式为mode 0有效
 * 	0    输出低电平
 * 	1    输出高电平
 *
 * 在TACCRx中，溢出中断标志位是COV
 *
 * 在TACCRx中，捕获-比较中断标志位是CCIF
 *  捕获模式：寄存器TACCRx捕获了定时器TAR的值置位
 *  比较模式：定时器TAR的值等于寄存器TACCRx的值置位
 *
 * 在TACCRx中，捕获模式通过捕获模式控制位CAPTMOD设定
 *  CAPTMOD1 CAPTMOD0
 *     0        0       禁用捕获模式
 *     0        1       上升沿捕获模式
 *     1        0       下降沿捕获模式
 *     1        1       上升沿下降沿都捕获模式
 *
 * 在TACCRx中，捕获模式输入端的选择通过CCISI位设定
 *  CCIS1 CCIS0
 *    0     0     选择CCIxA输入
 *    0     1     选择CCIxB输入
 *    1     0     选择GND输入
 *    1     1     选择VCC输入
 *  CCIxA可CCIxB来源于定时器模块的外部。
 *   CCIxA连接到外部引脚TAx，而CCIxB连接到MCU内部的其他模块。
 *
 * 在TACCRx中，捕获信号与定时器输入时钟信号是否同步，通过SCS为设定
 *   0   异步捕获
 *   1   同步捕获
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
