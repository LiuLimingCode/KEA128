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



int main(void)
{
	int16 ADC_Read[8] = {0};
	get_clk();              //获取时钟频率 必须执行
	OLED_Init();
	MCP41_Init();
	Shangweiji_Init();
	gpio_init(G2, GPI, 1);
	gpio_init(G3, GPI, 1);
	adc_init(AD_CH_R__, ADC_8bit);
	
	OLED_P6x8Str(0, 0, "P1:");
	
//	MCP41_SetCmd(2, 0xff);

	EnableInterrupts;
	
	while(1)
	{
		if(gpio_get(G2) == 0)
		{
			MCP41_SetValue(3, 120);
			while(!gpio_get(G2));
		}
		if(gpio_get(G3) == 0)
		{
			MCP41_SetValue(3, 40);
			while(!gpio_get(G3));
		}
		
		
		ADC_Read[0] = adc_read(AD_CH_R__, 1); 

		OLED_P6x8Int(24, 0, ADC_Read[0], 3);
		
		
		
		systick_delay_ms(100);
	}
}
