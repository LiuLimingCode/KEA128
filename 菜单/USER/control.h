#ifndef __CONTROL_H
#define __CONTROL_H

#include  "common.h"

typedef struct
{
  float P;
  float I;
  float D;
  float error[3];
  float Ad_value;
	float PID_Out;
}PID;

typedef enum
{
  L_1 = 0,
  L__,
  M_1,
  R__,
  R_1,
  ADC_NUM,
}ADC_POSITION;

typedef struct
{
  float Raw_Data[ADC_NUM];
  float Pro_Data[ADC_NUM];
	int16 Min_Data[ADC_NUM];
}ADC_DATA_Typedef;


#define	ADC_NOR(x, y)						((float)(y - g_ADC_Data.Min_Data[x])/((3088  - g_ADC_Data.Min_Data[x])/100.0))
#define	FLASH_SAVE_ADC_NORMAL 	(FLASH_SECTOR_NUM - 1)
#define	FLASH_SAVE_RUN_TIMES		(FLASH_SECTOR_NUM - 11)
#define	FLASH_SAVE_MCP41				(FLASH_SECTOR_NUM - 23)
#define	OLED_SHOW(x)						(int16)(25.5 * (x - 1))
#define	ERROR_MAX								100

#define WARN_READIIC_FAIL				0x01				//陀螺仪读数据失败,可能是线松了
#define WARN_HUANDAOENTER				0x02				//车模进环岛
#define WARN_HUANDAOOUT					0x04				//车模出环岛
#define WARN_BATTERY_LOW				0x08				//电池电量低
#define WARN_PODAOENTER					0x10
#define WARN_PODAOOUT						0x20

#define	PWM_MAX									1000
#define	PWM_MIN									-1000
#define	PWM_DEAD_R_Z						40
#define	PWM_DEAD_R_F						55
#define	PWM_DEAD_L_Z						50
#define	PWM_DEAD_L_F						45

//所有在 control.c使用的全局变量在这里声明
extern PID g_PID_Speed, g_PID_DIR;
extern ADC_DATA_Typedef g_ADC_Data;
extern int16 g_nSpeedExpect;
extern int16 g_nSpeedRealLeft;
extern int16 g_nSpeedRealRight;
extern int16 g_nSpeedReal;
extern int16 g_nSpeedLeft;
extern int16 g_nSpeedRight;
extern int16 g_nLeftPWM, g_nRighPWM;
extern int16 g_nBattery;
extern float chabihe;
extern float g_fDirectionError;
extern float g_fHuandaoTurn;
extern int16 g_nHuandaoTimer;
extern int16 g_nHuandaoAnger;
extern float g_fHuandaoOut;
extern int16 g_nPodaoFlag;
extern int16 g_nPodaoTimer;
extern int16 g_nPodaoThres;
extern int16 g_nPodaoSpeed;

void DataRead(void);
void SpeedControl(void);
void DirectionControl(void);
void PWMOut(void);
void UISend(void);
void JudgeCarStatus(void);

#endif
