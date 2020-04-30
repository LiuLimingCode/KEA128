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

uint8 Pit_flag = 0;

void PIT_CH0_IRQHandler(void)
{
	PIT_FlAG_CLR(pit0);
    Pit_flag = 1;
}

int32 times1,times2,time,delay;
int main(void)
{
	
	get_clk();//上电后必须运行一次这个函数，获取各个频率信息
	OLED_Init();
	pit_init_ms(pit0, 100);
	set_irq_priority(PIT_CH0_IRQn, 0);
	enable_irq(PIT_CH0_IRQn);
	EnableInterrupts;
	
	while(1)
	{
		if(Pit_flag == 1)
		{
			Pit_flag = 0;
			delay = 0;
			times1 = PIT->CHANNEL[pit0].CVAL;
			if(PIT->CHANNEL[pit0].TFLG &  PIT_TFLG_TIF_MASK)
				delay += 1;
			
			OLED_P6x8Int(0, 0, 121, 3);
			OLED_P6x8Int(0, 1, 12, 3);
			OLED_P6x8Int(0, 2, 0, 3);
			OLED_P6x8Int(0, 3, -121, 3);
			OLED_P6x8Int(0, 4, 121, -3);
			OLED_P6x8Flo(0, 5, -12222.22, -5);
			OLED_P6x8Flo(0, 6, 122222.22, 6);
			OLED_P6x8Int(64, 0, 121, 3);
			OLED_P6x8Int(64, 1, 12, 3);
			OLED_P6x8Int(64, 2, 1, 2);
			OLED_P6x8Int(64, 3, -1, 1);
			OLED_P6x8Int(64, 4, 0, 1);
			OLED_P6x8Int(64, 5, -21, 2);
			OLED_P6x8Int(64, 6, -123, 2);
			times2 = PIT->CHANNEL[pit0].CVAL;
			if(PIT->CHANNEL[pit0].TFLG &  PIT_TFLG_TIF_MASK)
				delay += 1;
			time = times1 - times2;
			OLED_P6x8Flo(0, 7, (float)time/(100 * bus_clk_khz) * 100, 2);
			OLED_P6x8Str(64, 7, "ms");
		}
	}
}

