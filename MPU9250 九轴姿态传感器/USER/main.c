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

int16 MPU9250_Data[9];

void PIT_CH0_IRQHandler(void)
{
	PIT_FlAG_CLR(pit0);

	MPU9250_ReadData(DATA_X_ACCEL, &MPU9250_Data[0]);
	MPU9250_ReadData(DATA_Y_ACCEL, &MPU9250_Data[1]);
	MPU9250_ReadData(DATA_Z_ACCEL, &MPU9250_Data[2]);
	MPU9250_ReadData(DATA_X_GYRO, &MPU9250_Data[3]);
	MPU9250_ReadData(DATA_Y_GYRO, &MPU9250_Data[4]);
	MPU9250_ReadData(DATA_Z_GYRO, &MPU9250_Data[5]);
			
	AK8963_ReadData(DATA_Z_MAG, &MPU9250_Data[6]);
	AK8963_ReadData(DATA_Y_MAG, &MPU9250_Data[7]);
	AK8963_ReadData(DATA_Z_MAG, &MPU9250_Data[8]);
	
	OLED_P6x8Int(0, 0, MPU9250_Data[0], -5);
	OLED_P6x8Int(0, 1, MPU9250_Data[1], -5);
	OLED_P6x8Int(0, 2, MPU9250_Data[2], -5);
	
	OLED_P6x8Int(0, 5, MPU9250_Data[3], -5);
	OLED_P6x8Int(0, 6, MPU9250_Data[4], -5);
	OLED_P6x8Int(0, 7, MPU9250_Data[5], -5);
}

int main(void)
{
	get_clk();              //获取时钟频率 必须执行
	
	OLED_Init();
	while(MPU9250_Init());
	pit_init_ms(pit0, 10);
	set_irq_priority(PIT_CH0_IRQn, 0);
	enable_irq(PIT_CH0_IRQn);
	EnableInterrupts;
	
	while(1);
}
