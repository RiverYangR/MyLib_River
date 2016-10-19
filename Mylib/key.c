#include "key.h"

tKeyData KeyData[KEY_NUM];
uint8_t NewStatus[KEY_NUM];
uint8_t KeyStatus[KEY_NUM] = {0,0,0,0};     //�洢����״̬

/**
  * @brief  the gpio of key configuration
  *         PA1--s1 ,PA0--s2,PA15--S3,PB3--S4;
  * @param  ��
  * @retval ��
  */
void KeyInit(void)
{
    GPIO_InitTypeDef gpio;   

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
		
    gpio.GPIO_Pin = GPIO_Pin_3;	
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0 | GPIO_Pin_15;
    GPIO_Init(GPIOA,&gpio);
    
    for(uint8_t i = 0;i<KEY_NUM;i++)
    {
        KeyData[i].KeyValue = UNVALID_KEY_VALUE;
        KeyData[i].press_time = 0;
        KeyData[i].effect_time = 0;
        KeyData[i].release_time = 0;
        KeyData[i].interval_time = 0;
    }
}

static uint8_t READ_KEY1_INPUT(void)
{
    return (GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_PIN));
}
static uint8_t READ_KEY2_INPUT(void)
{
    return (GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_PIN));
}
static uint8_t READ_KEY3_INPUT(void)
{
    return (GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_PIN));
}
static uint8_t READ_KEY4_INPUT(void)
{
    return (GPIO_ReadInputDataBit(KEY4_GPIO,KEY4_PIN));
}
/**
  * @brief  ����ɨ�����������ɨ�裬֧�ֶ̰�������ʶ�𣬶���Double_Press����֧��˫��ʶ��,����չΪ�������            
  * @param  None
  * @retval None
  * @Edit   River
  */
void ScanKey(void)
{    
    NewStatus[0] = READ_KEY1_INPUT();
    NewStatus[1] = READ_KEY2_INPUT();
    NewStatus[2] = READ_KEY3_INPUT();
    NewStatus[3] = READ_KEY4_INPUT();
    
    for(uint8_t i = 0;i<KEY_NUM;i++)
    {
        if(NewStatus[i] == RESET)//���°���
        {
            if(KeyData[i].release_time != 0)
                KeyData[i].interval_time = KeyData[i].release_time;
            KeyData[i].press_time += TIME_KEY_PERIOD;
            KeyData[i].release_time = 0;
            
            if(KeyData[i].press_time > TIME_PRESS_LONG)//���������ס���ڳ���ʶ��ʱ�䣬���ж�Ϊ����
            {
                KeyData[i].effect_time = KeyData[i].press_time;
            }
        }
        else                     //�ɿ�����
        {
            if(KeyData[i].press_time != 0)
                KeyData[i].effect_time = KeyData[i].press_time;
            if(KeyData[i].release_time < TIME_KEY_PERIOD*100)
                KeyData[i].release_time += TIME_KEY_PERIOD;
            KeyData[i].press_time = 0;  
#ifndef  MANUAL_CLEAR_KEY            
            for(uint8_t i = 0;i<KEY_NUM;i++)        //�������ɨ��ʱ����Զ������ֵ������ÿ�ΰ�����Чʱ��Ϊһ������ɨ�����ڣ�  @TIME_KEY_PERIOD
            {                                       //����������ֶ������ֵ(MANUAL_CLEAR_KEY)������Ҫ�ڳ���������ط������ֵ�������Զ������ֵ��
                if(KeyData[i].KeyValue != 0)
                    KeyData[i].KeyValue = 0;
            }
#endif        
        }
    }
    for(uint8_t i = 0;i<KEY_NUM;i++)
    {
        if(KeyData[i].effect_time>0 && KeyData[i].effect_time<TIME_PRESS_SHORT)
        {
#ifdef  Double_Press            
            if(KeyData[i].interval_time>0 && KeyData[i].interval_time<TIME_PRESS_DOUBLE_INTERVAL)
            { 
                KeyData[i].KeyValue = PRESS_DOUBLE;
                KeyData[i].interval_time = 0;
                KeyData[i].effect_time = 0;
            }            
            else
#endif                
            {
                KeyData[i].KeyValue = PRESS_SHORT;
                KeyData[i].effect_time = 0;
            }
        }
        else if(KeyData[i].effect_time>TIME_PRESS_LONG)
        {
            KeyData[i].KeyValue = PRESS_LONG;
            KeyData[i].effect_time = 0;
        }
#ifdef  Double_Press                    
        if(KeyData[i].release_time>TIME_PRESS_DOUBLE_INTERVAL)
#endif            
        {
            KeyStatus[i] = KeyData[i].KeyValue;
        }
    }
}
/**
  * @brief  manually clear key value,key value effection until to use this function.        
  * @param  _key_num��the keys number of need to clear
  * @retval None
  * @Edit   River
  */
void ClearKey(uint8_t _key_num)
{
    switch ( _key_num )
    {
        case LEFT_UP:
            KeyData[LEFT_UP].KeyValue = 0;
            break;
        case LEFT_DOWN:
            KeyData[LEFT_DOWN].KeyValue = 0;
            break;
        case RIGHT_UP:
            KeyData[RIGHT_UP].KeyValue = 0;
            break;
        case RIGHT_DOWN:
            KeyData[RIGHT_DOWN].KeyValue = 0;
            break;
        case All_KEY:
            for(uint8_t i = 0;i<KEY_NUM;i++)
            {
                KeyData[i].KeyValue = 0;
            }
            break;
    }
}


uint8_t* GetKeyStatusAdddr(void)
{
    return KeyStatus;
}
/**
  * @brief  key calm status judge of accept once key sum        
  * @param  None
  * @retval 
            -Zero: no key press,key status is calm
            -No-zero: key status is not calm
  * @Edit   River
  */
uint8_t SumKey(void)
{
    uint8_t temp_sum_key = 0;
    for(uint8_t i = 0;i<KEY_NUM;i++)
    {
        temp_sum_key+= KeyStatus[i];
    }
    return temp_sum_key;
}



/************************ end of key.c ***************************/


