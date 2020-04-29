#include "KEA128_mpu9250.h"
#include "KEA128_myiic.h"
#include <math.h>

/************************************************************************/
//函数:MPU9250初始化函数
//返回:0 = 成功,其他 = 失败
/************************************************************************/
uint8_t MPU9250_Init(void)
{
  IIC_Init();
  if(IIC_WriteRegister(MPU9250_ADDR, REGISTER_PWR_MGMT_1, 0x01))return ERR_IIC_CHECKACK_FAIL;
  if(IIC_WriteRegister(MPU9250_ADDR, REGISTER_SMPLRT_DIV, 0x04))return ERR_IIC_CHECKACK_FAIL;   //200Hz
  if(IIC_WriteRegister(MPU9250_ADDR, REGISTER_CONFIG, DLPF_CFG_BANDWIDTH_94))return ERR_IIC_CHECKACK_FAIL;
  if(IIC_WriteRegister(MPU9250_ADDR, REGISTER_ACCEL_CONFIG, AFS_SEL_2G))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_WriteRegister(MPU9250_ADDR, REGISTER_ACCEL_CONFIG2, A_DLPF_CFG_BANDWIDTH_99))return ERR_IIC_CHECKACK_FAIL;
  if(IIC_WriteRegister(MPU9250_ADDR, REGISTER_GYRO_CONFIG, FS_SEL_2000))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_WriteRegister(MPU9250_ADDR, REGISTER_PIN_CFG, 0x02))return ERR_IIC_CHECKACK_FAIL;//turn on Bypass Mode 
	if(IIC_WriteRegister(AK8963_ADDR, REGISTER_CONTROL_2, 0x01))return ERR_IIC_CHECKACK_FAIL;
	systick_delay_ms(1);
	if(IIC_WriteRegister(AK8963_ADDR, REGISTER_CONTROL_1, 0x06))return ERR_IIC_CHECKACK_FAIL;
  return NO_ERR;
}

/************************************************************************/
//函数:MPU9250读取数据
//参数:MPU9250_DATA_TypeDef 读取数据的类型，data 为读取数据存放的地址
//返回:0 = 成功,其他 = 失败
/************************************************************************/
uint8_t MPU9250_ReadData(MPU9250_DATA_TypeDef Data_Type, int16_t *data)
{
	uint8_t Register_Arr;
	uint8_t dat_h, dat_l;
	Register_Arr = REGISTER_ACCEL_XOUTH + Data_Type * 2;
	if(IIC_ReadRegister(MPU9250_ADDR, Register_Arr, &dat_h))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_ReadRegister(MPU9250_ADDR, Register_Arr + 1, &dat_l))return ERR_IIC_CHECKACK_FAIL;
	*data = (int16_t)((dat_h << 8) | dat_l);
	return NO_ERR;
}

/************************************************************************/
//函数:AK8963读取数据
//参数:AK8963_DATA_TypeDef 读取数据的类型，data 为读取数据存放的地址
//返回:0 = 成功,其他 = 失败
/************************************************************************/
uint8_t AK8963_ReadData(AK8963_DATA_TypeDef Data_Type, int16_t *data)
{
	uint8 temp;
	uint8_t Register_Arr;
	uint8_t dat_h, dat_l;
	
	Register_Arr = REGISTER_MAG_XOUT_L + Data_Type * 2;
	if(IIC_ReadRegister(AK8963_ADDR, Register_Arr, &dat_l))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_ReadRegister(AK8963_ADDR, Register_Arr + 1, &dat_h))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_ReadRegister(AK8963_ADDR, REGISTER_STATUS_2, &temp))return ERR_IIC_CHECKACK_FAIL;
	*data = (int16_t)((dat_h << 8) | dat_l);

	return NO_ERR;
}
