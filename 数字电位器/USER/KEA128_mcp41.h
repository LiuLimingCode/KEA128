/*!
 * @file       mcp41.h
 * @brief      mcp41数字电位器函数实现
 * @author     llm
 */
#ifndef __KEA128_MCP41_H
#define __KEA128_MCP41_H

#include "headfile.h"

#define MCP41_SCK		B2
#define MCP41_SDA		B3

#define SPI_MCP41_SCKL	gpio_set(MCP41_SCK, 0)
#define SPI_MCP41_SCKH	gpio_set(MCP41_SCK, 1)
#define SPI_MCP41_SDAL	gpio_set(MCP41_SDA, 0)
#define SPI_MCP41_SDAH	gpio_set(MCP41_SDA, 1)

#define AD_CH_L_1				ADC0_SE15
#define AD_CH_L__				ADC0_SE8
#define AD_CH_M__				ADC0_SE4
#define AD_CH_R__				ADC0_SE13
#define AD_CH_R_1				ADC0_SE12

#define CS0		I6
#define CS1		H4
#define CS2		F3
#define CS3		H3
#define CS4		A7
#define CS5		0
#define CS6		0
#define CS7		0

#define P0		AD_CH_L_1
#define P1		AD_CH_L__
#define P2		AD_CH_M__
#define P3		AD_CH_R__
#define P4		AD_CH_R_1
#define P5		0
#define P6		0
#define P7		0

void MCP41_Init(void);
void MCP41_SetCmd(uint8 Set_Num ,uint8 data);
uint8 MCP41_SetValue(uint8 Set_Num, uint8 Ask_Value);

#endif
