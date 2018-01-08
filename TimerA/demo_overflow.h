/*
 * demo_overflow.h
 *
 *  Created on: 2018年1月3日
 *      Author: meng4
 */

#ifndef DEMO_OVERFLOW_H_
#define DEMO_OVERFLOW_H_

/**
 * TimerA 16位计数器TAR(计数寄存器)
 *  TAR存放当前的计数值，计数范围为0 ~ 65535 (0xffff)。
 *
 * 演示TimerA的控制寄存器TACTL
 *  15-10    9     8     7    6    5    4    3    2    1     0
 *  未用       SSEL1 SSEL2  ID1  ID0  MC1  MC0  未用    CLR  TAIE TAIFG
 *
 * 在TimerA的控制寄存器TACTL中，通过SSEL1, SSEL0时钟源控制位，选定定时器输入分频器的时钟源
 *  SSEL1  SSEL0   输入时钟源
 *    0      0      TACLK    外部引脚引入信号	(TASSEL_0)
 *    0      1      ACLK     辅助时钟                 (TASSEL_1)
 *    1      0      SMCLK    子系统时钟             (TASSEL_2)
 *    1      1      INCLK    见MCU文档               (TASSEL_3)
 *
 * 在TimerA的控制寄存器TACTL中，通过ID1, ID0输入分频控制位，配置分频模式
 *  ID1  ID0
 *   0    0     不分频     (ID_0)
 *   0    1     2分频       (ID_1)
 *   1    0     4分频       (ID_2)
 *   1    1     8分频       (ID_3)
 *
 * 在TimerA的控制寄存器TACTL中，通过MC1, MC0计数模式控制位，配置计数模式
 *  MC1  MC0
 *   0    0    停止模式             (MC_0)
 *   0    1    增计数模式         (MC_1)
 *   1    0    连续计数模式     (MC_2)
 *   1    1    增减计数模式     (MC_3)
 *
 * 停止模式：定时器暂停计数，保留寄存器中的值。下次启动后，接着计数
 *
 * 增计数模式：从0x0000开始，计数到TACCR0的值。下一个计数后，从0重新开始。计数周期为(TACCR0+1)
 *  当计数器计数到TACCR0时，捕获比较中断寄存器CCTL0的CCIFG标志置位。
 *  例如，TACCR0=4，计数值序列为0,1,2,3,4,0,1,2,...
 *  这种模式常用于提供频率固定的输出，如PWM。
 *
 * 连续计数模式：从0x0000开始，计数到0xFFFF,溢出后，从0x0000开始重新计数。计数周期为2^16 = 65,536 次。
 *  当溢出后，回到0x0000重新计数时，TAIFG置位(TAIFG为溢出标记)，但并不中断，只有TAIE=1时才产生中断。
 *  这种计数模式常用于捕获输入，或提供频率不固定的输出。
 *
 * 增减计数模式：从0x0000开始，计数到TACCR0的值后，再计数到0x0000。计数周期为(2*TACCR0)
 *  当计数器计数到TACCR0时，捕获比较控制寄存器CCTL0的CCIF0置位；当减到0时，TAIFG置位。
 *  例如，TACCR0=3，计数值序列为0,1,2,3,2,1,0,1,2,...
 *  这种模式常用于中心PWM。
 *
 * 在TimerA的控制寄存器TACTL中，CLR为定时器清除位
 *  CLR置位时，定时器和输入分频器复位，定时器在下一个有效输入沿开始工作，CLR由硬件自动复位。
 *
 * 在TimerA的控制寄存器TACTL中，TAIE为计数器溢出中断控制位
 *  0  禁止计数器溢出中断
 *  1  允许计数器溢出中断
 *
 * 在TimerA的控制寄存器TACTL中，TAIFG为计数器溢出中断标记位
 *  增计数模式：当计数器由TACCR0计数到0时，TAIFG置位 (只有溢出后到0，才会置位)
 *  连续计数模式： 计数器计数0xffff后重新回到0时，TAIFG置位
 *  增减计数模式：当计数器计数由TACCR0减计数到0时，TAIFG置位
 */

#define MAX_CNT 0xFFFF
#define HALF_CNT (MAX_CNT >> 2)

void up_mode_overflow();
void cont_mode_overflow();
void updown_mode_overflow();

#endif /* DEMO_OVERFLOW_H_ */
