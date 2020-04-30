/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/

#include "headfile.h"

int16 value_L,value_R;
uint8 Pit_5ms_flag = 0;

#define			PWM_CH_R_Z					ftm_ch2
#define			PWM_CH_R_F					ftm_ch3
#define			PWM_CH_L_Z					ftm_ch4
#define			PWM_CH_L_F					ftm_ch5

void PIT_CH0_IRQHandler(void)
{
	PIT_FlAG_CLR(pit0);
	value_L = ftm_count_get(ftm0);
	if(!gpio_get(C5)) value_L = - value_L;
	ftm_count_clean(ftm0);
	value_R = ftm_count_get(ftm1);
	if(gpio_get(H2)) value_R = - value_R;
	ftm_count_clean(ftm1);
	Pit_5ms_flag = 1;
}

int main(void)
{
	get_clk();              //获取时钟频率 必须执行
	OLED_Init();
	pit_init_ms(pit0, 5);
	
	ftm_count_init(ftm0);
	ftm_count_init(ftm1);
	gpio_init(C5, GPI, 1);
	gpio_init(H2, GPI, 1);
	
	ftm_pwm_init(ftm2, PWM_CH_L_Z, 14000, 0);//左轮正60
	ftm_pwm_init(ftm2, PWM_CH_R_Z, 14000, 0);//右轮正65
	ftm_pwm_init(ftm2, PWM_CH_L_F, 14000, 500);//左轮反65
	ftm_pwm_init(ftm2, PWM_CH_R_F, 14000, 0);//右轮反65
	
	set_irq_priority(PIT_CH0_IRQn, 1);
	enable_irq(PIT_CH0_IRQn);
	EnableInterrupts;
    
	while(1)
	{
		if(Pit_5ms_flag)
		{
			Pit_5ms_flag = 0;
			
			OLED_P6x8Int(0, 0, value_L, -4);
			OLED_P6x8Int(0, 1, value_R, -4);
		}
	}
        
	
}
