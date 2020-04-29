/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		中断文件
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/



#ifndef _isr_h
#define _isr_h

#include "common.h"

//所有在 isr.c使用的全局变量在这里声明
extern int8 Flag_Huandao;
extern int8 Flag_Huandao_DIR;
extern int8 Flag_Speed;
extern int8 Flag_Direction;
extern int8 Flag_Run;
extern int8 Flag_Stop;
extern int16 Flag_timeset;
extern int16 Flag_time;
extern int8 Flag_Warning;
extern int8 Flag_PIT;
extern int8 Flag_Motor;
extern int8 Flag_Podao;
extern int16 Flag_PodaoTimer;
extern int8 Flag_Timer;
extern int8 Flag_Magnet;

void PIT_CH0_IRQHandler(void);
void NMI_Handler(void);
void HardFault_Handler(void);

#endif
