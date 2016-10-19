/**
  ******************************************************************************
  * @file    SysTime.c
  * @author   copy
  * @version  V1.0
  * @date    07-2016
  * @brief   This file is softeware timer
  ******************************************************************************
  * @attention
  * Used tim3 for soft timer
  ******************************************************************************
  */
#include "SysTime.h"

/********************** Private Varible ********************/
static tTimerElemt     SoftTimer[TIMER_ELEMENT_NUM_MAX];

/********************** Macro Function ********************/
#define INT_STATE uint32_t
#define MASTER_INT_STATE_GET()           __get_PRIMASK()
#define MASTER_INT_ENABLE()              do{__enable_irq();}while(0)
#define MASTER_INT_DISABLE()             do{__disable_irq();}while(0)
#define MASTER_INT_RESTORE(x)                do{__set_PRIMASK(x);}while(0)

#define CRITICAL_SETCION_ENTER()                        \
do                                                      \
{                                                       \
    INT_STATE   cpu_state = MASTER_INT_STATE_GET();     \
    MASTER_INT_DISABLE();
    
#define CRITICAL_SETCION_EXIT()                         \
    MASTER_INT_RESTORE(cpu_state);                      \
}while(0)

/********************** function implement ********************/


static void HardTimer_Init(void)
{
    TIM_TimeBaseInitTypeDef     Tim_TimeBaseStructure;
    NVIC_InitTypeDef            NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
    //1ms
    Tim_TimeBaseStructure.TIM_Period = 1000 - 1;
    Tim_TimeBaseStructure.TIM_Prescaler = 72 - 1;
    Tim_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    Tim_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3,&Tim_TimeBaseStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM3,ENABLE);    
}

static void HardTimer_Destroy(void)
{
    NVIC_InitTypeDef            NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,DISABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);  
}

uint16_t SoftTimer_Init(void)
{
    uint8_t i = 0;
    
    //clear all elements
    for(i = 1;i<TIMER_ELEMENT_NUM_MAX;i++)
    {
        SoftTimer[i].handle = 0;
        SoftTimer[i].delay = 0;
    }
    
    HardTimer_Init();
    
    return (SOFT_TIMER_SUCCESS);
}

uint16_t SoftTimer_Destroy(void)
{
    uint8_t i = 0;
    
    //clear all elements
    for(i = 1;i<TIMER_ELEMENT_NUM_MAX;i++)
    {
        SoftTimer[i].handle = 0;
        SoftTimer[i].delay = 0;
    }
    
    HardTimer_Destroy();
    
    return (SOFT_TIMER_SUCCESS);
    
}

/**
  * @brief  Request Timer resource        
  * @param  Tick is time elipse value
  * @retval 
            -Zero               Operate Failure,No Timer Available
            -Non-Zero           Valid Timer Handle
  * @Edit   
  */
uint16_t SoftTimer_Req(uint32_t Tick)
{
    uint8_t i = 0;
    
    for(i =  1;i < TIMER_ELEMENT_NUM_MAX;i++)\
    {
        if(SoftTimer[i].handle == 0)
        {
            CRITICAL_SETCION_ENTER();
            
            SoftTimer[i].handle = i;
            SoftTimer[i].delay  = Tick;
            
            CRITICAL_SETCION_EXIT();
            
            return (i);
        }
    }
    
    return (0);
}

uint16_t SoftTimer_Update(uint32_t Id,uint32_t Tick)
{
    uint8_t  i = 0;
    
    for(i =  1;i < TIMER_ELEMENT_NUM_MAX;i++)
    {
        if(SoftTimer[i].handle == Id)
        {
            CRITICAL_SETCION_ENTER();
            SoftTimer[i].delay = Tick;
            CRITICAL_SETCION_EXIT();
            //Success
            return (0);
        }
    } 

    //Failure
    return (1);
}
uint16_t SoftTimer_SetInvalid(uint32_t Id)
{
    uint8_t i = 0;
    for(i = 1;i < TIMER_ELEMENT_NUM_MAX;i++)
    {
        if(SoftTimer[i].handle == Id)
        {
            CRITICAL_SETCION_ENTER();
            
            SoftTimer[i].handle    = 0;
            SoftTimer[i].delay  = 0;
            
            CRITICAL_SETCION_EXIT();
            
            //Success
            return (0);
        }
    }
    
    //Failure
    return(1);
    
    
    
}
/**
  * @brief  Status of Timer detect        
  * @param  Handle is timer ID
  * @retval 
            -SOFT_TIMER_ING                      1           //!< Timer Counting
            -SOFT_TIMER_TIMEOUT                  2           //!< Timer Timeout
            -SOFT_TIMER_ERR                      3           //!< Timer is not exist 
  * @Edit   River
  */
uint16_t SoftTimer_Check(uint16_t Handle)
{
    uint16_t  retval = SOFT_TIMER_ERR;
    
    CRITICAL_SETCION_ENTER();
    if(SoftTimer[Handle].handle == Handle)
    {
        if(SoftTimer[Handle].delay)
        {
            retval = SOFT_TIMER_ING;
        }
        else
        {
            retval = SOFT_TIMER_TIMEOUT;
        }
    }
    CRITICAL_SETCION_EXIT();
    
    return (retval);   
}


void Timer_ISR_Hook(void)
{
    uint8_t i = 0;
    for(i = 1;i < TIMER_ELEMENT_NUM_MAX;i++)
    {
        if(SoftTimer[i].handle != 0)
        {
            if(SoftTimer[i].delay)          //等于0时，停止自减
            {
                SoftTimer[i].delay--;
            }
        }
    }
}


void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
        Timer_ISR_Hook();
    } 
}






/******************************** end of SysTime.c ***********************************/

