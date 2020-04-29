#include "KEA128_itg3205.h"
#include "KEA128_myiic.h"
#include <math.h>

/************************************************************************/
//函数:ITG3205初始化函数
//返回:0 = 成功,其他 = 失败
/************************************************************************/
uint8_t ITG3205_Init(void)
{
  IIC_Init();
  if(IIC_WriteRegister(ITG3205_ADDR, REGISTER_PWR_MGM, 0x01))return ERR_IIC_CHECKACK_FAIL;
  if(IIC_WriteRegister(ITG3205_ADDR, REGISTER_SMPLRT_DIV, 0x04))return ERR_IIC_CHECKACK_FAIL;   //200Hz
	if(IIC_WriteRegister(ITG3205_ADDR, REGISTER_DLPF_FS, FS_SEL_2000 | DLPF_CFG_BANDWIDTH_98))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_WriteRegister(ITG3205_ADDR, REGISTER_INT_CFG, 0x00))return ERR_IIC_CHECKACK_FAIL;
  return NO_ERR;
}

/************************************************************************/
//函数:ITG3205读取数据
//参数:ITG3205_DATA_TypeDef 读取数据的类型，data 为读取数据存放的地址
//返回:0 = 成功,其他 = 失败
/************************************************************************/
uint8_t ITG3205_ReadData(DATA_TypeDef Data_Type, int16_t *data)
{
	uint8_t Register_Arr;
	uint8_t dat_h, dat_l;
	Register_Arr = REGISTER_TEMP_OUTH + Data_Type * 2;
	if(IIC_ReadRegister(ITG3205_ADDR, Register_Arr, &dat_h))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_ReadRegister(ITG3205_ADDR, Register_Arr + 1, &dat_l))return ERR_IIC_CHECKACK_FAIL;
	*data = (int16_t)((dat_h << 8) | dat_l);
	return NO_ERR;
}


