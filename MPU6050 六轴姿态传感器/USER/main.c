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

float pitch, roll, yaw;
int16 gyroX, gyroY, gyroZ;

void PIT_CH0_IRQHandler(void)
{
	PIT_FlAG_CLR(pit0);

	MPU6050_DMPGetData(&pitch, &roll, &yaw);
	MPU6050_ReadData(MPU6050_GYRO_X, &gyroX);
	MPU6050_ReadData(MPU6050_GYRO_Y, &gyroY);
	MPU6050_ReadData(MPU6050_GYRO_Z, &gyroZ);
	
	OLED_P6x8Flo(0, 0, pitch, -3);
	OLED_P6x8Flo(0, 1, roll, -3);
	OLED_P6x8Flo(0, 2, yaw, -3);
	
	OLED_P6x8Int(0, 5, gyroX, 4);
	OLED_P6x8Int(0, 6, gyroY, 4);
	OLED_P6x8Int(0, 7, gyroZ, 4);
}

int main(void)
{
	
	get_clk();//上电后必须运行一次这个函数，获取各个频率信息
	
	OLED_Init();
	while(MPU6050_DMPInit());
	pit_init_ms(pit0, 10);
	set_irq_priority(PIT_CH0_IRQn, 0);
	enable_irq(PIT_CH0_IRQn);
	EnableInterrupts;
	
	while(1);
}

