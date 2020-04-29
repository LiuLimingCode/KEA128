#include "KEA128_oled.h"
#include "KEA128_gpio.h"
#include "charcode.h"

static void OLED_SetPos(uint8_t x, uint8_t y);
static void OLED_PinInit(void);
static void OLED_DLY_ms(uint16_t ms);

__STATIC_INLINE void SPI_Init(void)
{
	(void)spi_init(spi1, NOT_PCS, MASTER, bus_clk_khz*500);
}

__STATIC_INLINE void OLED_WriteCmd(uint8 cmd)
{
	OLED_DCL;
	while( !( SPI1->S & SPI_S_SPTEF_MASK ));
	SPI1->D = cmd;
}

__STATIC_INLINE void OLED_WriteDat(uint8 data)
{
	OLED_DCH;
	while( !( SPI1->S & SPI_S_SPTEF_MASK ));
	SPI1->D = data;
}

//==============================================================
//功能描述:OLED显示屏屏幕显示控制函数
//参数:DISABLE,关屏幕;ENABLE,开屏幕
//==============================================================
void OLED_Cmd(char NewState)
{
	OLED_WriteCmd(0xAE | NewState);		// Set Display On/Off
							// Default => 0xAE
							// 0xAE (0x00) => Display Off
							// 0xAF (0x01) => Display On
}

//==============================================================
//功能描述:OLED显示屏清屏函数
//参数:BLACK,屏幕全暗;WHITE,屏幕全亮
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
//功能描述:OLED显示屏初始化函数
//参数:无
//==============================================================
void OLED_Init (void)
{
	OLED_PinInit();
	SPI_Init();
	
	OLED_RSTL; //初始化OLED
        
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
//功能描述:显示BMP图片  大小为(x1-x0+1) * ((y1-y0+1)*8)
//		       BMP图片先取页码y为0以8位(低位在上)为单位取满(x1-x0+1)列
//		      再取页码y为1以8?位(低位在上)为单位取满(x1-x0+1)列以此类推
//参数:x的范围为0~127,y为页的范围0~7
//==============================================================
void OLED_PrintBMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,char bmp[])
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
//功能描述:显示一个6x8标准ASCII字符串
//参数:x为显示的横坐标,y的范围为0~7,ch是要显示的字符串
//============================================================== 
void OLED_P6x8Str(uint8_t x, uint8_t y, char *ch)
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
//功能描述:显示一个6x8标准ASCII整数
//参数:x为显示的横坐标,y的范围为0~7,ch是要显示的整数
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
//功能描述:显示一个6x8的浮点数
//参数:x为显示的横坐标,y的范围为0~7,ch是要显示的浮点数
//============================================================== 
void OLED_P6x8Flo(uint8_t x, uint8_t y, float data1, int8 set)
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
    data2 = (uint32_t)(data1*100);
		result[1] = (uint8_t)((data2%1000000)/100000+48);
		result[2] = (uint8_t)((data2%100000)/10000+48);
    result[3] = (uint8_t)((data2%10000)/1000+48);
    result[4] = (uint8_t)((data2%1000)/100+48);
    result[5] = '.';
    result[6] = (uint8_t)((data2%100)/10+48);
    result[7] = (uint8_t)(data2%10+48);
    result[8] = '\0';
		
		if(set < 0)
		{
			result[4 + set] = result[0];
			OLED_P6x8Str(x, y, (char *)&(result[4 + set]));
		}
		else if(set > 0)
		{
			OLED_P6x8Str(x, y, (char *)&(result[5 - set]));
		}
}
#endif

#ifdef SHOW_8X16ASCII
//==============================================================
//功能描述:显示一个8x16标准ASCII字符串
//参数:x为显示的横坐标,y的范围为0~7,ch是要显示的字符串
//============================================================== 
void OLED_P8x16Str(uint8_t x, uint8_t y, char *ch)
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

void OLED_P8x16Int(uint8_t x, uint8_t y, uint16 data1)
{
  char result[6];
  
  result[0] = (uint8_t)((data1%100000)/10000+48);
  result[1] = (uint8_t)((data1%10000)/1000+48);
  result[2] = (uint8_t)((data1%1000)/100+48);
  result[3] = (uint8_t)((data1%100)/10+48);
  result[4] = (uint8_t)(data1%10+48);
  result[5] = '\0';
  
  OLED_P8x16Str(x, y, result);
}
#endif

#ifdef SHOW_14x16CHCHAR
//==============================================================
//功能描述:显示一个14x16的中文字符
//参数:x为显示的横坐标,y的范围为0~7,ch是要显示的中文字符
//宋体11,纵向取模,字节倒叙
//============================================================== 
void OLED_P14x16CHCHAR(uint8_t x, uint8_t y, char *ch)
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

/****************** 以下为内部函数，不可调用 ******************/
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
        gpio_init(OLED_RES_Pin, GPO, 1);
        gpio_init(OLED_DC_Pin, GPO, 1);
}

static void OLED_SetPos(uint8_t x, uint8_t y)//设置坐标
{ 
	OLED_WriteCmd(0xb0+y);
	OLED_WriteCmd(((x&0xf0)>>4)|0x10);
	OLED_WriteCmd((x&0x0f)|0x01); 
}
