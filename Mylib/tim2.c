#include "tim2.h"

/**
  * @brief  配置TIM2 时钟频率f=40KHz
  * @param  None
  * @retval None
  * @Edit   River
  */
void TIM2_ConfigInit(void)
{
	TIM_TimeBaseInitTypeDef tim2;
	NVIC_InitTypeDef        nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
    //中断时间：20ms
	tim2.TIM_ClockDivision = TIM_CKD_DIV1;
	tim2.TIM_CounterMode   = TIM_CounterMode_Down;
	tim2.TIM_Period        = 200-1;
	tim2.TIM_Prescaler     = 7200-1;   //f_tim2 = 10KHz
	tim2.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &tim2);	
	
	nvic.NVIC_IRQChannel                   = TIM2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority        = 0;
	nvic.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&nvic);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	
}

/**
  * @brief  开启TIM2定时器
  * @param  None
  * @retval None
  * @Edit   River
  */
void TIM2_Start(void)
{
	TIM_Cmd(TIM2, ENABLE);
}


/**
  * @brief  关闭TIM2定时器
  * @param  None
  * @retval None
  * @Edit   River
  */
void TIM2_Stop(void)
{
	TIM_Cmd(TIM2, DISABLE);
}


/**
  * @brief  TIM2中断服务函数
  * @param  None
  * @retval None
  * @Edit   River
  */
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)==SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        ScanKey();		
	}
}
/*********************************** end of tim2.c ******************************/

