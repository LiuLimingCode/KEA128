/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
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
	
	get_clk();//�ϵ���������һ�������������ȡ����Ƶ����Ϣ
	
	OLED_Init();
	while(MPU6050_DMPInit());
	pit_init_ms(pit0, 10);
	set_irq_priority(PIT_CH0_IRQn, 0);
	enable_irq(PIT_CH0_IRQn);
	EnableInterrupts;
	
	while(1);
}

