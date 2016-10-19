/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   usartӦ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "usart1.h"
#include "main.h"

uint8_t SendBuff[SENDBUFF_SIZE];
uint8_t RecieveBuff[REBUFF_SIZE]={0};

 /**
  * @brief  USART1 GPIO ����,����ģʽ���á�9600 8-N-1
  * @param  ��
  * @retval ��
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
	
	/* ʹ�ܴ���1�����ж� */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
}

/// ����USART1�����ж�
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
	
		/*����DMAʱ��*/
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

		/*����DMAԴ���������ݼĴ�����ַ*/
		DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;	   

		/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;

		/*���򣺴��ڴ浽����*/		
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	

		/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
		DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;

		/*�����ַ����*/	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 

		/*�ڴ��ַ����*/
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	

		/*�������ݵ�λ*/	
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

		/*�ڴ����ݵ�λ 8bit*/
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 

		/*DMAģʽ������ѭ��*/
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
		//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 

		/*���ȼ�����*/	
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  

		/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

		/*����DMA1��4ͨ��*/		   
		DMA_Init(DMA1_Channel4, &DMA_InitStructure); 	   
		
		/*��ʱ��ʹ��DMA*/
		DMA_Cmd (DMA1_Channel4,DISABLE);					
		//DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //����DMA������ɺ�����ж�
}


/// �ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/// �ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

//�����ⲿ���������ڱ�ʾ���ݽ����Ƿ���ɣ�0��ʾδ��ɣ�1��ʾ���
unsigned char RecieveEndFlag=0;
uint8_t REBUFF_SIZE_I=0;
// uint16_t CHECK=0;
void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
			Time2_Time=0;	
			RecieveBuff[REBUFF_SIZE_I]= (unsigned char)USART_ReceiveData(USART1);
		//ָ�����������ݽ������
			if(REBUFF_SIZE_I==(REBUFF_SIZE-1))
			{
					RecieveEndFlag=1;
				
			}	
			//���ջ�����������ѭ������
			REBUFF_SIZE_I=((REBUFF_SIZE_I+1)>=REBUFF_SIZE)?(0):(REBUFF_SIZE_I+1);

	} 
	
}

//�����ر���
uint8_t robot_type[10];//ս������
uint16_t robot_HP[10];//ս��Ѫ������
uint16_t robot_R_HP_current=0;//�췽ս����ǰѪ��
uint16_t robot_B_HP_current=0;//����ս����ǰѪ��
uint16_t base_R_HP=0;//�췽����Ѫ��
uint16_t base_B_HP=0;//��������Ѫ��
uint8_t  Decoding_success=0;//����ɹ���־��1��ʾ����ɹ���0��ʾδ�����
void Decoding(void)//Э����
{
		uint8_t  i;
		uint8_t  j=20;//ս��Ѫ����ʼ�ֽ�
		for(i=0;i<10;i++)//ս��Ѫ��
		{
				robot_HP [i]= ((RecieveBuff [j+1]<<8)&0xff00)|RecieveBuff [j];//�ȷ��͵�λ���ݣ�20-39
				if(robot_HP[i]>2000)
				{
					robot_HP[i]=0;
				}
				j+=2;
		}
		//��ӻ���Ѫ��
		base_R_HP = ((RecieveBuff [41]<<8)&0xff00)|RecieveBuff [40];
		if(base_R_HP>BASE_R_HP_MAX){base_R_HP=0;}
		//���ӻ���Ѫ��
		base_B_HP = ((RecieveBuff [43]<<8)&0xff00)|RecieveBuff [42];
		if(base_B_HP>BASE_B_HP_MAX){base_R_HP=0;}



		
 		//�������
#if TEAM_LABEL==RED_LABEL 
 		base_R_HP = (base_R_HP*100) / BASE_R_HP_MAX ;//����Ѫ���ٷֱ�
		robot_R_HP_current=0;
		for(i=0;i<5;i++)
		{
			robot_R_HP_current+=robot_HP[i];//�췽ս����ǰѪ��ֵ
		}
		robot_R_HP_current =(robot_R_HP_current * 100)/ROBOT_R_HP_MAX ;//16λ
		base_R_HP=min(base_R_HP ,robot_R_HP_current );//�˴�������췽������ս����ǰ����СѪ��
// 		base_R_HP=0;
#elif TEAM_LABEL==BLUE_LABEL 
		base_B_HP = (base_B_HP*100) / BASE_B_HP_MAX ;//����Ѫ���ٷֱ�
		robot_B_HP_current=0;
		for(i=5;i<10;i++)
		{
			robot_B_HP_current+=robot_HP[i];//����ս����ǰѪ��ֵ
		}
		robot_B_HP_current =(robot_B_HP_current * 100)/ROBOT_B_HP_MAX ;//16λ
		base_B_HP=min(base_B_HP ,robot_B_HP_current );//�˴����������������ս����ǰ����СѪ��
#endif
		
		//����ɹ�
		Decoding_success = 1;	
}


/*********************************************END OF FILE**********************/
