/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   usart应用bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "usart1.h"
#include "main.h"

uint8_t SendBuff[SENDBUFF_SIZE];
uint8_t RecieveBuff[REBUFF_SIZE]={0};

 /**
  * @brief  USART1 GPIO 配置,工作模式配置。9600 8-N-1
  * @param  无
  * @retval 无
  */
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;//| USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/* 使能串口1接收中断 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
}

/// 配置USART1接收中断
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART1_DMA_Config(void)
{
		DMA_InitTypeDef DMA_InitStructure;
	
		/*开启DMA时钟*/
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

		/*设置DMA源：串口数据寄存器地址*/
		DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;	   

		/*内存地址(要传输的变量的指针)*/
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;

		/*方向：从内存到外设*/		
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	

		/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
		DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;

		/*外设地址不增*/	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 

		/*内存地址自增*/
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	

		/*外设数据单位*/	
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

		/*内存数据单位 8bit*/
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 

		/*DMA模式：不断循环*/
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
		//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 

		/*优先级：中*/	
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  

		/*禁止内存到内存的传输	*/
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

		/*配置DMA1的4通道*/		   
		DMA_Init(DMA1_Channel4, &DMA_InitStructure); 	   
		
		/*暂时不使能DMA*/
		DMA_Cmd (DMA1_Channel4,DISABLE);					
		//DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //配置DMA发送完成后产生中断
}


/// 重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/// 重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

//定义外部变量，用于表示数据接收是否完成，0表示未完成，1表示完成
unsigned char RecieveEndFlag=0;
uint8_t REBUFF_SIZE_I=0;
// uint16_t CHECK=0;
void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
			Time2_Time=0;	
			RecieveBuff[REBUFF_SIZE_I]= (unsigned char)USART_ReceiveData(USART1);
		//指定数量的数据接收完成
			if(REBUFF_SIZE_I==(REBUFF_SIZE-1))
			{
					RecieveEndFlag=1;
				
			}	
			//接收缓冲区计数器循环计数
			REBUFF_SIZE_I=((REBUFF_SIZE_I+1)>=REBUFF_SIZE)?(0):(REBUFF_SIZE_I+1);

	} 
	
}

//解包相关变量
uint8_t robot_type[10];//战车类型
uint16_t robot_HP[10];//战车血量数组
uint16_t robot_R_HP_current=0;//红方战车当前血量
uint16_t robot_B_HP_current=0;//蓝方战车当前血量
uint16_t base_R_HP=0;//红方基地血量
uint16_t base_B_HP=0;//蓝方基地血量
uint8_t  Decoding_success=0;//解包成功标志，1表示解包成功，0表示未解包；
void Decoding(void)//协议解包
{
		uint8_t  i;
		uint8_t  j=20;//战车血量开始字节
		for(i=0;i<10;i++)//战车血量
		{
				robot_HP [i]= ((RecieveBuff [j+1]<<8)&0xff00)|RecieveBuff [j];//先发送低位数据；20-39
				if(robot_HP[i]>2000)
				{
					robot_HP[i]=0;
				}
				j+=2;
		}
		//红队基地血量
		base_R_HP = ((RecieveBuff [41]<<8)&0xff00)|RecieveBuff [40];
		if(base_R_HP>BASE_R_HP_MAX){base_R_HP=0;}
		//蓝队基地血量
		base_B_HP = ((RecieveBuff [43]<<8)&0xff00)|RecieveBuff [42];
		if(base_B_HP>BASE_B_HP_MAX){base_R_HP=0;}



		
 		//解包处理
#if TEAM_LABEL==RED_LABEL 
 		base_R_HP = (base_R_HP*100) / BASE_R_HP_MAX ;//基地血量百分比
		robot_R_HP_current=0;
		for(i=0;i<5;i++)
		{
			robot_R_HP_current+=robot_HP[i];//红方战车当前血量值
		}
		robot_R_HP_current =(robot_R_HP_current * 100)/ROBOT_R_HP_MAX ;//16位
		base_R_HP=min(base_R_HP ,robot_R_HP_current );//此处已算出红方基地与战车当前的最小血量
// 		base_R_HP=0;
#elif TEAM_LABEL==BLUE_LABEL 
		base_B_HP = (base_B_HP*100) / BASE_B_HP_MAX ;//基地血量百分比
		robot_B_HP_current=0;
		for(i=5;i<10;i++)
		{
			robot_B_HP_current+=robot_HP[i];//蓝方战车当前血量值
		}
		robot_B_HP_current =(robot_B_HP_current * 100)/ROBOT_B_HP_MAX ;//16位
		base_B_HP=min(base_B_HP ,robot_B_HP_current );//此处已算出蓝方基地与战车当前的最小血量
#endif
		
		//解包成功
		Decoding_success = 1;	
}


/*********************************************END OF FILE**********************/
