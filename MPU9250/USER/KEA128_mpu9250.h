#ifndef _KEA128_MPU9250_H
#define _KEA128_MPU9250_H

#include "headfile.h"

#define MPU9250_ADDR 0x68
#define AK8963_ADDR  0x0C

#define NO_ERR									(uint8_t)0
#define ERR_IIC_CHECKACK_FAIL		(uint8_t)1
#define ERR_SELTTEST_FAIL				(uint8_t)2
#define ERR_MPU9250INIT_FAIL		(uint8_t)3
#define ERR_DMPINIT_FAIL				(uint8_t)4

typedef enum
{ 
	DATA_X_ACCEL = 0,
	DATA_Y_ACCEL = 1,
	DATA_Z_ACCEL = 2,
	DATA_TEMP = 3,
	DATA_X_GYRO = 4,
	DATA_Y_GYRO = 5,
	DATA_Z_GYRO = 6,
}MPU9250_DATA_TypeDef;

typedef enum
{ 
	DATA_X_MAG = 0,
	DATA_Y_MAG = 1,
	DATA_Z_MAG = 2,
}AK8963_DATA_TypeDef;

/******************* MPU9250寄存器地址 *******************/

#define REGISTER_SELF_TEST_X_GYRO			0x00		//X轴陀螺仪自测值
#define REGISTER_SELF_TEST_Y_GYRO			0x01		//Y轴陀螺仪自测值
#define REGISTER_SELF_TEST_Z_GYRO			0x02		//Z轴陀螺仪自测值
#define REGISTER_SELF_TEST_X_ACCEL		0x0D		//X轴加速度计自测值
#define REGISTER_SELF_TEST_Y_ACCEL		0x0E		//X轴加速度计自测值
#define REGISTER_SELF_TEST_Z_ACCEL		0x0F		//X轴加速度计自测值
#define REGISTER_XG_OFFSET_H					0x13
#define REGISTER_XG_OFFSET_L					0x14
#define REGISTER_YG_OFFSET_H					0x15
#define REGISTER_YG_OFFSET_L					0x16
#define REGISTER_ZG_OFFSET_H					0x17
#define REGISTER_ZG_OFFSET_L					0x18
#define REGISTER_SMPLRT_DIV						0x19		//输出分频
#define	REGISTER_CONFIG								0x1A		//低通滤波器
#define REGISTER_GYRO_CONFIG					0x1B		//X,Y,Z轴陀螺仪自检,选择陀螺仪量程
#define REGISTER_ACCEL_CONFIG					0x1C		//X,Y,Z轴加速度传感器自检,加速度传感器量程选择
#define REGISTER_ACCEL_CONFIG2				0x1D		//X,Y,Z轴加速度传感器滤波器设置
#define REGISTER_LP_ACCEL_ODR					0x1E
#define REGISTER_WOM_THR							0x1F
#define REGISTER_FIFO_EN							0x23		//FIFO使能寄存器
#define REGISTER_PIN_CFG							0x37
#define	REGISTER_INT_ENABLE						0x38		//中断使能寄存器
#define REGISTER_INT_STATUS						0x3A		//中断状态寄存器
#define	REGISTER_ACCEL_XOUTH					0x3B		//存储最近X轴加速度传感器的测量值(高8位)
#define REGISTER_ACCEL_XOUTL					0x3C		//存储最近X轴加速度传感器的测量值(低8位)
#define REGISTER_ACCEL_YOUTH					0x3D		//存储最近Y轴加速度传感器的测量值(高8位)
#define REGISTER_ACCEL_YOUTL					0x3E		//存储最近Y轴加速度传感器的测量值(低8位)
#define REGISTER_ACCEL_ZOUTH					0x3F		//存储最近Z轴加速度传感器的测量值(高8位)
#define REGISTER_ACCEL_ZOUTL					0x40		//存储最近Z轴加速度传感器的测量值(低8位)
#define REGISTER_TEMP_OUTH						0x41		//存储最近温度传感器的测量值(高8位)
#define REGISTER_TEMP_OUTL						0x42		//存储最近温度传感器的测量值(低8位)
#define	REGISTER_GYRO_XOUTH						0x43		//存储最近X轴陀螺仪的测量值(高8位)
#define REGISTER_GYRO_XOUTL						0x44		//存储最近X轴陀螺仪的测量值(低8位)
#define REGISTER_GYRO_YOUTH						0x45		//存储最近Y轴陀螺仪的测量值(高8位)
#define REGISTER_GYRO_YOUTL						0x46		//存储最近Y轴陀螺仪的测量值(低8位)
#define REGISTER_GYRO_ZOUTH						0x47		//存储最近Z轴陀螺仪的测量值(高8位)
#define REGISTER_GYRO_ZOUTL						0x48		//存储最近Z轴陀螺仪的测量值(低8位)
#define REGISTER_PWR_MGMT_1						0x6B		//电源管理1
#define REGISTER_PWR_MGMT_2						0x6C		//电源管理2
#define REGISTER_WHO_AM_I							0x75		//从机地址寄存器
#define REGISTER_XA_OFFSET_H					0x77
#define REGISTER_XA_OFFSET_L					0x78
#define REGISTER_YA_OFFSET_H					0x7A
#define REGISTER_YA_OFFSET_L					0x7B
#define REGISTER_ZA_OFFSET_H					0x7D
#define REGISTER_ZA_OFFSET_L					0x7E

/******************* AK8963寄存器地址 *******************/

#define REGISTER_DEVICE_ID						0x00
#define REGISTER_INFORMATION					0x01
#define REGISTER_STATUS_1							0x02
#define REGISTER_MAG_XOUT_L						0x03
#define REGISTER_MAG_XOUT_H						0x04
#define REGISTER_MAG_YOUT_L						0x05
#define REGISTER_MAG_YOUT_H						0x06
#define REGISTER_MAG_ZOUT_L						0x07
#define REGISTER_MAG_ZOUT_H						0x08
#define REGISTER_STATUS_2							0x09
#define REGISTER_CONTROL_1						0x0A
#define REGISTER_CONTROL_2						0x0B
#define REGISTER_SELF_TEST_CONTROL		0x0C
#define REGISTER_TEST_1								0x0D
#define REGISTER_TEST_2								0x0E
#define REGISTER_I2CDISABLE						0x0F

/********************** CONFIG **********************/
typedef enum															//陀螺仪滤波器设置
{ 
	DLPF_CFG_BANDWIDTH_260 = 0x00,
	DLPF_CFG_BANDWIDTH_184 = 0x01,
	DLPF_CFG_BANDWIDTH_94 = 0x02,
	DLPF_CFG_BANDWIDTH_44 = 0x03,
	DLPF_CFG_BANDWIDTH_21 = 0x04,
	DLPF_CFG_BANDWIDTH_10 = 0x05,
	DLPF_CFG_BANDWIDTH_5 = 0x06,
}DLPF_CFG_TypeDef;

/********************** ACCEL_CONFIG2 **********************/
typedef enum															//加速度计滤波器设置
{ 
	A_DLPF_CFG_BANDWIDTH_1046 = 0x00,
	A_DLPF_CFG_BANDWIDTH_218 = 0x01,
	A_DLPF_CFG_BANDWIDTH_99 = 0x02,
	A_DLPF_CFG_BANDWIDTH_44 = 0x03,
	A_DLPF_CFG_BANDWIDTH_21 = 0x04,
	A_DLPF_CFG_BANDWIDTH_10 = 0x05,
	A_DLPF_CFG_BANDWIDTH_5 = 0x06,
}A_DLPF_CFG_TypeDef;

/********************** GYRO_CONFIG **********************/

#define XG_ST 	0x80												//开启X轴陀螺仪自检
#define YG_ST 	0x40
#define ZG_ST 	0x20
#define XYZG_ST	0xE0

typedef enum															//陀螺仪量程
{ 
	FS_SEL_250 = 0x00,
	FS_SEL_500 = 0x08,
	FS_SEL_1000 = 0x10,
	FS_SEL_2000 = 0x18,
}FS_SEL_TypeDef;

/********************** ACCEL_CONFIG **********************/

#define XA_ST 	0x80												//开启X轴加速度传感器自检
#define YA_ST 	0x40
#define ZA_ST		0x20
#define XYZA_ST	0xE0

typedef enum															//加速度传感器量程
{ 
	AFS_SEL_2G = 0x00,
	AFS_SEL_4G = 0x08,
	AFS_SEL_8G = 0x10,
	AFS_SEL_16G = 0x18,
}AFS_SEL_TypeDef;


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

uint8_t MPU9250_Init(void);
uint8_t MPU9250_ReadData(MPU9250_DATA_TypeDef Data_Type, int16_t *data);
uint8_t AK8963_ReadData(AK8963_DATA_TypeDef Data_Type, int16_t *data);

#endif
