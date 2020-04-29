#ifndef __KEA128_MCP41_H
#define __KEA128_MCP41_H

#include "headfile.h"

#define SCK		B2
#define SDA		B3

#define SPI_SCKL	gpio_set(SCK, 0)
#define SPI_SCKH	gpio_set(SCK, 1)
#define SPI_SDAL	gpio_set(SDA, 0)
#define SPI_SDAH	gpio_set(SDA, 1)

#define CS0		I6
#define CS1		D5
#define CS2		D7
#define CS3		D6
#define CS4		H4
#define CS5		0
#define CS6		0
#define CS7		0

#define P0		AD_CH_L_1
#define P1		AD_CH_L__
#define P2		AD_CH_M_1
#define P3		AD_CH_R__
#define P4		AD_CH_R_1
#define P5		0
#define P6		0
#define P7		0

void MCP41_Init(void);
void MCP41_SetCmd(uint8 Set_Num ,uint8 data);
uint8 MCP41_SetValue(uint8 Set_Num, uint8 Ask_Value);


#endif
