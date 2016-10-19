#ifndef __IIC_H__
#define __IIC_H__

#include "stm32f10x.h"

/* macro define */
#define IIC_SPEED   400000          //fast mode
#define IIC_OWN_ADDRESS     0x1A    //any value of deffrent from slave machine address

/* function statement */
static void I2C_GPIO_Config(void);
static void IIC_Mode_Config(void);
void IIC_Init_Config(void);











#endif 
/********************************** end of iic.h ******************************/


