/**
  ***************************************************************
  * @file       iic.c 
  * @author     River
  * @version    v1.0
  * @date       2016-1-25
  * @brief      stm32f103 hardware iic
               PB6-I2C1_SCL，PB7-I2C1_SDA
  **************************************************************/
#include "iic.h"

/**
  * @brief  I2C1 I/O mode config
  * @param  None
  * @retval None
  */
static void IIC_GPIO_Config(void)
{
     GPIO_InitTypeDef  GPIO_InitStructure; 

	/* Enable clock*/
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  
    
     /* PB6-I2C1_SCL，PB7-I2C1_SDA*/
     GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_6 | GPIO_Pin_7;
     GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_AF_OD;	      //Open drain output                                                                 
     GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  I2C1 work mode config
  * @param  无
  * @retval 无
  */
static void IIC_Mode_Config(void)
{
    I2C_InitTypeDef IIC_InitStrcture;

    IIC_InitStrcture.I2C_ClockSpeed             = IIC_SPEED;
    IIC_InitStrcture.I2C_Mode                   = I2C_Mode_I2C;
    IIC_InitStrcture.I2C_DutyCycle              = I2C_DutyCycle_2;
    IIC_InitStrcture.I2C_OwnAddress1            = IIC_OWN_ADDRESS;
    IIC_InitStrcture.I2C_Ack                    = I2C_Ack_Enable;
    IIC_InitStrcture.I2C_AcknowledgedAddress    = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1,&IIC_InitStrcture);

    /* enable IIC */
    I2C_Cmd(I2C1,ENABLE);
}


/**
  * @brief  I2C1 Initialization configuration
  * @param  无
  * @retval 无
  */
void IIC_Init_Config(void)
{
    IIC_GPIO_Config();
    IIC_Mode_Config();
}



/********************************* end of iic.c *****************************/


