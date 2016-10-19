#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"

#define LED_RED_ON() GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define LED_RED_OFF() GPIO_ResetBits(GPIO,GPIO_Pin_5)
#define LED_RED_TOGGLE GPIOB->ODR ^= GPIO_Pin_5

#define LED_GREEN_ON() GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define LED_GREEN_OFF() GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define LED_GREEN_TOGGLE GPIOB->ODR ^= GPIO_Pin_7

#define LED_BLUE_ON() GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define LED_BLUE_OFF() GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define LED_BLUE_TOGGLE GPIOB->ODR ^= GPIO_Pin_6


void LED_Configuration(void);

#endif
