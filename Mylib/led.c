#include "led.h"

//LED----PB12, '0' is ON,'1' is OFF

void LED_Configuration(void)
{
    GPIO_InitTypeDef gpio;   

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		
	gpio.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);
	
  //LED_RED_OFF();
	LED_GREEN_OFF();
	LED_BLUE_OFF();
}
