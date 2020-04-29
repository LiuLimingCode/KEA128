#ifndef __KEA128_MYIIC_H
#define __KEA128_MYIIC_H

#include "headfile.h"

#define NOP()   __NOP();__NOP();

#define SCL_GPIO		A0
#define SDA_GPIO		A1

#define SCL_H 			gpio_set(SCL_GPIO, 1)
#define SCL_L 			gpio_set(SCL_GPIO, 0)

#define SDA_H                   gpio_set(SDA_GPIO, 1)
#define SDA_L                   gpio_set(SDA_GPIO, 0)

#define READ_SDA                gpio_get(SDA_GPIO)

#define SET_SDA_IN              gpio_init(SDA_GPIO, GPI, 1)
#define SET_SDA_OUT             gpio_init(SDA_GPIO, GPO, 0)

#define SET_SCL_IN              gpio_ddr(SCL_GPIO, GPI)
#define SET_SCL_OUT             gpio_ddr(SCL_GPIO, GPO)

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

void IIC_Init(void);
uint8_t IIC_ReadRegister(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t *data);
uint8_t IIC_ReadRegisterLen(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t len, uint8_t *buf);
uint8_t IIC_WriteRegister(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data);
uint8_t IIC_WriteRegisterLen(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t len, uint8_t *buf);

#endif
