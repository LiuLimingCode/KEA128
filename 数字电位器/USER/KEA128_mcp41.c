/*!
 * @file       mcp41.c
 * @brief      mcp41���ֵ�λ������ʵ��
 * @author     llm
 */
#include "KEA128_mcp41.h"
#include "KEA128_oled.h"

static void SPI_WriteCmd(uint8_t cmd);

PTX_n CSX[8] = {(PTX_n)CS0, (PTX_n)CS1, (PTX_n)CS2, (PTX_n)CS3, (PTX_n)CS4, (PTX_n)CS5, (PTX_n)CS6, (PTX_n)CS7};
ADCn_Ch ADC_CX[8] = {(ADCn_Ch)P0, (ADCn_Ch)P1, (ADCn_Ch)P2, (ADCn_Ch)P3, (ADCn_Ch)P4, (ADCn_Ch)P5, (ADCn_Ch)P6, (ADCn_Ch)P7};

	/*!
*  @����       MCP41���ֵ�λ�����ų�ʼ��
*  @����       ��
*  ʾ��:       MCP41_Init ();
*/
void MCP41_Init(void)
{
	gpio_init(MCP41_SCK, GPO, 1);
	gpio_init(MCP41_SDA, GPO, 1);
	for(int8 temp = 0; CSX[temp] != 0; temp++)
		gpio_init(CSX[temp], GPO, 1);
}
	
/*!
*  @����       MCP41���ֵ�λ��д�����ֵ
*  @����       Set_Num       ���õ�·��(0~7)
*							 data					 ���õĵ���ֵ(0~255)
*  ʾ��:       MCP41_SetCmd (2, 0x7F);
*/
void MCP41_SetCmd(uint8 Set_Num ,uint8 data)
{
	gpio_set(CSX[Set_Num], 0);
#define CMD 0x11
	SPI_WriteCmd(CMD);
	SPI_WriteCmd(0xff - data);
	gpio_set(CSX[Set_Num], 1);
}

/*!
*  @����       MCP41���ֵ�λ������ADC����ֵ
*  @����       Set_Num       ���õ�·��(0~7)
*							 data					 ���õĶ���ֵ(0~255)
*  @����       ���õĵ���ֵ
*  ʾ��:       MCP41_SetValue (2, 100);
*/
uint8 MCP41_SetValue(uint8 Set_Num, uint8 Ask_Value)
{
	uint16 Temp_ADC_Data;
	int16 Set_Data = 0xff;
	MCP41_SetCmd(Set_Num, Set_Data);
	
	while(1)
	{
		if(Set_Data <= 0x00) break;
		systick_delay_ms(5);
		Temp_ADC_Data = adc_read(ADC_CX[Set_Num], 1) / 16;
		Set_Data --;
		MCP41_SetCmd(Set_Num, Set_Data);
		if(Temp_ADC_Data <= Ask_Value)
			break;
	}
	MCP41_SetCmd(Set_Num, Set_Data + 1);
	systick_delay_ms(5);
	return Set_Data + 1;
}

////////////////////////////////////////////////
//�ڲ�����
static void SPI_WriteCmd(uint8_t cmd) //д����
{
	uint8_t temp = 8;
	
	SPI_MCP41_SCKL;
	while (temp--)
	{
		if (cmd & 0x80)
		{
			SPI_MCP41_SDAH;
		}
		else
		{
			SPI_MCP41_SDAL;
		}
		SPI_MCP41_SCKH;
		SPI_MCP41_SCKL;
		cmd <<= 1;   
	}
}
