 /*!
 * @�ļ�       mpu9250.c
 * @����       MPU9250����ʵ��,MPU9250��һ���������̬������,�ܹ�����������ٶȡ����ٶ��Լ��ش�ǿ�ȡ�
 *             �ڲ������ٶȡ����ٶȷ��棬MPU9250��MPU6050�����빦�ܼ�����ȫ��ͬ���ڲ����ش�ǿ��ǿ��
 *             ���棬MPU9250�ĵشŴ���������Ƶ�ʵ͡����жȵ͡�����������������ߵĴ��������MPU9250
 *             �����MPU6050���������ơ��Ƽ�ʹ��FXOS8700��FXAS2100�ľ��ᴫ����
 * @����       ������
 * @���ʱ��   2019-12
 */
 
#include "KEA128_mpu9250.h"
#include "KEA128_myiic.h"
#include <math.h>

/************************************************************************/
//����:MPU9250��ʼ������
//����:0 = �ɹ�,���� = ʧ��
/************************************************************************/
uint8_t MPU9250_Init(void)
{
	IIC_Init(MPU9250_SDA_Pin, MPU9250_SCK_Pin);
	if(IIC_WriteData(MPU9250_SDA_Pin, MPU9250_SCK_Pin, MPU9250_ADDR, REGISTER_PWR_MGMT_1, 0x01))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_WriteData(MPU9250_SDA_Pin, MPU9250_SCK_Pin, MPU9250_ADDR, REGISTER_SMPLRT_DIV, 0x04))return ERR_IIC_CHECKACK_FAIL;   //200Hz
	if(IIC_WriteData(MPU9250_SDA_Pin, MPU9250_SCK_Pin, MPU9250_ADDR, REGISTER_CONFIG, DLPF_CFG_BANDWIDTH_94))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_WriteData(MPU9250_SDA_Pin, MPU9250_SCK_Pin, MPU9250_ADDR, REGISTER_ACCEL_CONFIG, AFS_SEL_2G))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_WriteData(MPU9250_SDA_Pin, MPU9250_SCK_Pin, MPU9250_ADDR, REGISTER_ACCEL_CONFIG2, A_DLPF_CFG_BANDWIDTH_99))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_WriteData(MPU9250_SDA_Pin, MPU9250_SCK_Pin, MPU9250_ADDR, REGISTER_GYRO_CONFIG, FS_SEL_2000))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_WriteData(MPU9250_SDA_Pin, MPU9250_SCK_Pin, MPU9250_ADDR, REGISTER_PIN_CFG, 0x02))return ERR_IIC_CHECKACK_FAIL;//turn on Bypass Mode 
	if(IIC_WriteData(MPU9250_SDA_Pin, MPU9250_SCK_Pin, AK8963_ADDR, REGISTER_CONTROL_2, 0x01))return ERR_IIC_CHECKACK_FAIL;
	systick_delay_ms(1);
	if(IIC_WriteData(MPU9250_SDA_Pin, MPU9250_SCK_Pin, AK8963_ADDR, REGISTER_CONTROL_1, 0x06))return ERR_IIC_CHECKACK_FAIL;
	return NO_ERR;
}

/************************************************************************/
//����:MPU9250��ȡ����
//����:MPU9250_DATA_TypeDef ��ȡ���ݵ����ͣ�data Ϊ��ȡ���ݴ�ŵĵ�ַ
//����:0 = �ɹ�,���� = ʧ��
/************************************************************************/
uint8_t MPU9250_ReadData(MPU9250_DATA_TypeDef Data_Type, int16_t *data)
{
	uint8_t Register_Arr;
	uint8_t dat_h, dat_l;
	Register_Arr = REGISTER_ACCEL_XOUTH + Data_Type * 2;
	if(IIC_ReadData(MPU9250_SDA_Pin, MPU9250_SCK_Pin, MPU9250_ADDR, Register_Arr, &dat_h))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_ReadData(MPU9250_SDA_Pin, MPU9250_SCK_Pin, MPU9250_ADDR, Register_Arr + 1, &dat_l))return ERR_IIC_CHECKACK_FAIL;
	*data = (int16_t)((dat_h << 8) | dat_l);
	return NO_ERR;
}

/************************************************************************/
//����:AK8963��ȡ����
//����:AK8963_DATA_TypeDef ��ȡ���ݵ����ͣ�data Ϊ��ȡ���ݴ�ŵĵ�ַ
//����:0 = �ɹ�,���� = ʧ��
/************************************************************************/
uint8_t AK8963_ReadData(AK8963_DATA_TypeDef Data_Type, int16_t *data)
{
	uint8 temp;
	uint8_t Register_Arr;
	uint8_t dat_h, dat_l;
	
	Register_Arr = REGISTER_MAG_XOUT_L + Data_Type * 2;
	if(IIC_ReadData(MPU9250_SDA_Pin, MPU9250_SCK_Pin, AK8963_ADDR, Register_Arr, &dat_l))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_ReadData(MPU9250_SDA_Pin, MPU9250_SCK_Pin, AK8963_ADDR, Register_Arr + 1, &dat_h))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_ReadData(MPU9250_SDA_Pin, MPU9250_SCK_Pin, AK8963_ADDR, REGISTER_STATUS_2, &temp))return ERR_IIC_CHECKACK_FAIL;
	*data = (int16_t)((dat_h << 8) | dat_l);

	return NO_ERR;
}
