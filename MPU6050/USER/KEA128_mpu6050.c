#include "KEA128_mpu6050.h"
#include "KEA128_myiic.h"
#if MPU6050_DMP_EN == 1
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include <math.h>
#endif

/************************************************************************/
//函数:MPU6050初始化函数
//返回:0 = 成功,其他 = 失败
/************************************************************************/
uint8_t MPU6050_Init(void)
{
  IIC_Init();
  if(IIC_WriteRegister(MPU6050_ADDR, REGISTER_PWR_MGMT_1, 0x01))return ERR_IIC_CHECKACK_FAIL;
  if(IIC_WriteRegister(MPU6050_ADDR, REGISTER_SMPLRT_DIV, 0x04))return ERR_IIC_CHECKACK_FAIL;   //200Hz
  if(IIC_WriteRegister(MPU6050_ADDR, REGISTER_CONFIG, DLPF_CFG_BANDWIDTH_94))return ERR_IIC_CHECKACK_FAIL;
  if(IIC_WriteRegister(MPU6050_ADDR, REGISTER_ACCEL_CONFIG, AFS_SEL_2G))return ERR_IIC_CHECKACK_FAIL;
  if(IIC_WriteRegister(MPU6050_ADDR, REGISTER_GYRO_CONFIG, FS_SEL_2000))return ERR_IIC_CHECKACK_FAIL;
  return NO_ERR;
}

/************************************************************************/
//函数:MPU6050读取数据
//参数:Data_Type 读取数据的类型，data 为读取数据存放的地址
//返回:0 = 成功,其他 = 失败
/************************************************************************/
uint8_t MPU6050_ReadData(DATA_TypeDef Data_Type, int16_t *data)
{
	uint8_t Register_Arr;
	uint8_t dat_h, dat_l;
	Register_Arr = REGISTER_ACCEL_XOUTH + Data_Type * 2;
	if(IIC_ReadRegister(MPU6050_ADDR, Register_Arr, &dat_h))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_ReadRegister(MPU6050_ADDR, Register_Arr + 1, &dat_l))return ERR_IIC_CHECKACK_FAIL;
	*data = (int16_t)((dat_h << 8) | dat_l);
	return NO_ERR;
}

#if MPU6050_DMP_EN == 1 //以下全为DMP函数

unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx);

static signed char gyro_orientation[9] = { 1, 0, 0,
                                              0, 1, 0,
                                              0, 0, 1};

/************************************************************************/
//函数:MPU6050初始化函数
//返回:0 = 成功,其他 = 失败
/************************************************************************/
uint8_t MPU6050_DMPInit(void)
{
	uint8_t err=0;
	IIC_Init();				//初始化IIC总线
	if(mpu_init()==0)	//初始化MPU6050
	{
		err = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);//设置需要的传感器
		if(err)return ERR_MPU6050INIT_FAIL; 
		err = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);//设置FIFO
		if(err)return ERR_MPU6050INIT_FAIL; 
		err = mpu_set_sample_rate(200);	//设置采样率200HZ
		if(err)return ERR_MPU6050INIT_FAIL; 
		err = dmp_load_motion_driver_firmware();		//加载DMP固件
		if(err)return ERR_DMPINIT_FAIL; 
		err = dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));//设置陀螺仪方向
		if(err)return ERR_MPU6050INIT_FAIL; 
		err = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_TAP|DMP_FEATURE_ANDROID_ORIENT|
														 DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO|DMP_FEATURE_GYRO_CAL);//设置DMP功能
		if(err)return ERR_DMPINIT_FAIL; 
		err = dmp_set_fifo_rate(100);	//设置DMP输出速率
		if(err)return ERR_DMPINIT_FAIL;   
		err = MPU6050_SelfTest();		//自检
		if(err)return ERR_SELTTEST_FAIL;    
		err = mpu_set_dmp_state(1);	//使能DMP
		if(err)return ERR_DMPINIT_FAIL;     
	}else return ERR_MPU6050INIT_FAIL;
	return NO_ERR;
}

/************************************************************************/
//函数:MPU6050自检函数
//返回:0 = 成功,其他 = 失败
/************************************************************************/
uint8_t MPU6050_SelfTest(void)
{
	int result;
	long gyro[3], accel[3]; 
	result = mpu_run_self_test(gyro, accel);
	if (result == 0x3) 
	{
		float sens;
		unsigned short accel_sens;
		mpu_get_gyro_sens(&sens);
		gyro[0] = (long)(gyro[0] * sens);
		gyro[1] = (long)(gyro[1] * sens);
		gyro[2] = (long)(gyro[2] * sens);
		dmp_set_gyro_bias(gyro);
		mpu_get_accel_sens(&accel_sens);
		accel_sens = 0;
		accel[0] *= accel_sens;
		accel[1] *= accel_sens;
		accel[2] *= accel_sens;
		dmp_set_accel_bias(accel);
		return NO_ERR;
	}else return ERR_SELTTEST_FAIL;
}


/************************************************************************/
//函数:MPU6050读取DMP处理之后的数据
//参数:pitch:俯仰角   精度:0.1°   范围: -90.0° <--->  +90.0°
//  	 roll :横滚角   精度:0.1°   范围:-180.0° <---> +180.0°
//		 yaw  :航向角   精度:0.1°   范围:-180.0° <---> +180.0°
//返回:0 = 成功,其他 = 失败
//备注:若读DMP时间太慢,会导致FIFO溢出,造成dmp_read_fifo()函数调用错误
//		 函数调用错误以后会自动调用mpu_reset_fifo();故解决办法为:1.定时
//		 器定时调用MPU6050_DMPGetData()2.在一次调用失败以后延时再一次调
//		 用
/************************************************************************/
uint8_t MPU6050_DMPGetData(float *pitch,float *roll,float *yaw)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	unsigned long sensor_timestamp;
	short gyro[3], accel[3], sensors;
	unsigned char more;
	long quat[4];
	if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more))return 1;
	if(sensors&INV_WXYZ_QUAT) 
	{
		q0 = quat[0] / q30;
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30; 
		*pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	// pitch
		*roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	// roll
		*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	}else return 2;
	return NO_ERR;
}

//方向转换
unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

//陀螺仪方向控制
unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar; 
    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */
    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;
    return scalar;
}

#endif