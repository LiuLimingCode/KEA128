#ifndef _KEA128_ITG3205_H
#define _KEA128_ITG3205_H

#include "headfile.h"

#define ITG3205_ADDR 0x68

#define NO_ERR									(uint8_t)0
#define ERR_IIC_CHECKACK_FAIL		(uint8_t)1
#define ERR_SELTTEST_FAIL				(uint8_t)2
#define ERR_MPU9250INIT_FAIL		(uint8_t)3
#define ERR_DMPINIT_FAIL				(uint8_t)4

typedef enum
{
	DATA_TEMP = 0,
	DATA_X_GYRO = 1,
	DATA_Y_GYRO = 2,
	DATA_Z_GYRO = 3,
}DATA_TypeDef;


/******************* ITG3205寄存器地址 *******************/

#define REGISTER_WHO_AM_I							0x00		//从机地址寄存器
#define REGISTER_SMPLRT_DIV						0x15		//输出分频
#define REGISTER_DLPF_FS							0x16
#define REGISTER_INT_CFG							0x17
#define REGISTER_STATUS								0x1A
#define REGISTER_TEMP_OUTH						0x1B		//存储最近温度传感器的测量值(高8位)
#define REGISTER_TEMP_OUTL						0x1C		//存储最近温度传感器的测量值(低8位)
#define	REGISTER_GYRO_XOUTH						0x1D		//存储最近X轴陀螺仪的测量值(高8位)
#define REGISTER_GYRO_XOUTL						0x1E		//存储最近X轴陀螺仪的测量值(低8位)
#define REGISTER_GYRO_YOUTH						0x1F		//存储最近Y轴陀螺仪的测量值(高8位)
#define REGISTER_GYRO_YOUTL						0x20		//存储最近Y轴陀螺仪的测量值(低8位)
#define REGISTER_GYRO_ZOUTH						0x21		//存储最近Z轴陀螺仪的测量值(高8位)
#define REGISTER_GYRO_ZOUTL						0x22		//存储最近Z轴陀螺仪的测量值(低8位)
#define REGISTER_PWR_MGM							0x3E		//电源管理

/********************** DLPF_FS **********************/

#define FS_SEL_2000 (0x18)

typedef enum															//陀螺仪滤波器设置
{ 
	DLPF_CFG_BANDWIDTH_256 = 0x00,
	DLPF_CFG_BANDWIDTH_188 = 0x01,
	DLPF_CFG_BANDWIDTH_98 = 0x02,
	DLPF_CFG_BANDWIDTH_42 = 0x03,
	DLPF_CFG_BANDWIDTH_20 = 0x04,
	DLPF_CFG_BANDWIDTH_10 = 0x05,
	DLPF_CFG_BANDWIDTH_5 = 0x06,
}DLPF_CFG_TypeDef;

/********************** PWR_MGM **********************/

typedef enum
{
	CLKSEL_8MHZ = 0x00,
	CLKSEL_PLL_X = 0x01,
	CLKSEL_PLL_Y = 0x02,
	CLKSEL_PLL_Z = 0x03,
	CLKSEL_PLL_32_768KHZ = 0x04,
	CLKSEL_PLL_19_2MHZ = 0x05,
	CLKSEL_STOP = 0x07,
}CLKSEL_TypeDef;

uint8_t ITG3205_Init(void);
uint8_t ITG3205_ReadData(DATA_TypeDef Data_Type, int16_t *data);

#endif