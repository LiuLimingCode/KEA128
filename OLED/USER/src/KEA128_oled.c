#include "KEA128_oled.h"
#include "KEA128_gpio.h"
#include "charcode.h"

static void OLED_WriteDat(uint8_t data);
static void OLED_WriteCmd(uint8_t cmd);
static void OLED_SetPos(uint8_t x, uint8_t y);
static void OLED_PinInit(void);
static void OLED_DLY_ms(uint16_t ms);

//==============================================================
//��������:OLED��ʾ����Ļ��ʾ���ƺ���
//����:DISABLE,����Ļ;ENABLE,����Ļ
//==============================================================
void OLED_Cmd(char NewState)
{
	OLED_WriteCmd(0xAE | NewState);		// Set Display On/Off
							// Default => 0xAE
							// 0xAE (0x00) => Display Off
							// 0xAF (0x01) => Display On
}

//==============================================================
//��������:OLED��ʾ����������
//����:BLACK,��Ļȫ��;WHITE,��Ļȫ��
//==============================================================
void OLED_ClearScreen(uint8_t color)
{
	uint8_t y,x;	
	for(y=0; y < OLED_PAGE_MAX; y++)
	{
		OLED_WriteCmd(0xb0 + y);
		OLED_WriteCmd(0x01);
		OLED_WriteCmd(0x10); 
		for(x=0; x < OLED_X_MAX; x++)
			OLED_WriteDat(color);
	}
}

//==============================================================
//��������:OLED��ʾ����ʼ������
//����:��
//==============================================================
void OLED_Init (void)
{
	OLED_PinInit();

	OLED_SCLH;	
	OLED_RSTL; //��ʼ��OLED
        
  OLED_DLY_ms(50);
	OLED_RSTH;

	OLED_Cmd(0);//--turn off oled panel
  OLED_WriteCmd(0x00);//---set low column address
  OLED_WriteCmd(0x10);//---set high column address
  OLED_WriteCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_WriteCmd(0x81);//--set contrast control register
  OLED_WriteCmd(0xcf); // Set SEG Output Current Brightness
  OLED_WriteCmd(0xa1);//--Set SEG/Column Mapping     //
  OLED_WriteCmd(0xc8);//Set COM/Row Scan Direction  //
  OLED_WriteCmd(0xa6);//--set normal display
  OLED_WriteCmd(0xa8);//--set multiplex ratio(1 to 64)
  OLED_WriteCmd(0x3f);//--1/64 duty
  OLED_WriteCmd(0xd3);//-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WriteCmd(0x00);//-not offset
  OLED_WriteCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  OLED_WriteCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WriteCmd(0xd9);//--set pre-charge period
  OLED_WriteCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WriteCmd(0xda);//--set com pins hardware configuration
  OLED_WriteCmd(0x12);
  OLED_WriteCmd(0xdb);//--set vcomh
  OLED_WriteCmd(0x40);//Set VCOM Deselect Level
  OLED_WriteCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  OLED_WriteCmd(0x02);//
  OLED_WriteCmd(0x8d);//--set Charge Pump enable/disable
  OLED_WriteCmd(0x14);//--set(0x10) disable
  OLED_WriteCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  OLED_WriteCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
  OLED_Cmd(1);//--turn on oled panel
	OLED_ClearScreen(BLACK);
	OLED_SetPos(0, 0);
}

#ifdef SHOW_PICTURE
//==============================================================
//��������:��ʾBMPͼƬ  ��СΪ(x1-x0+1) * ((y1-y0+1)*8)
//		       BMPͼƬ��ȡҳ��yΪ0��8λ(��λ����)Ϊ��λȡ��(x1-x0+1)��
//		      ��ȡҳ��yΪ1��8?λ(��λ����)Ϊ��λȡ��(x1-x0+1)���Դ�����
//����:x�ķ�ΧΪ0~127,yΪҳ�ķ�Χ0~7
//==============================================================
void OLED_PrintBMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,const char bmp[])
{ 	
	uint16_t temp=0;
	uint8_t x,y;
	for(y = y0; y <= y1; ++y)
	{
		OLED_SetPos(x0,y);				
		for(x = x0; x <= x1; ++x)
		{      
			OLED_WriteDat(bmp[temp++]);	    	
		}
	}
}
#endif

#ifdef SHOW_6X8ASCII
//==============================================================
//��������:��ʾһ��6x8��׼ASCII�ַ���
//����:xΪ��ʾ�ĺ�����,y�ķ�ΧΪ0~7,ch��Ҫ��ʾ���ַ���
//============================================================== 
void OLED_P6x8Str(uint8_t x, uint8_t y, const char *ch)
{
	uint8_t c = 0, i = 0, j = 0;      
	while (ch[j]!='\0')
	{    
		c =ch[j]-32;
		if(x>126)
		{
			x=0;
			y++;
		}
		OLED_SetPos(x,y);    
		for(i=0;i<6;i++)
		{     
			OLED_WriteDat(F6x8[c][i]);  
		}
		x+=6;
		j++;
	}
}

//==============================================================
//��������:��ʾһ��6x8��׼ASCII����
//����:xΪ��ʾ�ĺ�����,y�ķ�ΧΪ0~7,ch��Ҫ��ʾ������
//============================================================== 
void OLED_P6x8Int(uint8_t x, uint8_t y, int16_t data1, int8 set)
{
  uint8_t result[7];
  if(data1<0)
  {
    data1 *= -1;
    result[0] = '-';
  }
  else
  {
    result[0] = '+';
  }
  result[1] = (uint8_t)((data1%100000)/10000+48);
  result[2] = (uint8_t)((data1%10000)/1000+48);
  result[3] = (uint8_t)((data1%1000)/100+48);
  result[4] = (uint8_t)((data1%100)/10+48);
  result[5] = (uint8_t)(data1%10+48);
  result[6] = '\0';
  if(set < 0)
  {
    result[5 + set] = result[0];
    OLED_P6x8Str(x, y, (char *)&(result[5 + set]));
  }
  else if(set > 0)
  {
    OLED_P6x8Str(x, y, (char *)&(result[6 - set]));
  }
}

//==============================================================
//��������:��ʾһ��6x8�ĸ�����
//����:xΪ��ʾ�ĺ�����,y�ķ�ΧΪ0~7,ch��Ҫ��ʾ�ĸ�����
//============================================================== 
void OLED_P6x8Flo(uint8_t x, uint8_t y, float data1)
{
    uint8_t result[9];
    uint32_t data2;
    if(data1<0)
		{			
      result[0] = '-';
      data1 *= -1;
    }
    else
		{
    result[0]='+';
		}
    data1 += 0.000001;
    data2 = (uint32_t)(data1*1000);
		result[1] = (uint8_t)((data2%1000000)/100000+48);
    result[2] = (uint8_t)((data2%100000)/10000+48);
    result[3] = (uint8_t)((data2%10000)/1000+48);
    result[4] = '.';
    result[5] = (uint8_t)((data2%1000)/100+48);
    result[6] = (uint8_t)((data2%100)/10+48);
    result[7] = (uint8_t)(data2%10+48);
    result[8] = '\0';
    OLED_P6x8Str(x, y, (char *)result);
}
#endif

#ifdef SHOW_8X16ASCII
//==============================================================
//��������:��ʾһ��8x16��׼ASCII�ַ���
//����:xΪ��ʾ�ĺ�����,y�ķ�ΧΪ0~7,ch��Ҫ��ʾ���ַ���
//============================================================== 
void OLED_P8x16Str(uint8_t x, uint8_t y, const char *ch)
{
  uint8_t c = 0, i = 0, j = 0;
  while (ch[j] != '\0')
  {    
    c = ch[j] - 32;
    if(x > 120)
		{
			x=0;
			y++;
		}
    OLED_SetPos(x,y);
  	for(i=0;i<8;i++)
		{
  	  OLED_WriteDat(F8X16[c*16+i]);
		}
  	OLED_SetPos(x,y+1);
  	for(i=0;i<8;i++)
		{
  	  OLED_WriteDat(F8X16[c*16+i+8]);
		}
  	x+=8;
  	j++;
  }
}
#endif

#ifdef SHOW_14x16CHCHAR
//==============================================================
//��������:��ʾһ��14x16�������ַ�
//����:xΪ��ʾ�ĺ�����,y�ķ�ΧΪ0~7,ch��Ҫ��ʾ�������ַ�
//����11,����ȡģ,�ֽڵ���
//============================================================== 
void OLED_P14x16CHCHAR(uint8_t x, uint8_t y, const char *ch)
{
	uint8_t wordnum, i;
	while(*ch != '\0')
	{
		if(x > 114)
		{
			x = 0;
			++ y;
		}
		OLED_SetPos(x, y);
		for(wordnum = 0; wordnum < sizeof(Word14x16)/31; ++wordnum)
		{
			if(((*ch & 0xff) == (Word14x16[wordnum].word_name[0] & 0xff)) && ((*(ch+1) & 0xff) == (Word14x16[wordnum].word_name[1] & 0xff)))
			{
				for(i=0; i<14; i++)
				{
					OLED_WriteDat(Word14x16[wordnum].index[i]);
				}
				OLED_SetPos(x, y+1);
				for(i=14; i<28; i++)
				{
					OLED_WriteDat(Word14x16[wordnum].index[i]);
				}
				x += 14;
				ch += 2;
				break;
			}
		}
	}
}
#endif

/****************** ����Ϊ�ڲ����������ɵ��� ******************/
static void OLED_DLY_ms(uint16_t ms)
{                         
  unsigned int a;
  while(ms)
  {
    a=222;
    while(a--);
    ms--;
  }
  return;
}

static void OLED_PinInit(void)
{
        gpio_init(OLED_SCL_Pin, GPO, 1);
        gpio_init(OLED_SDA_Pin, GPO, 1);
        gpio_init(OLED_RES_Pin, GPO, 1);
        gpio_init(OLED_DC_Pin, GPO, 1);
}

static void OLED_WriteDat(uint8_t data)   //д����
{
	uint8_t temp = 8;
#ifdef USE_CS
	OLED_CSL;
#endif /* USE_CS */
	OLED_DCH;
	OLED_SCLL;
	while (temp--)
	{
		if (data & 0x80)
		{
			OLED_SDAH;
		}
		else
		{
			OLED_SDAL;
		}
		OLED_SCLH;
		OLED_SCLL;
		data <<= 1;    
	}
#ifdef USE_CS
	OLED_CSH;
#endif /* USE_CS */
}
static void OLED_WriteCmd(uint8_t cmd) //д����
{
	uint8_t temp = 8;
#ifdef USE_CS
	OLED_CSL;
#endif /* USE_CS */
	OLED_DCL;
	OLED_SCLL;
	while (temp--)
	{
		if (cmd & 0x80)
		{
			OLED_SDAH;
		}
		else
		{
			OLED_SDAL;
		}
		OLED_SCLH;
		OLED_SCLL;
		cmd <<= 1;   
	}
#ifdef USE_CS
	OLED_CSH;
#endif /* USE_CS */
}

static void OLED_SetPos(uint8_t x, uint8_t y)//��������
{ 
	OLED_WriteCmd(0xb0+y);
	OLED_WriteCmd(((x&0xf0)>>4)|0x10);
	OLED_WriteCmd((x&0x0f)|0x01); 
}