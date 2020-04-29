/****************************************************************************
 * @�ļ�       menu.h
 * @����       ������
 * @����			 �˵������ڵ�������
 * @ע��			 �ó���ʹ�û���OLED�������������Ӧ��������ȷ�ϣ���FLASH��д
							 ��һ�����������ɹرգ�����ֲʱ��ȷ����ʼ��
 * @���ʱ��	 2018-4-25
****************************************************************************/
#ifndef __MENU_H
#define __MENU_H

#include "headfile.h"
#include "KEA128_oled.h"

#define BUTTON_UP				KU
#define BUTTON_DOWN			KD
#define BUTTON_LEFT			KL
#define BUTTON_RIGHT		KR
#define BUTTON_CONFIRM	KC
#define BUZZER					BUZZ

#define MENU_MAX				10										//�˵��ĸ���
#define SUBMENU_MAX			4											//ÿ���˵��ܴ���������ı�����
#define MENU_SHOW(x)		((x == 0)? 0: OLED_X_MAX/2)
#define  FLASH_SAVE_RAW_MENU		(FLASH_SECTOR_NUM - 150)
#define  FLASH_SAVE_PRO_MENU		(FLASH_SECTOR_NUM - 100)

typedef struct
{
	const char* Title;													//�ò˵�������
	void* Sub_Value[SUBMENU_MAX];								//�ò˵�����������ı�����ֵ
	int8 Sub_Type[SUBMENU_MAX];									//�ò˵�����������ı���������
	const char* Sub_Title[SUBMENU_MAX];					//�ò˵�����������ı���������
	int8 Sub_Value_Num;
}Menu_TypeDef;

void Menu_Init(void);													//��ʼ���˵�
uint8 Menu_Work(void);												//�˵�����

#endif
