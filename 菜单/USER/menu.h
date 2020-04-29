/****************************************************************************
 * @文件       menu.h
 * @作者       刘力铭
 * @功能			 菜单，用于调整参数
 * @注意			 该程序使用基于OLED，五个按键（对应上下左右确认），FLASH读写
							 和一个蜂鸣器（可关闭），移植时请确保初始化
 * @完成时间	 2018-4-25
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

#define MENU_MAX				10										//菜单的个数
#define SUBMENU_MAX			4											//每个菜单能储存待调整的变量数
#define MENU_SHOW(x)		((x == 0)? 0: OLED_X_MAX/2)
#define  FLASH_SAVE_RAW_MENU		(FLASH_SECTOR_NUM - 150)
#define  FLASH_SAVE_PRO_MENU		(FLASH_SECTOR_NUM - 100)

typedef struct
{
	const char* Title;													//该菜单的名字
	void* Sub_Value[SUBMENU_MAX];								//该菜单储存待调整的变量数值
	int8 Sub_Type[SUBMENU_MAX];									//该菜单储存待调整的变量数类型
	const char* Sub_Title[SUBMENU_MAX];					//该菜单储存待调整的变量数名字
	int8 Sub_Value_Num;
}Menu_TypeDef;

void Menu_Init(void);													//初始化菜单
uint8 Menu_Work(void);												//菜单工作

#endif
