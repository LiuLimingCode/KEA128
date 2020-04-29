#ifndef __UI_H
#define __UI_H

#include "headfile.h"

#define	UARTX							UART0
#define UARTX_IRQHandler	UART0_IRQHandler
#define	uartx							uart0
#define UARTX_IRQn				UART0_IRQn

typedef struct
{
  uint8 Send_Data[68];
	uint16 Send_len;
	uint16 Send_Finish_Len;
	uint16 Send_Error;
}Bluetooth_Send;

void UARTX_IRQHandler(void);
void UI_Init(void);
void WirelessSerial(float a,float b,float c,float d);
void vcan_sendware(void *wareaddr, uint32 waresize);


#endif