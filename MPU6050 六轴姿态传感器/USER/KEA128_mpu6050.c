 /*!
 * @�ļ�       mpu6050.c
 * @����       MPU6050����ʵ��,MPU6050��һ��������̬������,����ٶȴ���������Ư��,����������ȱ��.
 *             ����ٶȴ����������ϴ�,���ݱ���ͨ������󷽿�ʹ��.оƬ�����ŵ����Դ�DMP��Ԫ,����
 *             ������Ԫ��,Ϊ��Ƭ�������˺ܴ�ĸ���,����Ƭ����оƬDMP��ͨ�ű������ƴ���.
 * @����       ������
 * @���ʱ��   2019-12
 */
 
#include "KEA128_mpu6050.h"
#include "KEA128_myiic.h"
#if MPU6050_DMP_EN == 1
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include <math.h>
#endif

#if MPU6050_DMP_EN == 0
//----------------------------------------------------------------------------
//  @����       MPU6050��ʼ������
//  @����       0 = �ɹ�;1 = ʧ��
//  @ʾ��       if(MPU6050_Init()) { ... }
//  @ע��       Ϊ�˲�������ʼ������,ֻ���ڲ�����DMP����ʱ,���ܵ��øú���
//----------------------------------------------------------------------------
uint8_t MPU6050_Init(void)
{
  IIC_Init(MPU6050_SDA_Pin, MPU6050_SCK_Pin);
  if(IIC_WriteData(MPU6050_SDA_Pin, MPU6050_SCK_Pin, MPU6050_ADDR, MPU6050_REGISTER_PWR_MGMT_1, 0x01))return ERR_IIC_CHECKACK_FAIL;
  if(IIC_WriteData(MPU6050_SDA_Pin, MPU6050_SCK_Pin, MPU6050_ADDR, MPU6050_REGISTER_SMPLRT_DIV, (1000 / MPU6050_OUTPUT_RATE - 1)))return ERR_IIC_CHECKACK_FAIL;   //200Hz
  if(IIC_WriteData(MPU6050_SDA_Pin, MPU6050_SCK_Pin, MPU6050_ADDR, MPU6050_REGISTER_CONFIG, DLPF_CFG_BANDWIDTH_94))return ERR_IIC_CHECKACK_FAIL;
  if(IIC_WriteData(MPU6050_SDA_Pin, MPU6050_SCK_Pin, MPU6050_ADDR, MPU6050_REGISTER_ACCEL_CONFIG, MPU6050_AFS_SEL))return ERR_IIC_CHECKACK_FAIL;
  if(IIC_WriteData(MPU6050_SDA_Pin, MPU6050_SCK_Pin, MPU6050_ADDR, MPU6050_REGISTER_GYRO_CONFIG, MPU6050_FS_SEL))return ERR_IIC_CHECKACK_FAIL;
  return NO_ERR;
}
#endif

//----------------------------------------------------------------------------
//  @����       MPU6050��ȡ����
//  @����       Data_Type��ȡ���ݵ�����,��MPU6050_DATA_TypeDef
//  @����       data Ϊ��ȡ���ݴ�ŵĵ�ַ
//  @����       0 = �ɹ�,���� = ʧ��
//  @ʾ��       if(MPU6050_ReadData(MPU6050_GYRO_X, &gyroXData)) { ... }
//----------------------------------------------------------------------------
uint8_t MPU6050_ReadData(MPU6050_DATA_TypeDef Data_Type, int16_t *data)
{
	uint8_t Register_Arr;
	uint8_t dat_h, dat_l;
	Register_Arr = MPU6050_REGISTER_ACCEL_XOUTH + Data_Type * 2;
	if(IIC_ReadData(MPU6050_SDA_Pin, MPU6050_SCK_Pin, MPU6050_ADDR, Register_Arr, &dat_h))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_ReadData(MPU6050_SDA_Pin, MPU6050_SCK_Pin, MPU6050_ADDR, Register_Arr + 1, &dat_l))return ERR_IIC_CHECKACK_FAIL;
	*data = (int16_t)((dat_h << 8) | dat_l);
	return NO_ERR;
}

#if MPU6050_DMP_EN == 1 //����ȫΪDMP����

static unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx);
static unsigned short inv_row_2_scale(const signed char *row);

static signed char gyro_orientation[9] = { 1, 0, 0,
                                           0, 1, 0,
                                           0, 0, 1};

//----------------------------------------------------------------------------
//  @����       MPU6050��ʼ������
//  @����       0 = �ɹ�,���� = ʧ��
//  @ʾ��       while(MPU6050_DMPInit());
//  @ע��       DMPĬ����������������Ϊ��2000,���ٶȼ�Ϊ��2G
//----------------------------------------------------------------------------
uint8_t MPU6050_DMPInit(void)
{
	uint8_t err=0;
	IIC_Init(MPU6050_SDA_Pin, MPU6050_SCK_Pin); //��ʼ��IIC����
	if(mpu_init()==0)	//��ʼ��MPU6050
	{
		err = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL); //������Ҫ�Ĵ�����
		if(err)return ERR_MPU6050INIT_FAIL; 
		err = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL); //����FIFO
		if(err)return ERR_MPU6050INIT_FAIL; 
		err = mpu_set_sample_rate(MPU6050_OUTPUT_RATE);	//���ò�����200HZ
		if(err)return ERR_MPU6050INIT_FAIL; 
		err = dmp_load_motion_driver_firmware();		//����DMP�̼�
		if(err)return ERR_DMPINIT_FAIL; 
		err = dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)); //���������Ƿ���
		if(err)return ERR_MPU6050INIT_FAIL; 
		err = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP | DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL); //����DMP����
		if(err)return ERR_DMPINIT_FAIL; 
		err = dmp_set_fifo_rate(MPU6050_DMP_RATE); //����DMP�������
		if(err)return ERR_DMPINIT_FAIL;   
		//err = MPU6050_SelfTest(); //�Լ�,�������η���ÿ�γ�ʼ���ƶ������ǻᵼ���Լ�ʧ��
		//if(err)return ERR_SELTTEST_FAIL;    
		err = mpu_set_dmp_state(1);	//ʹ��DMP
		if(err)return ERR_DMPINIT_FAIL;     
	}else return ERR_MPU6050INIT_FAIL;
	return NO_ERR;
}

//----------------------------------------------------------------------------
//  @����       MPU6050�Լ캯��
//  @����       0 = �ɹ�,���� = ʧ��
//  @ʾ��       if(MPU6050_SelfTest()) { ... }
//  @ע��       �ڵ����Լ캯��ʱ,���뱣֤�����Ǿ�ֹ�Ż᷵�سɹ�,
//              ����ֻ�ڻ��ɴ�����������ʱ�ŵ��øú���
//----------------------------------------------------------------------------
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


//----------------------------------------------------------------------------
//  @����       MPU6050��ȡDMP����֮�������
//  @����       pitch:�����Ǵ�ŵ�ַ   ����:0.1��   ��Χ: -90.0�� <--->  +90.0��
//  @����       roll :����Ǵ�ŵ�ַ   ����:0.1��   ��Χ:-180.0�� <---> +180.0��
//  @����       yaw  :����Ǵ�ŵ�ַ   ����:0.1��   ��Χ:-180.0�� <---> +180.0��
//  @����       0 = �ɹ�,���� = ʧ��
//  @ʾ��       if(MPU6050_DMPGetData(&pitch, &roll, &yaw)) { ... }
//  @ע��       ����DMPʱ��̫����̫��,�ᵼ��FIFO���,���dmp_read_fifo()��������
//              ���������ô����Ժ���Զ�����mpu_reset_fifo(),�ʽ���취Ϊ:
//              1.��ʱ����ʱ����MPU6050_DMPGetData(),���ҵ���Ƶ�ʺ�MPU6050_DMP_RATEһ��
//              2.��һ�ε���ʧ���Ժ���ʱ��һ�ε���while(MPU6050_DMPGetData())
//----------------------------------------------------------------------------
uint8_t MPU6050_DMPGetData(float *pitch, float *roll, float *yaw)
{
	float q0 = 1.0f,q1 = 0.0f,q2 = 0.0f,q3 = 0.0f;
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
		*pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
		*roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
		*yaw   = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3; //yaw
	}else return 2;
	return NO_ERR;
}

//����ת��
static unsigned short inv_row_2_scale(const signed char *row)
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

//�����Ƿ������
static unsigned short inv_orientation_matrix_to_scalar( const signed char *mtx)
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
