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
#include "math.h"

int16 GZ_Data, AX_Data;

int main(void)
{
	get_clk();              //获取时钟频率 必须执行
	
	OLED_Init();
	FXOS8700_Init();
	FXAS2100_Init();
	
	while(1)
	{
		FXAS2100_ReadData(FXAS2100_DATA_Z_GYRO, &GZ_Data);
		FXOS8700_ReadData(FXOS8700_DATA_X_ACCEL, &AX_Data);
		OLED_P6x8Int(0, 0, GZ_Data, -5);
		OLED_P6x8Int(0, 0, AX_Data, -5);
		
		systick_delay_ms(10);
	}
}
