#include "KEA128_mcp41.h"
#include "KEA128_oled.h"

PTX_n CSX[8] = {(PTX_n)CS0, (PTX_n)CS1, (PTX_n)CS2, (PTX_n)CS3, (PTX_n)CS4, (PTX_n)CS5, (PTX_n)CS6, (PTX_n)CS7};
ADCn_Ch ADC_CX[8] = {(ADCn_Ch)P0, (ADCn_Ch)P1, (ADCn_Ch)P2, (ADCn_Ch)P3, (ADCn_Ch)P4, (ADCn_Ch)P5, (ADCn_Ch)P6, (ADCn_Ch)P7};

static void SPI_WriteCmd(uint8_t cmd);

#define CMD 0x11

static void SPI_WriteCmd(uint8_t cmd) //–¥√¸¡Ó
{
	uint8_t temp = 8;
	
	SPI_SCKL;
	while (temp--)
	{
		if (cmd & 0x80)
		{
			SPI_SDAH;
		}
		else
		{
			SPI_SDAL;
		}
		SPI_SCKH;
		SPI_SCKL;
		cmd <<= 1;   
	}
}

void MCP41_SetCmd(uint8 Set_Num ,uint8 data)
{
	gpio_set(CSX[Set_Num], 0);
	SPI_WriteCmd(CMD);
	SPI_WriteCmd(0xff - data);
	gpio_set(CSX[Set_Num], 1);
}

void MCP41_Init(void)
{
	gpio_init(SCK, GPO, 1);
	gpio_init(SDA, GPO, 1);
	for(int8 temp = 0; CSX[temp] != 0; temp++)
		gpio_init(CSX[temp], GPO, 1);
	FLASH_Init();
}

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
