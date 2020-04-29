//==============================================================
//�ļ�:KEA128OLED�̼���
//ռ����Դ:4������,SDA,SCL,DC,RES(�����CS)
//����:����SDA,SCL,DC,RES�ĺ����ı�����,���USE_CS��������CS����
//============================================================== 
#ifndef __KEA128_OLED_H
#define __KEA128_OLED_H

#include "headfile.h"

/*************** ѡ�������� ***************/
#define SHOW_6X8ASCII	    				//������ʾ6*8��׼ASCII�ַ�����
#define SHOW_8X16ASCII						//������ʾ8*16��׼ASCII�ַ�����
#define SHOW_14x16CHCHAR					//������ʾ14*16���ĺ��ֹ���
#define SHOW_PICTURE						//������ʾͼƬ����

/***************** ����ѡ�� *****************/
#define OLED_SDA_Pin   H6

#define OLED_SCL_Pin   D1                                    //D0

#define OLED_DC_Pin    H5

#define OLED_RES_Pin   H7

//#define USE_CS
#ifdef USE_CS
#define OLED_CS_Pin
#endif /* USE_CS */

#define OLED_SCLH 	gpio_set(OLED_SCL_Pin, 1)//FGPIOA->PDOR |=  ((uint32)1<<PTn(OLED_SCL_Pin))
#define OLED_SCLL 	gpio_set(OLED_SCL_Pin, 0)//FGPIOA->PDOR &= ~((uint32)1<<PTn(OLED_SCL_Pin))

#define OLED_SDAH 	gpio_set(OLED_SDA_Pin, 1)//FGPIOB->PDOR |=  ((uint32)1<<PTn(OLED_SDA_Pin))
#define OLED_SDAL 	gpio_set(OLED_SDA_Pin, 0)//FGPIOB->PDOR &= ~((uint32)1<<PTn(OLED_SDA_Pin))

#define OLED_RSTH 	gpio_set(OLED_RES_Pin, 1)//FGPIOB->PDOR |=  ((uint32)1<<PTn(OLED_RES_Pin))
#define OLED_RSTL 	gpio_set(OLED_RES_Pin, 0)//FGPIOB->PDOR &= ~((uint32)1<<PTn(OLED_RES_Pin))

#define OLED_DCH  	gpio_set(OLED_DC_Pin, 1)//FGPIOB->PDOR |=  ((uint32)1<<PTn(OLED_DC_Pin))
#define OLED_DCL  	gpio_set(OLED_DC_Pin, 0)//FGPIOB->PDOR &= ~((uint32)1<<PTn(OLED_DC_Pin))

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
void OLED_PrintBMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1, const char bmp[]);
#endif
#ifdef SHOW_6X8ASCII
void OLED_P6x8Str(uint8_t x, uint8_t y, const char *ch);
void OLED_P6x8Int(uint8_t x, uint8_t y, int16_t data1, int8 set);
void OLED_P6x8Flo(uint8_t x, uint8_t y, float data1);
#endif
#ifdef SHOW_8X16ASCII
void OLED_P8x16Str(uint8_t x, uint8_t y, const char *ch);
#endif
#ifdef SHOW_14x16CHCHAR
void OLED_P14x16CHCHAR(uint8_t x, uint8_t y, const char *ch);
#endif

#endif
