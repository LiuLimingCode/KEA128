 /*!
 * @文件       mcp41.c
 * @功能       mcp41数字电位器函数实现
 * @作者       刘力铭
 * @完成时间   2019-12
 */
 
#include "KEA128_mcp41.h"
#include "KEA128_myspi.h"
#include "KEA128_oled.h"

static uint8 CSX[8] = {MCP41_CS0, MCP41_CS1, MCP41_CS2, MCP41_CS3, MCP41_CS4, MCP41_CS5, MCP41_CS6, MCP41_CS7};
static uint8 ADC_CX[8] = {MCP41_P0, MCP41_P1, MCP41_P2, MCP41_P3, MCP41_P4, MCP41_P5, MCP41_P6, MCP41_P7};

//--------------------------------------------------------------
//  @描述       MCP41数字电位器引脚初始化
//  @示例       MCP41_Init ();
//--------------------------------------------------------------
void MCP41_Init(void)
{
	SPI_InitSimulated(MCP41_SDA_Pin, MCP41_SCL_Pin);
	for(int8 temp = 0; CSX[temp] != 0xff && temp < 8; temp++)
		gpio_init((PTX_n)CSX[temp], GPO, 1);
	for(int8 temp = 0; ADC_CX[temp] != 0xff && temp < 8; temp++)
		adc_init((ADCn_Ch)ADC_CX[temp], ADC_8bit);
}

//--------------------------------------------------------------
//  @描述       MCP41数字电位器写入电阻值
//  @参数       Set_Num   设置的路数(0~7)
//  @参数       data      设置的电阻值(0~255)
//  @示例       MCP41_SetCmd (2, 0x7F);
//--------------------------------------------------------------
void MCP41_SetCmd(uint8 Set_Num ,uint8 data)
{
	gpio_set((PTX_n)CSX[Set_Num], 0);
	SPI_SendDataSimulated(MCP41_SDA_Pin, MCP41_SCL_Pin, 0x11);
	SPI_SendDataSimulated(MCP41_SDA_Pin, MCP41_SCL_Pin, 0xff - data);
	gpio_set((PTX_n)CSX[Set_Num], 1);
}

//--------------------------------------------------------------
//  @描述       MCP41数字电位器设置ADC读数值
//  @参数       Set_Num   设置的路数(0~7)
//  @参数       data      设置的读数值(0~255)
//  @返回       设置的电阻值
//  @示例       MCP41_SetValue (2, 100);
//--------------------------------------------------------------
uint8 MCP41_SetValue(uint8 Set_Num, uint8 Ask_Value)
{
	uint16 Temp_ADC_Data;
	int16 Set_Data = 0xff;
	MCP41_SetCmd(Set_Num, Set_Data);
	
	while(1)
	{
		if(Set_Data <= 0x00) break;
		systick_delay_ms(5);
		Temp_ADC_Data = adc_read((ADCn_Ch)ADC_CX[Set_Num], 1);
		Set_Data --;
		MCP41_SetCmd(Set_Num, Set_Data);
		if(Temp_ADC_Data <= Ask_Value)
			break;
	}
	MCP41_SetCmd(Set_Num, Set_Data + 1);
	systick_delay_ms(5);
	return Set_Data + 1;
}
