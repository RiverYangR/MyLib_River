#include "main.h"

//encoder.INPUT_A---PB4(TIM3_CH1)
//encoder.INPUT_B---PB5(TIM3_CH2)

void Encoder_Configuration(void)
{
    GPIO_InitTypeDef gpio;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
		gpio.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOB,&gpio);

    TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12, TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
    TIM_Cmd(TIM3,ENABLE);	 
}

void Encoder_Start(void)
{
    TIM3->CNT = 0x7fff;
}

char Encoder_Dir = 0;

int Encoder_Get_CNT(void)
{
    int cnt = 0;
    static unsigned char first_flag = 0;
    
    if(first_flag == 0)
    {
        first_flag = 1;
        return 0;
    }
    
    cnt = (TIM3->CNT)-0x7fff;
    TIM3->CNT = 0x7fff;
    
    if(Encoder_Dir == 1)
        return cnt;
    else 
        return -cnt;
}
