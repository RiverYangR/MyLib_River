#ifndef TIME2_BASE_H
#define TIME2_BASE_H

#include "stm32f10x.h"

extern uint16_t Time2_Time;
extern uint16_t Time2_Wait_Time;



void TIM2_NVIC_Configuration(void);
void TIM2_Configuration(void);
void TIME2_DELAY(uint8_t m);//��ʱ����ʱ����


#endif	

/* TIME2_BASE_H */

