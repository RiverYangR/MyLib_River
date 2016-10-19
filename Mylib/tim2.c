#include "tim2.h"

/**
  * @brief  ����TIM2 ʱ��Ƶ��f=40KHz
  * @param  None
  * @retval None
  * @Edit   River
  */
void TIM2_ConfigInit(void)
{
	TIM_TimeBaseInitTypeDef tim2;
	NVIC_InitTypeDef        nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
    //�ж�ʱ�䣺20ms
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
  * @brief  ����TIM2��ʱ��
  * @param  None
  * @retval None
  * @Edit   River
  */
void TIM2_Start(void)
{
	TIM_Cmd(TIM2, ENABLE);
}


/**
  * @brief  �ر�TIM2��ʱ��
  * @param  None
  * @retval None
  * @Edit   River
  */
void TIM2_Stop(void)
{
	TIM_Cmd(TIM2, DISABLE);
}


/**
  * @brief  TIM2�жϷ�����
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

