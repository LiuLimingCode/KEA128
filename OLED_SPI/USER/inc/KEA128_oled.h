//==============================================================
//文件:KEA128OLED固件库
//占用资源:2个引脚DC,RES(或外加CS)
//更改:更改DC,RES的宏来改变引脚,添加USE_CS宏来加入CS引脚
//============================================================== 
#ifndef __KEA128_OLED_H
#define __KEA128_OLED_H

#include "headfile.h"

/*************** 选择开启功能 ***************/
#define SHOW_6X8ASCII	    				//开启显示6*8标准ASCII字符功能
#define SHOW_8X16ASCII						//开启显示8*16标准ASCII字符功能
#define SHOW_14x16CHCHAR					//开启显示14*16中文汉字功能
#define SHOW_PICTURE						//开启显示图片功能
/***************** 引脚选择 *****************/
#define OLED_DC_Pin    I3

#define OLED_RES_Pin   I2

//#define USE_CS
#ifdef USE_CS
#define OLED_CS_Pin
#endif /* USE_CS */

#define OLED_RSTH 	gpio_set(OLED_RES_Pin, 1)
#define OLED_RSTL 	gpio_set(OLED_RES_Pin, 0)

#define OLED_DCH  	gpio_set(OLED_DC_Pin, 1)
#define OLED_DCL  	gpio_set(OLED_DC_Pin, 0)

#ifdef USE_CS
#define OLED_CSH		gpio_set(OLED_CS_Pin, 1)
#define OLED_CSL		gpio_set(OLED_CS_Pin, 0)
#endif /* USE_CS */

#define OLED_X_MAX 				128
#define OLED_Y_MAX 				64
#define OLED_PAGE_MAX  		(OLED_Y_MAX/8)
#define WHITE 						(uint8_t)0XFF
#define BLACK							(uint8_t)0X00

void OLED_Init(void);
void OLED_Cmd(char NewState);
void OLED_ClearScreen(uint8_t color);
#ifdef SHOW_PICTURE
void OLED_PrintBMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1, char bmp[]);
#endif
#ifdef SHOW_6X8ASCII
void OLED_P6x8Str(uint8_t x, uint8_t y, char *ch);
void OLED_P6x8Int(uint8_t x, uint8_t y, int16_t data1, int8 set);
void OLED_P6x8Flo(uint8_t x, uint8_t y, float data1, int8 set);
#endif
#ifdef SHOW_8X16ASCII
void OLED_P8x16Str(uint8_t x, uint8_t y, char *ch);
#endif
#ifdef SHOW_14x16CHCHAR
void OLED_P14x16CHCHAR(uint8_t x, uint8_t y, char *ch);
#endif

#endif
