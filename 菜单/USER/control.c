#include "headfile.h"

PID g_PID_Speed={38.5, //38.5
                3.20,//3.2
                0.0,
                0,0,0,
                0,0},
    g_PID_DIR=  {2.6,
                0.0,
                18.70,
                0,0,0,
                0,0};

ADC_DATA_Typedef g_ADC_Data = {0};

float g_fGyroZ;												//Z轴角速度
float g_fGyroX;
int16 g_nMagnetX;
int16 g_nMagnetY;
float g_fangerX = 0;
float g_fangerZ = 0;

int16 g_nSpeedExpect = 85;						//期望速度
int16 g_nSpeedRealLeft;								//左轮数据处理后的速度
int16 g_nSpeedRealRight;							//右轮数据处理后的速度
int16 g_nSpeedLeft;										//左轮编码器读取速度
int16 g_nSpeedRight;									//右轮编码器读取速度
int16 g_nSpeedReal;										//数据处理后的车模速度
float g_fSpeedControlOut;							//速度环输出值

float g_fDirectionError;							//方向环计算误差值
float g_fDirectionControlOut;					//方向环输出值

float g_fHuandaoTurn = 1.50;
int16 g_nHuandaoTimer = 60;
int16 g_nHuandaoAnger = 320; 
float g_fHuandaoOut = 0.2;

int16 g_nPodaoFlag = 0;
int16 g_nPodaoTimer = 150;
int16 g_nPodaoThres = 250;
int16 g_nPodaoSpeed = 300;

int16 g_nLeftPWM, g_nRighPWM;					//左右轮PWM输出值
int16 g_nBattery;											//电池电量

float chabihe = 0;
int16 ILimt=9;
float fSpeedError = 0;
float UI_data[8];

/**
 * @备注    数据读取
 * @输出		g_fGyroZCo 								Z轴角速度
						g_ADC_Data.Pro_Data				电感归一化数据
 */ 
void DataRead(void)
{
	int16 GyroZ, GyroX;
	static int16 g_nMagnetYOld = 0;
	if(FXAS2100_ReadData(FXAS2100_DATA_Z_GYRO, &GyroZ)) Flag_Warning |= WARN_READIIC_FAIL;
	g_fGyroZ = GyroZ / 16.0;
	if(FXAS2100_ReadData(FXAS2100_DATA_X_GYRO, &GyroX)) Flag_Warning |= WARN_READIIC_FAIL;
	g_fGyroX = GyroX / 16.0;
	if(FXOS8700_ReadData(FXOS8700_DATA_Y_MAGNET, &g_nMagnetY)) Flag_Warning |= WARN_READIIC_FAIL;
	if(FXOS8700_ReadData(FXOS8700_DATA_X_MAGNET, &g_nMagnetX)) Flag_Warning |= WARN_READIIC_FAIL;
	
	if(Flag_time > 500 && g_nMagnetYOld - g_nMagnetY > 900 && Flag_Magnet > 0)
		Flag_Stop = ON;
	g_nMagnetYOld = g_nMagnetY;
	
	g_nBattery = adc_read(AD_CH_BAT, 1) / 4095.0 * 3.79 * 3.3 * 100;
	if(g_nBattery < 740 && g_nBattery > 500 && Flag_Run == OFF) Flag_Warning |= WARN_BATTERY_LOW;
	
	uint16 ad_temp[5][5]={0};
	
	for(int8 i=0;i<5;i++)
	{
		ad_temp[0][i]=adc_read(AD_CH_L_1, 4);
		ad_temp[1][i]=adc_read(AD_CH_L__, 4);
		ad_temp[2][i]=adc_read(AD_CH_M_1, 4);
		ad_temp[3][i]=adc_read(AD_CH_R__, 4);
		ad_temp[4][i]=adc_read(AD_CH_R_1, 4);
	}
	for(int8 i=0;i<ADC_NUM;i++)//冒泡排序升序
	{
		for(int8 j=0;j<4;j++)
		{
			for(int8 k=0;k<4-j;k++)
			{
				if(ad_temp[i][k] > ad_temp[i][k+1])        //前面的比后面的大  则进行交换
				{
					uint16 temp;
					temp = ad_temp[i][k+1];
					ad_temp[i][k+1] = ad_temp[i][k];
					ad_temp[i][k] = temp;
				}
			}
		}
	}
	for(int8 i=0;i<ADC_NUM;i++)    //求中间三项的和
	{
		g_ADC_Data.Raw_Data[i] = (ad_temp[i][1] + ad_temp[i][2] + ad_temp[i][3]) / 3.0;
		g_ADC_Data.Pro_Data[i] = 100 - ADC_NOR(i, g_ADC_Data.Raw_Data[i]);
		if(g_ADC_Data.Pro_Data[i] < 1) g_ADC_Data.Pro_Data[i] = 1;
	}
	
	if(Flag_Podao == 0 && g_fGyroX < -g_nPodaoThres && fabs(g_ADC_Data.Pro_Data[R__] - g_ADC_Data.Pro_Data[L__]) < 15)
	{
		Flag_Podao = 1;
		Flag_Warning |= WARN_PODAOENTER;
		Flag_PodaoTimer = Flag_time;
	}
	if( Flag_Podao == 1 && Flag_time > (Flag_PodaoTimer + g_nPodaoTimer))
	{
		Flag_Podao = 3;
		Flag_Warning &= ~WARN_PODAOENTER;
	}
}

/**
 * @备注    车模速度读取,处理
 * @输出		g_nSpeedRealLeft					左轮数据处理后的速度
						g_nSpeedRealRight					右轮数据处理后的速度
						g_nSpeedLeft							左轮编码器读取速度
						g_nSpeedRight							右轮编码器读取速度
						g_nSpeedReal							数据处理后的车模速度
 */
void SpeedProcess(uint8 times)
{
	g_nSpeedRealLeft = g_nSpeedLeft;
	g_nSpeedRealRight = g_nSpeedRight;
	g_nSpeedReal = (g_nSpeedRealLeft + g_nSpeedRealRight) * 0.5;
}

/**
 * @备注    速度环PID
 * @输出		g_fSpeedControlOut				速度环输出值
 */
void SpeedPID(uint8 times, float speed_ask)
{
	static float  fSpeedPIDOutOld = 0;
	float fSpeedError = 0;
	int index = 1;
	
  if(times == SPEED_PERIOD)
  {
		fSpeedError = speed_ask - g_nSpeedReal;
				
		fSpeedError = (fSpeedError > 20 ? 20 : fSpeedError);
		
		index = ((fabs(fSpeedError) > ILimt) ? 0 : 1);
		
		fSpeedPIDOutOld = g_PID_Speed.PID_Out;
		
		g_PID_Speed.error[0] = fSpeedError;
		
		if(Flag_time < 200 || Flag_Stop == ON)
			g_PID_Speed.PID_Out += (2+g_PID_Speed.P) * (g_PID_Speed.error[0] - g_PID_Speed.error[1]) + 
													 (g_PID_Speed.I+2) * g_PID_Speed.error[0];
		else
			g_PID_Speed.PID_Out += g_PID_Speed.P * (g_PID_Speed.error[0] - g_PID_Speed.error[1]) + 
												 g_PID_Speed.I * g_PID_Speed.error[0] * index;
		
		g_PID_Speed.error[1] = g_PID_Speed.error[0];
		
		if(g_nPodaoFlag > 0 && Flag_Podao == 1)
		{
			g_PID_Speed.PID_Out = g_nPodaoSpeed;
		}
		
		if(g_PID_Speed.PID_Out >= 1000) g_PID_Speed.PID_Out = 1000 - 0.00001;
		if(g_PID_Speed.PID_Out <= -1000) g_PID_Speed.PID_Out = -1000 + 0.00001;
  }
  g_fSpeedControlOut += (g_PID_Speed.PID_Out - fSpeedPIDOutOld) / (float)SPEED_PERIOD;
	
}


/**
 * @备注    速度环控制函数
 */
void SpeedControl(void)
{
	static uint8 nControltimes;
	nControltimes ++;
	
	SpeedProcess(nControltimes);
	
	if(Flag_Run == ON && Flag_Stop == OFF) SpeedPID(nControltimes, g_nSpeedExpect);
	else if(Flag_Stop == ON) SpeedPID(nControltimes, 0);
	
	if(nControltimes == SPEED_PERIOD) nControltimes = 0;
		
}

/**
 * @备注    车模偏离赛道偏离误差
 * @输出		g_fDirectionError					方向环误差值
 */
void ADCProcess(void)
{
	static float CHA = 0;
	static int ADC_Prodata_M_1[3] = {0};
	static int8 Huandao_Rise = 0;
	static float g_fDirectionErrorOld = 0;
	static int Huandao_Timer = 0;
	float fHuandaoErrorOut = 0;

	if((g_ADC_Data.Pro_Data[R__] > 20 && g_ADC_Data.Pro_Data[L__] > 20) || fabs(g_ADC_Data.Pro_Data[R__] - g_ADC_Data.Pro_Data[L__]) > fabs(CHA))
		CHA = g_ADC_Data.Pro_Data[R__] - g_ADC_Data.Pro_Data[L__];
	

	chabihe = 100 * (float)(CHA) / (float)(g_ADC_Data.Pro_Data[L__] + g_ADC_Data.Pro_Data[R__]);
	
	if(Flag_Huandao == 0 && (g_ADC_Data.Pro_Data[R__]> 120 || g_ADC_Data.Pro_Data[L__] > 120) && g_ADC_Data.Pro_Data[M_1]> 30 && (g_ADC_Data.Pro_Data[L_1]> 25 || g_ADC_Data.Pro_Data[R_1]> 25) && Flag_Podao != 1)//准备进环岛
	{
		Flag_Huandao = 1;
	}
	
	if(Flag_Huandao == 1 || Flag_Huandao == 3 || Flag_Huandao == 5)
		g_fangerZ += g_fGyroZ * 0.9765 * 0.005;
	
	if(Flag_Huandao == 1)
	{
		if(Huandao_Rise == 0 && (((int)(g_ADC_Data.Pro_Data[M_1] * 2) < ADC_Prodata_M_1[0] && ADC_Prodata_M_1[0] < ADC_Prodata_M_1[1] && ADC_Prodata_M_1[1] < ADC_Prodata_M_1[2])))
		{
			Huandao_Rise = 3;
			Flag_Huandao_DIR = (g_ADC_Data.Pro_Data[R__] > g_ADC_Data.Pro_Data[L__] ? -1 : 1);
		}
		if(Huandao_Rise == 3 && (int)(g_ADC_Data.Pro_Data[M_1] * 2) >= ADC_Prodata_M_1[0])
		{
			Huandao_Rise = 5;
			
		}
		if(Huandao_Rise == 5 && (g_ADC_Data.Pro_Data[R_1] - g_ADC_Data.Pro_Data[L_1]) * Flag_Huandao_DIR > 0)
		{
			Huandao_Rise = 0;
			Flag_Huandao = 3;
			ADC_Prodata_M_1[0] = 0, ADC_Prodata_M_1[1] = 0, ADC_Prodata_M_1[2] = 0;
		}
		
		ADC_Prodata_M_1[2] = ADC_Prodata_M_1[1], ADC_Prodata_M_1[1] = ADC_Prodata_M_1[0], ADC_Prodata_M_1[0] = (int)(g_ADC_Data.Pro_Data[M_1] * 2);
	}
	if(Flag_Huandao == 3)
	{
		fHuandaoErrorOut = g_fHuandaoTurn * ((g_ADC_Data.Pro_Data[R__] - g_ADC_Data.Pro_Data[L__]) * 1 + (g_ADC_Data.Pro_Data[R_1] - g_ADC_Data.Pro_Data[L_1]) * 3) / 4.0;
		
		
		if(g_ADC_Data.Pro_Data[L__] + g_ADC_Data.Pro_Data[R__] < 150)
		{
			if(chabihe * Flag_Huandao_DIR > 0 && fabs(chabihe) > 0.5 * fabs(fHuandaoErrorOut))
			{
				Flag_Huandao = 5;
			}
		}
		if(fabs(g_fangerZ) > 60)
			Flag_Huandao = 5;
		
		if(fHuandaoErrorOut * Flag_Huandao_DIR < 0) fHuandaoErrorOut = 0;
		if(fHuandaoErrorOut > 55) fHuandaoErrorOut = 55;
		if(fHuandaoErrorOut < -55) fHuandaoErrorOut = -55;
	}
	
	if(Flag_Huandao == 5 && fabs(g_fangerZ) > g_nHuandaoAnger)
	{
		Flag_Huandao = 7;
		Huandao_Timer = g_nHuandaoTimer;
	}
	if(Flag_Huandao == 7 && Huandao_Timer -- < 0)
	{
		Flag_Huandao = 9;
		Huandao_Rise = 0;
		ADC_Prodata_M_1[0] = 0, ADC_Prodata_M_1[1] = 0, ADC_Prodata_M_1[2] = 0;
		g_fangerZ = 0;
	}
	
	if(Flag_Huandao == 9 && g_ADC_Data.Pro_Data[M_1] < 30 && g_ADC_Data.Pro_Data[L__] < 100 && g_ADC_Data.Pro_Data[R__] < 100)
		Flag_Huandao = 0;
	
	if(g_ADC_Data.Pro_Data[L__] <= 5 || g_ADC_Data.Pro_Data[R__] <= 5)//丢线
	{
		if(g_fDirectionErrorOld < 0)
			g_fDirectionError = -ERROR_MAX;
		else if(g_fDirectionErrorOld > 0)
			g_fDirectionError = ERROR_MAX;
		else
		{
			if(g_ADC_Data.Pro_Data[L__] < g_ADC_Data.Pro_Data[R__])
				g_fDirectionError = ERROR_MAX;
			else if(g_ADC_Data.Pro_Data[L__] > g_ADC_Data.Pro_Data[R__])
				g_fDirectionError = -ERROR_MAX;
		}
	}
	
	if(Flag_Huandao == 3)
	{
		g_fDirectionError = fHuandaoErrorOut;
		Flag_Warning |= WARN_HUANDAOENTER;
	}
	else if(Flag_Huandao == 7)
	{
		Flag_Warning |= WARN_HUANDAOOUT;
		g_fDirectionError = chabihe;
		if(g_fDirectionError * Flag_Huandao_DIR > 0) g_fDirectionError *= g_fHuandaoOut;
	}
	else
	{
		g_fDirectionError = chabihe;
		Flag_Warning &= ~(WARN_HUANDAOENTER | WARN_HUANDAOOUT);
	}
	
	if(g_fDirectionError > ERROR_MAX) g_fDirectionError = ERROR_MAX;
	if(g_fDirectionError < -ERROR_MAX) g_fDirectionError = -ERROR_MAX;
	
	g_fDirectionErrorOld = g_fDirectionError;
}

/**
 * @备注    速度环PID
 * @输出		g_fDirectionControlOut		方向环PID输出
 */
void DirectionPID(float speed)
{
	float Direction_error_diff;
	static uint8 Huandao_Flag_Last = 0;
	
	g_PID_DIR.error[0] = 0 - g_fDirectionError;
	
	if((Flag_Huandao == 1 || Flag_Huandao == 3 ) && Huandao_Flag_Last != Flag_Huandao)
		Direction_error_diff = 0;
	else
		Direction_error_diff = g_PID_DIR.error[0] - g_PID_DIR.error[1];
	Huandao_Flag_Last = Flag_Huandao;
	
	Direction_error_diff = ((Direction_error_diff >= 4) ? (4): Direction_error_diff);
	Direction_error_diff = ((Direction_error_diff <= -4) ? (-4): Direction_error_diff);
	
	g_PID_DIR.PID_Out = (g_PID_DIR.error[0] * g_PID_DIR.P +
											Direction_error_diff * g_PID_DIR.D) * 10;
	g_PID_DIR.error[1] = g_PID_DIR.error[0];		//error[1]上一次误差
	
	if(g_PID_DIR.PID_Out >= 1200) g_PID_DIR.PID_Out = 1200 - 0.00001;
	if(g_PID_DIR.PID_Out <= -1200) g_PID_DIR.PID_Out = -1200 + 0.00001;
	
	g_fDirectionControlOut = g_PID_DIR.PID_Out;
}

/**
 * @备注    方向环控制
 */
void DirectionControl(void)
{
	ADCProcess();
	
	if(Flag_Run == ON && Flag_Stop == OFF) DirectionPID(g_nSpeedExpect);
}

/**
 * @备注    PWM输出函数
 * @输出    g_nLeftPWM								左轮输出PWM值
						g_nRightPWM								右轮输出PWM值
 */
void PWMOut(void)
{
	g_nLeftPWM = (int16)(g_fSpeedControlOut - g_fDirectionControlOut);
	g_nRighPWM = (int16)(g_fSpeedControlOut + g_fDirectionControlOut); 
	if(Flag_Stop == ON) g_nLeftPWM = g_nRighPWM = (int16)(g_fSpeedControlOut);
	
	if(Flag_Motor == OFF) g_nLeftPWM = g_nRighPWM = 0;
	
	if(Flag_Run == ON)
	{
		if(g_nRighPWM >= 0)
		{
			if(g_nRighPWM + PWM_DEAD_R_Z >= PWM_MAX) g_nRighPWM = PWM_MAX - PWM_DEAD_R_Z;
			if(Flag_Stop == ON) g_nRighPWM -= PWM_DEAD_R_Z;
			ftm_pwm_duty(ftm2, PWM_CH_R_F, 0);
			ftm_pwm_duty(ftm2, PWM_CH_R_Z, g_nRighPWM + PWM_DEAD_R_Z);
		}
		else if(g_nRighPWM < 0)
		{
			if(g_nRighPWM - PWM_DEAD_R_F <= PWM_MIN) g_nRighPWM = PWM_MIN + PWM_DEAD_R_F;
			if(Flag_Stop == ON) g_nRighPWM += PWM_DEAD_R_Z;
			ftm_pwm_duty(ftm2, PWM_CH_R_F, - g_nRighPWM + PWM_DEAD_R_F);
			ftm_pwm_duty(ftm2, PWM_CH_R_Z, 0);
		}
		
		if(g_nLeftPWM >= 0)
		{
			if(g_nLeftPWM + PWM_DEAD_L_Z >= PWM_MAX) g_nLeftPWM = PWM_MAX - PWM_DEAD_L_Z;
			if(Flag_Stop == ON) g_nRighPWM -= PWM_DEAD_L_Z;
			ftm_pwm_duty(ftm2, PWM_CH_L_F, 0);
			ftm_pwm_duty(ftm2, PWM_CH_L_Z, g_nLeftPWM + PWM_DEAD_L_Z);
		}
		else if(g_nLeftPWM < 0)
		{
			if(g_nLeftPWM - PWM_DEAD_L_F <= PWM_MIN) g_nLeftPWM = PWM_MIN + PWM_DEAD_L_F;
			if(Flag_Stop == ON) g_nRighPWM -= PWM_DEAD_L_F;
			ftm_pwm_duty(ftm2, PWM_CH_L_F, - g_nLeftPWM + PWM_DEAD_L_F);
			ftm_pwm_duty(ftm2, PWM_CH_L_Z, 0);
		}
	}
}

/**
 * @备注    上位机发送函数
 */
void UISend(void)
{
//  UI_data[0] = g_nSpeedRealLeft;
//  UI_data[1] = g_nSpeedRealRight;
//  UI_data[2] = g_fDirectionError;
//  UI_data[3] = g_fDirectionControlOut;
//  UI_data[4] = Flag_Warning;
//  UI_data[5] = Flag_Podao;
//  UI_data[6] = g_fSpeedControlOut;
//  UI_data[7] = g_fGyroX;
//	
	UI_data[0] = g_ADC_Data.Pro_Data[R_1];
	UI_data[1] = g_ADC_Data.Pro_Data[L_1];
	UI_data[2] = g_ADC_Data.Pro_Data[M_1];
	UI_data[3] = g_fangerZ;
	UI_data[4] = g_fGyroZ;
	UI_data[5] = g_fDirectionError;
	UI_data[6] = g_ADC_Data.Pro_Data[R__];
	UI_data[7] = g_ADC_Data.Pro_Data[L__];
  
  vcan_sendware((uint8 *)UI_data, sizeof(UI_data));
// 
//  WirelessSerial(speed_value_L, ASK_SPEED, speed_value_R , SPEED_PID_Output);
}

/**
 * @备注    车模状态判断函数
 */
void JudgeCarStatus(void)
{
	if(Flag_Run == ON)
		Flag_time ++;
	if(Flag_Run == OFF && gpio_get(KC) == 0)
  {
		DisableInterrupts;
		OLED_ClearScreen(BLACK);
		OLED_Cmd(0);
		while(gpio_get(KC) == 0);
		systick_delay_ms(1000);
		Flag_Timer = !gpio_get(S2);
		Flag_Magnet = gpio_get(S1);
		pit_init_ms(pit0, 5);
		Flag_Run = ON;
		EnableInterrupts;
  }
  if(Flag_Run == ON && Flag_time >= Flag_timeset * 20 && Flag_Timer > 0)                            //定时停车
  {
     Flag_Stop = ON;
  }
	if(Flag_Run == ON && Flag_time >= Flag_timeset * 20 + 100 && Flag_Timer > 0)
  {
    Flag_Motor = OFF;
  }
  if(g_ADC_Data.Pro_Data[L__] <= 8 && g_ADC_Data.Pro_Data[M_1] <= 8 && g_ADC_Data.Pro_Data[R__] <= 8 && Flag_Run == ON  && Flag_Timer > 0)
  {
    Flag_Stop = ON;
  }
	if(Flag_Warning != 0) gpio_set(BUZZ, 1);
	else gpio_set(BUZZ, 0);
}
