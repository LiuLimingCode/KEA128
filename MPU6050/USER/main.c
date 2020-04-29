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

typedef struct
{
  int16 Raw_Data[7];
  int16 Correct_Data[7];
  float Yaw;
  float Pitch;
  float Roll;
}MPU6050_DATA_Typedef;

uint8 PIT0_Delay_5ms = 0;
MPU6050_DATA_Typedef MPU6050_Data = {0};
float angleAx = 0, gyroGy = 0;

void PIT_CH0_IRQHandler(void)
{
	PIT_FlAG_CLR(pit0);
  PIT0_Delay_5ms = 1;
}

void MPU6050_Correct(void)
{
  uint8 times;
  int16 temp;
  for(times = 0; times < 128; times++)
  {
    if(MPU6050_ReadData(DATA_X_GYRO, &temp)) temp = 0;
    MPU6050_Data.Correct_Data[DATA_X_GYRO] += temp;
    if(MPU6050_ReadData(DATA_Y_GYRO, &temp)) temp = 0;
    MPU6050_Data.Correct_Data[DATA_Y_GYRO] += temp;
    if(MPU6050_ReadData(DATA_Z_GYRO, &temp)) temp = 0;
    MPU6050_Data.Correct_Data[DATA_Z_GYRO] += temp;
    systick_delay_ms(10);
  }
  MPU6050_Data.Correct_Data[DATA_X_GYRO] >>= 7;
  MPU6050_Data.Correct_Data[DATA_Y_GYRO] >>= 7;
  MPU6050_Data.Correct_Data[DATA_Z_GYRO] >>= 7;
}

void shangweiji_Send(void)
{
  float shangweiji_data[8] = {0};
  
  shangweiji_data[0] = angleAx;
  shangweiji_data[1] = gyroGy;
  shangweiji_data[2] = 0;
  shangweiji_data[3] = 0;
  shangweiji_data[4] = 0;
  shangweiji_data[5] = 0;
  shangweiji_data[6] = 0;
  shangweiji_data[7] = 0;
  
  vcan_sendware((uint8 *)shangweiji_data, sizeof(shangweiji_data));
// 
//  WirelessSerial(speed_value_L, ASK_SPEED, speed_value_R , SPEED_PID_Output);
}

int main(void)
{
	get_clk();              //获取时钟频率 必须执行
	
	OLED_Init();
//	Shangweiji_Init();
	pit_init_ms(pit0, 10);
  set_irq_priority(PIT_CH0_IRQn, 0);
	
//	MPU6050_Init();
	MPU6050_DMPInit();
//	if(MPU6050_Init())OLED_P6x8Str(0, 3, "MPU Init Fail!");
//	MPU6050_Correct();
	
	enable_irq (PIT_CH0_IRQn);
	EnableInterrupts;
	
	while(1)
	{
		if(PIT0_Delay_5ms)
		{
			MPU6050_ReadData(DATA_X_GYRO, &MPU6050_Data.Raw_Data[0]);
//			if(MPU6050_DMPGetData(&MPU6050_Data.Pitch, &MPU6050_Data.Roll, &MPU6050_Data.Yaw))
//				MPU6050_Data.Roll = MPU6050_Data.Yaw;
//			PIT0_Delay_5ms = 0;
//			if(MPU6050_ReadData(DATA_Y_GYRO, &MPU6050_Data.Raw_Data[DATA_Y_GYRO]))OLED_P6x8Str(0, 7, "MPU Read Fail!");
//			if(MPU6050_ReadData(DATA_X_ACCEL, &MPU6050_Data.Raw_Data[DATA_X_ACCEL]))OLED_P6x8Str(0, 7, "MPU Read Fail!");
//			if(MPU6050_ReadData(DATA_Z_ACCEL, &MPU6050_Data.Raw_Data[DATA_Z_ACCEL]))OLED_P6x8Str(0, 7, "MPU Read Fail!");
//			MPU6050_Data.Raw_Data[DATA_Y_GYRO] -= MPU6050_Data.Correct_Data[DATA_Y_GYRO];
//			
//			angleAx = atan2(MPU6050_Data.Raw_Data[DATA_X_ACCEL], MPU6050_Data.Raw_Data[DATA_Z_ACCEL])* 180/ 3.14; 
//			gyroGy = -MPU6050_Data.Raw_Data[DATA_Y_GYRO]/ 16.4;
//			
//			shangweiji_Send();
//			
//			OLED_P6x8Flo(0, 0, angleAx);
//			OLED_P6x8Flo(0, 1, gyroGy);
		}
	}
}
