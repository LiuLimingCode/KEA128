#include  "ui.h"
#include  "KEA128_uart.h"

Bluetooth_Send Bluetooth_Send_Data={0};

void UARTX_IRQHandler(void)
{
	if(UARTX->S1 & UART_S1_TC_MASK)
	{
		if(Bluetooth_Send_Data.Send_Finish_Len < Bluetooth_Send_Data.Send_len)
		{
			UARTX->D = (uint8)(Bluetooth_Send_Data.Send_Data[Bluetooth_Send_Data.Send_Finish_Len]);
			Bluetooth_Send_Data.Send_Finish_Len++;
		}
		else
		{
			UARTX->C2 |= UART_C2_SBK_MASK;
		}
	}
}

void UI_Init(void)
{
	uart_init(uartx, 115200);
	uart_txc_irq_en(uartx);
	set_irq_priority(UARTX_IRQn, 3);
}

unsigned int CRC_Check(unsigned char *Buf, unsigned char crc_count)
{
  unsigned short CRC_Temp;
  unsigned char i,j;
  CRC_Temp = 0xffff;
  
  for (i=0;i<crc_count; i++){
    CRC_Temp ^= Buf[i];
    for(j=0;j<8;j++){
      if (CRC_Temp & 0x01)
        CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
      else
        CRC_Temp = CRC_Temp >> 1;
    }
  }
  return(CRC_Temp);
}

void WirelessSerial(float a,float b,float c,float d)
{
	uint8 TxBuf[10]={0};
  int num;
  unsigned char *data, temp;
  
#if 1
  num = (int)a;    
  data = (unsigned char *)&num;
  TxBuf[0] = *data;
  data++;
  TxBuf[1] = *data;
  
  num = (int)b;      
  data = (unsigned char *)&num;
  TxBuf[2] = *data;
  data++;
  TxBuf[3] = *data;
  
  num = (int)c;        
  data = (unsigned char *)&num;
  TxBuf[4] = *data;
  data++;
  TxBuf[5] = *data;
  
  num = (int)d;
  data = (unsigned char *)&num;
  TxBuf[6] = *data;
  data++;
  TxBuf[7] = *data;
#endif
  
  num = CRC_Check(TxBuf,8);
  TxBuf[8] = num & 0xFF;
  TxBuf[9] = num >> 8;
	
	if(Bluetooth_Send_Data.Send_Finish_Len < Bluetooth_Send_Data.Send_len)
		Bluetooth_Send_Data.Send_Error++;
	
	for(temp = 0; temp < 10; ++temp)
	{
		Bluetooth_Send_Data.Send_Data[temp] = TxBuf[temp];
	}
	Bluetooth_Send_Data.Send_len = 10;
	Bluetooth_Send_Data.Send_Finish_Len = 1;
	
	UARTX->C2 &= ~UART_C2_SBK_MASK;
	
	UARTX->D = (uint8)(Bluetooth_Send_Data.Send_Data[0]);
}


void vcan_sendware(void *wareaddr, uint32 waresize)
{
	uint16 temp;
#define CMD_WARE     3
	
	if(Bluetooth_Send_Data.Send_Finish_Len < Bluetooth_Send_Data.Send_len)
		Bluetooth_Send_Data.Send_Error++;
	
	Bluetooth_Send_Data.Send_Data[0] = CMD_WARE;
	Bluetooth_Send_Data.Send_Data[1] = ~CMD_WARE;
	
	for(temp = 0; temp < waresize; ++temp)
	{
		Bluetooth_Send_Data.Send_Data[2 + temp] = ((uint8_t *)wareaddr)[temp];
	}
	
	Bluetooth_Send_Data.Send_Data[2 + waresize] = ~CMD_WARE;
	Bluetooth_Send_Data.Send_Data[3 + waresize] = CMD_WARE;
	
	Bluetooth_Send_Data.Send_len = waresize + 4;
	Bluetooth_Send_Data.Send_Finish_Len = 1;
	
	UARTX->C2 &= ~UART_C2_SBK_MASK;
	
	UARTX->D = (uint8)(Bluetooth_Send_Data.Send_Data[0]);
}
