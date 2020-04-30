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
	get_clk();              //��ȡʱ��Ƶ�� ����ִ��
	
	OLED_Init();
	while(MPU9250_Init());
	pit_init_ms(pit0, 10);
	set_irq_priority(PIT_CH0_IRQn, 0);
	enable_irq(PIT_CH0_IRQn);
	EnableInterrupts;
	
	while(1);
}
