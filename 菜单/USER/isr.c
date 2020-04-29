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
 
#include "headfile.h"

int8 Flag_Huandao = 0;								//环岛标志
int8 Flag_Huandao_DIR = 0;				//环岛方向
int8 Flag_Run = OFF;									//开车标志
int8 Flag_Stop = OFF;									//停车标志
int16 Flag_timeset = 70;							//定时停车单位0.1/s
int16 Flag_time = 0;									//当前开车时间
int8 Flag_Warning = 0;								//蜂鸣器报警标志
int8 Flag_PIT = 0;
int8 Flag_Motor = ON;
int8 Flag_Podao = 0;
int16 Flag_PodaoTimer = 0;
int8 Flag_Timer = 0;
int8 Flag_Magnet = 1;

/**
 * @备注    全车控制函数
 */
void PIT_CH0_IRQHandler(void)
{
	PIT_FlAG_CLR(pit0);
	
	Flag_PIT = 1;
	
	g_nSpeedLeft = ((gpio_get(CODER_DIR_L) == 1) ? ftm_count_get(CODER_L) : -ftm_count_get(CODER_L));
	ftm_count_clean(CODER_L);
	g_nSpeedRight = ((gpio_get(CODER_DIR_R) == 0) ? ftm_count_get(CODER_R) : -ftm_count_get(CODER_R));
	ftm_count_clean(CODER_R);
}

/**
 * @备注    发生异常中断立刻停车警告
 */
void NMI_Handler(void)
{
	ftm_pwm_duty(ftm2, PWM_CH_R_Z, 0);
  ftm_pwm_duty(ftm2, PWM_CH_R_F, 0);
  ftm_pwm_duty(ftm2, PWM_CH_L_Z, 0);
  ftm_pwm_duty(ftm2, PWM_CH_R_Z, 0);
	OLED_P8x16Str(20, 0, "NMI_Handler");
	while(1);
}

void HardFault_Handler(void)
{
	ftm_pwm_duty(ftm2, PWM_CH_R_Z, 0);
  ftm_pwm_duty(ftm2, PWM_CH_R_F, 0);
  ftm_pwm_duty(ftm2, PWM_CH_L_Z, 0);
  ftm_pwm_duty(ftm2, PWM_CH_R_Z, 0);
	OLED_P8x16Str(28, 0, "HardFault");
	while(1);
}
