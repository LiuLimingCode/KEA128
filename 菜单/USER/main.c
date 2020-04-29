///*********************************************************************************************************************
// * COPYRIGHT NOTICE
// * Copyright (c) 2017,逐飞科技
// * All rights reserved.
// * 技术讨论QQ群：179029047
// *
// * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
// * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
// *
// * @file       		main
// * @company	   		成都逐飞科技有限公司
// * @author     		逐飞科技(QQ3184284598)
// * @version    		v2.0
// * @Software 		IAR 7.7 or MDK 5.23
// * @Target core		S9KEA128AMLK
// * @Taobao   		https://seekfree.taobao.com/
// * @date       		2017-11-6
// ********************************************************************************************************************/

#include "headfile.h"

void Init_All(void);
void Normalized(void);
void Normalized_MCP41(void);
void Normalized_Flash(void);

int main(void)
{
	get_clk();
	DisableInterrupts;
	Init_All();
	
	if(gpio_get(S4) == 0) Normalized_MCP41();
	if(gpio_get(S3) == 0) Normalized();
	Menu_Init();
	while(!Menu_Work()) systick_delay_ms(200);
	Normalized_Flash();
	
	enable_irq(PIT_CH0_IRQn);
	EnableInterrupts;
	
	while(1)
	{
		if(Flag_PIT == 1)
		{
			Flag_PIT = 0;
			DataRead();					//陀螺仪,电感值读取
		
			SpeedControl();			//速度环控制
			
			DirectionControl();	//方向环控制
			
			PWMOut();						//PWM输出
			
			UISend();						//上位机数据发送
			
			if(Flag_Run == OFF)
			{
				//OLED显示函数全部在这里
				OLED_P6x8Int(OLED_SHOW(1), 0, g_ADC_Data.Raw_Data[L_1], -3);
				OLED_P6x8Int(OLED_SHOW(2), 0, g_ADC_Data.Raw_Data[L__], -3);
				OLED_P6x8Int(OLED_SHOW(3), 0, g_ADC_Data.Raw_Data[M_1], -3);
				OLED_P6x8Int(OLED_SHOW(4), 0, g_ADC_Data.Raw_Data[R__], -3);
				OLED_P6x8Int(OLED_SHOW(5), 0, g_ADC_Data.Raw_Data[R_1], -3);
				
				OLED_P6x8Int(OLED_SHOW(1), 1, g_ADC_Data.Pro_Data[L_1], -3);
				OLED_P6x8Int(OLED_SHOW(2), 1, g_ADC_Data.Pro_Data[L__], -3);
				OLED_P6x8Int(OLED_SHOW(3), 1, g_ADC_Data.Pro_Data[M_1], -3);
				OLED_P6x8Int(OLED_SHOW(4), 1, g_ADC_Data.Pro_Data[R__], -3);
				OLED_P6x8Int(OLED_SHOW(5), 1, g_ADC_Data.Pro_Data[R_1], -3);
				
				OLED_P6x8Int(OLED_SHOW(1), 4, g_nSpeedLeft, -3);
				OLED_P6x8Int(OLED_SHOW(2), 4, g_nSpeedRight, -3);
				OLED_P6x8Int(OLED_SHOW(3), 4, g_fDirectionError, -3);
				OLED_P6x8Int(OLED_SHOW(4), 4, chabihe, -3);
				OLED_P6x8Int(OLED_SHOW(5), 4, g_ADC_Data.Pro_Data[L__] + g_ADC_Data.Pro_Data[R__], -3);
				
				
				OLED_P6x8Int(OLED_SHOW(1), 5, Flag_Huandao, -1);
				OLED_P6x8Int(OLED_SHOW(2), 5, Flag_Huandao_DIR, -1);
				
				OLED_P6x8Int(OLED_SHOW(4), 5, g_ADC_Data.Pro_Data[L__] + g_ADC_Data.Pro_Data[L_1], -3);
				OLED_P6x8Int(OLED_SHOW(5), 5, g_ADC_Data.Pro_Data[R__] + g_ADC_Data.Pro_Data[R_1], -3);
				
				OLED_P6x8Int(OLED_SHOW(1), 7, g_nBattery, 3);
				OLED_P6x8Int(OLED_SHOW(2), 7, Flag_Warning, 3);
			}
			
			JudgeCarStatus();		//车模状态判断
		
		}
	}
}

void Normalized(void)
{
	uint16 Temp_ADC_Data[ADC_NUM];
	
	OLED_ClearScreen(BLACK);//清屏
	OLED_P6x8Int(OLED_SHOW(3) + 6, 2, 193 , 3); OLED_P6x8Str(OLED_SHOW(3) + 6, 1, "M_1");
	OLED_P6x8Int(OLED_SHOW(2) + 6, 2, 193 , 3); OLED_P6x8Str(OLED_SHOW(2) + 6, 1, "L__");
	OLED_P6x8Int(OLED_SHOW(4) + 6, 2, 193 , 3); OLED_P6x8Str(OLED_SHOW(4) + 6, 1, "R__");
	OLED_P6x8Int(OLED_SHOW(1) + 6, 2, 193 , 3); OLED_P6x8Str(OLED_SHOW(1) + 6, 1, "L_1");
	OLED_P6x8Int(OLED_SHOW(5) + 6, 2, 193 , 3); OLED_P6x8Str(OLED_SHOW(5) + 6, 1, "R_1");
	
	for(int temp = 0; temp < ADC_NUM; ++temp)
		g_ADC_Data.Min_Data[temp] = 3088;
	
	while(1)
	{
		Temp_ADC_Data[0] = adc_read(AD_CH_L_1, 16);
		Temp_ADC_Data[1] = adc_read(AD_CH_L__, 16);
		Temp_ADC_Data[2] = adc_read(AD_CH_M_1, 16);
		Temp_ADC_Data[3] = adc_read(AD_CH_R__, 16);
		Temp_ADC_Data[4] = adc_read(AD_CH_R_1, 16);
		
		for(int temp = 0; temp < ADC_NUM; ++temp)
		{
			OLED_P6x8Int(OLED_SHOW(temp + 1) + 6, 4, Temp_ADC_Data[temp] / 16.0 , 3);
			if(Temp_ADC_Data[temp] < g_ADC_Data.Min_Data[temp])
			{
				OLED_P6x8Int(OLED_SHOW(temp + 1) + 6, 2, Temp_ADC_Data[temp] / 16.0 , 3);
				g_ADC_Data.Min_Data[temp] = Temp_ADC_Data[temp];
			}
		}
		
		if(gpio_get(KU) == 0)
		{
			FLASH_EraseSector(FLASH_SAVE_ADC_NORMAL);
			for(int8 temp = 0; temp < ADC_NUM; temp++)
				FLASH_WriteSector(FLASH_SAVE_ADC_NORMAL, 32 * temp, g_ADC_Data.Min_Data[temp]);
			break;
		}
		if(gpio_get(KD) == 0) break;
		
		systick_delay_ms(50);
	}
}

/**
 * @备注    数字电位器人机界面操作函数，MCP41_Set数组为电感的预设值
 */
uint8 MCP41_Flash_Save[8];
ADCn_Ch MCP41_ADC[ADC_NUM] = {AD_CH_L_1, AD_CH_L__, AD_CH_M_1, AD_CH_R__, AD_CH_R_1};
uint8 MCP41_Set[ADC_NUM] = {80, 100, 80, 100, 80};
void Normalized_MCP41(void)
{
	int8 _index = 1, _index_old = 1;
	
	OLED_P6x8Str((OLED_X_MAX - 1 - 6 * (sizeof("Normalized") - 1)) / 2, 0, "Normalized");
	OLED_P6x8Str(OLED_SHOW(1), 1, "L_1");
	OLED_P6x8Str(OLED_SHOW(2), 1, "L__");
	OLED_P6x8Str(OLED_SHOW(3), 1, "M_1");
	OLED_P6x8Str(OLED_SHOW(4), 1, "R__");
	OLED_P6x8Str(OLED_SHOW(5), 1, "R_1");
	while(1)
	{
		OLED_P6x8Int(OLED_SHOW(1), 3, adc_read(AD_CH_L_1, 16) / 16, 3);
		OLED_P6x8Int(OLED_SHOW(2), 3, adc_read(AD_CH_L__, 16) / 16, 3);
		OLED_P6x8Int(OLED_SHOW(3), 3, adc_read(AD_CH_M_1, 16) / 16, 3);
		OLED_P6x8Int(OLED_SHOW(4), 3, adc_read(AD_CH_R__, 16) / 16, 3);
		OLED_P6x8Int(OLED_SHOW(5), 3, adc_read(AD_CH_R_1, 16) / 16, 3);
		
		if(gpio_get(KL) == 0) _index--;
		if(gpio_get(KR) == 0) _index++;
		if(gpio_get(KU) == 0)
		{
			FLASH_EraseSector(FLASH_SAVE_MCP41);
			for(int8 temp = 0; temp < 8; temp++)
				FLASH_WriteSector(FLASH_SAVE_MCP41, 32 * temp, MCP41_Flash_Save[temp]);
			FLASH_EraseSector(FLASH_SAVE_ADC_NORMAL);
			for(int8 temp = 0; temp < ADC_NUM; temp++)
				FLASH_WriteSector(FLASH_SAVE_ADC_NORMAL, 32 * temp, g_ADC_Data.Min_Data[temp]);
			break;
		}
		if(gpio_get(KD) == 0) break;
		if(gpio_get(KC) == 0)
		{
			OLED_P6x8Str(OLED_SHOW(_index), 2, " & ");
			MCP41_Flash_Save[_index - 1] = MCP41_SetValue(_index - 1, MCP41_Set[_index  - 1]);
			systick_delay_ms(50);
			g_ADC_Data.Min_Data[_index - 1] = adc_read(MCP41_ADC[_index - 1], 16);
			OLED_P6x8Str(OLED_SHOW(_index), 2, " * ");
			OLED_P6x8Int(OLED_SHOW(_index), 4, MCP41_Set[_index - 1], 3);
		}
		
		if(_index == 0) _index = ADC_NUM;
		if(_index == ADC_NUM + 1) _index = 1;
		
		OLED_P6x8Str(OLED_SHOW(_index), 2, " * ");
		if(_index != _index_old)
			OLED_P6x8Str(OLED_SHOW(_index_old), 2, "   ");
		
		_index_old = _index;
		systick_delay_ms(50);
		
		for(int temp = 0; temp < 3; ++temp)
		{
			OLED_P6x8Int(OLED_SHOW(1), 3, adc_read(AD_CH_L_1, 16) / 16, 3);
			OLED_P6x8Int(OLED_SHOW(2), 3, adc_read(AD_CH_L__, 16) / 16, 3);
			OLED_P6x8Int(OLED_SHOW(3), 3, adc_read(AD_CH_M_1, 16) / 16, 3);
			OLED_P6x8Int(OLED_SHOW(4), 3, adc_read(AD_CH_R__, 16) / 16, 3);
			OLED_P6x8Int(OLED_SHOW(5), 3, adc_read(AD_CH_R_1, 16) / 16, 3);
			systick_delay_ms(50);
		}
	}
	OLED_ClearScreen(BLACK);
	while(gpio_get(KU) == 0);
	while(gpio_get(KD) == 0);
}

/**
 * @备注    从FLASH中读取ADC的实际最小值用于归一化计算
 */
void Normalized_Flash(void)
{
	for(uint8 temp= 0; temp < ADC_NUM; temp++)
	{
		g_ADC_Data.Min_Data[temp] = flash_read(FLASH_SAVE_ADC_NORMAL, 32 * temp, int16);
		OLED_P6x8Int(OLED_SHOW(temp + 1) + 6, 2, g_ADC_Data.Min_Data[temp] ,3);
	}
	OLED_P6x8Str(OLED_SHOW(1) + 6, 3, "L_1");
	OLED_P6x8Str(OLED_SHOW(2) + 6, 3, "L__");
	OLED_P6x8Str(OLED_SHOW(3) + 6, 3, "M_1");
	OLED_P6x8Str(OLED_SHOW(4) + 6, 3, "R__");
	OLED_P6x8Str(OLED_SHOW(5) + 6, 3, "R_1");
}

/**
 * @备注    全车初始化函数
 */
void Init_All(void)
{
	adc_init(AD_CH_M_1, ADC_12bit);
	adc_init(AD_CH_L__, ADC_12bit);
	adc_init(AD_CH_R__, ADC_12bit);
	adc_init(AD_CH_L_1, ADC_12bit);
	adc_init(AD_CH_R_1, ADC_12bit);
	adc_init(AD_CH_BAT, ADC_12bit);
	
	OLED_Init();
	
	FLASH_Init();
	
	UI_Init();
	
	FXAS2100_Init();
	FXOS8700_Init();
	
	MCP41_Init();
	
	for(int8 temp = 0; temp < 8; temp++)
	{
		MCP41_Flash_Save[temp] = flash_read(FLASH_SAVE_MCP41, 32 * temp, uint8);
	}
	for(int8 temp = 0; temp < ADC_NUM; temp++)
	{
		MCP41_SetCmd(temp, MCP41_Flash_Save[temp]);
	}
	for(uint8 temp= 0; temp < ADC_NUM; temp++)
	{
		g_ADC_Data.Min_Data[temp] = flash_read(FLASH_SAVE_ADC_NORMAL, 32 * temp, int16);
	}
	
	gpio_init(BUZZ, GPO, 0);
	
	gpio_init(KU, GPI, 1);
	gpio_init(KD, GPI, 1);
	gpio_init(KC, GPI, 1);
	gpio_init(KL, GPI, 1);
	gpio_init(KR, GPI, 1);
	
	gpio_init(S1, GPI, 1);
	gpio_init(S2, GPI, 1);
	gpio_init(S3, GPI, 1);
	gpio_init(S4, GPI, 1);
	
	pit_init_ms(pit0, 20);
  set_irq_priority(PIT_CH0_IRQn, 3);
	
	gpio_init(CODER_DIR_L, GPI, 1);
	gpio_init(CODER_DIR_R, GPI, 1);
  ftm_count_init(CODER_L);
	ftm_count_init(CODER_R);
	
	ftm_pwm_init(ftm2, PWM_CH_R_Z, 7500, 0);
	ftm_pwm_init(ftm2, PWM_CH_R_F, 7500, 0);
	ftm_pwm_init(ftm2, PWM_CH_L_Z, 7500, 0);
	ftm_pwm_init(ftm2, PWM_CH_L_F, 7500, 0);
}
