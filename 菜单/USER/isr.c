/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		�ж��ļ�
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/
 
#include "headfile.h"

int8 Flag_Huandao = 0;								//������־
int8 Flag_Huandao_DIR = 0;				//��������
int8 Flag_Run = OFF;									//������־
int8 Flag_Stop = OFF;									//ͣ����־
int16 Flag_timeset = 70;							//��ʱͣ����λ0.1/s
int16 Flag_time = 0;									//��ǰ����ʱ��
int8 Flag_Warning = 0;								//������������־
int8 Flag_PIT = 0;
int8 Flag_Motor = ON;
int8 Flag_Podao = 0;
int16 Flag_PodaoTimer = 0;
int8 Flag_Timer = 0;
int8 Flag_Magnet = 1;

/**
 * @��ע    ȫ�����ƺ���
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
 * @��ע    �����쳣�ж�����ͣ������
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
