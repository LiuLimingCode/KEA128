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

int16 GZ_Data, AX_Data;

int main(void)
{
	get_clk();              //��ȡʱ��Ƶ�� ����ִ��
	
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
