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

int16 ITG3205_Data[9] = {0};

uint8 PIT0_Delay_5ms = 0;

void PIT_CH0_IRQHandler(void)
{
	PIT_FlAG_CLR(pit0);
  PIT0_Delay_5ms = 1;
}



void shangweiji_Send(void)
{
  float shangweiji_data[8] = {0};
  
//  shangweiji_data[0] = 0;
//  shangweiji_data[1] = 0;
//  shangweiji_data[2] = 0;
//  shangweiji_data[3] = 0;
//  shangweiji_data[4] = 0;
//  shangweiji_data[5] = 0;
//  shangweiji_data[6] = 0;
//  shangweiji_data[7] = 0;
	
	for(int8 temp = 0; temp < 8; ++temp)
	shangweiji_data[temp] = ITG3205_Data[temp];
	
  vcan_sendware((uint8 *)shangweiji_data, sizeof(shangweiji_data));
// 
//  WirelessSerial(speed_value_L, ASK_SPEED, speed_value_R , SPEED_PID_Output);
}

int main(void)
{
	get_clk();              //获取时钟频率 必须执行
	
	OLED_Init();
	UI_Init();
	pit_init_ms(pit0, 5);
  set_irq_priority(PIT_CH0_IRQn, 0);
	
	if(ITG3205_Init())
		OLED_P6x8Str(0, 3, "MPU Init Fail!");
	
	enable_irq (PIT_CH0_IRQn);
	EnableInterrupts;
	
	while(1)
	{
		if(PIT0_Delay_5ms == 1)
		{
			PIT0_Delay_5ms = 0;
//			ITG3205_ReadData(DATA_X_GYRO, &ITG3205_Data[0]);
//			ITG3205_ReadData(DATA_Y_GYRO, &ITG3205_Data[1]);
			ITG3205_ReadData(DATA_Z_GYRO, &ITG3205_Data[2]);
//			OLED_P6x8Int(0, 0, ITG3205_Data[0] / 16.0, 5);
			shangweiji_Send();
		}
	}
}
