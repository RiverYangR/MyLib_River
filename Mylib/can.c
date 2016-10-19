//Rx--PA11 Tx--PA12
#include "can.h"

/**
  * @brief  配置CAN1
  * @param  None
  * @retval None
  * @Edit   joe.qiao
  */
void CAN1_ConfigInit(void)
{
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;
	CAN_InitTypeDef   can1;
	CAN_FilterInitTypeDef can_filter;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,  ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE);

	//CAN_RX
	gpio.GPIO_Pin   = GPIO_Pin_11;
	gpio.GPIO_Mode  = GPIO_Mode_IPU;			//上拉输入
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	//CAN_TX
	gpio.GPIO_Pin   = GPIO_Pin_12;
	gpio.GPIO_Mode  = GPIO_Mode_AF_PP;		//复用推挽输出
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	nvic.NVIC_IRQChannel                   = USB_HP_CAN1_TX_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority        = 0;
	nvic.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannel                   = USB_LP_CAN1_RX0_IRQn;  
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority        = 1;
	nvic.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&nvic);

    //reset register
	CAN_DeInit(CAN1);
    //config
	can1.CAN_TTCM      = DISABLE;
	can1.CAN_ABOM      = DISABLE;
	can1.CAN_AWUM      = DISABLE;
	can1.CAN_NART      = ENABLE;
	can1.CAN_RFLM      = DISABLE;				
	can1.CAN_TXFP      = DISABLE;		
	can1.CAN_Mode      = CAN_Mode_Normal;
	can1.CAN_SJW       = CAN_SJW_1tq;
	can1.CAN_BS1       = CAN_BS1_5tq;
	can1.CAN_BS2       = CAN_BS2_3tq;
	can1.CAN_Prescaler = 4;     //CAN BaudRate 36/(1+5+3)/4=1Mbps
	CAN_Init(CAN1, &can1);

	can_filter.CAN_FilterNumber         = 0;
	can_filter.CAN_FilterFIFOAssignment = 0;
	can_filter.CAN_FilterIdHigh         = 0;
	can_filter.CAN_FilterIdLow          = 0;
	can_filter.CAN_FilterMaskIdHigh     = 0;
	can_filter.CAN_FilterMaskIdLow      = 0;
	can_filter.CAN_FilterMode           = CAN_FilterMode_IdMask;
	can_filter.CAN_FilterScale          = CAN_FilterScale_32bit;
	can_filter.CAN_FilterActivation     = ENABLE;
	CAN_FilterInit(&can_filter);


	CAN_ITConfig(CAN1, CAN_IT_TME,  ENABLE);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}


/**
  * @brief  CAN1发送消息
  * @param  stdid 标准ID
  * @param  *msg  消息指针
  * @param  len   数据长度
  * @retval None
  * @Edit   River
  */
void Can_Send_StdMsg(uint16_t stdid, uint8_t* msg, uint8_t len)
{ 
  CanTxMsg TxMessage;
  //等待发送邮箱空    
    
  TxMessage.StdId = stdid;	     
  TxMessage.ExtId = 0;	            
  TxMessage.IDE   = CAN_Id_Standard; 
  TxMessage.RTR   = CAN_RTR_Data;    
  TxMessage.DLC   = len;    	     
  
  memcpy(TxMessage.Data, msg, len);
  /*
    函数原型 :void *memcpy(void *dest, const void *src, size_t n);
    功能: 从源src所指的内存地址的起始位置开始拷贝n个字节到目标dest所指的内存地址的起始位置中
  */
  CAN_Transmit(CAN1, &TxMessage); 
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{   
  CanRxMsg rx_message;
  if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
  {
    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
    CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
    
    if(rx_message.StdId == 0x101)//1号灯条数据
    {
        GetSetDataAddr()->scene = rx_message.Data[0];
        GetSetDataAddr()->light = rx_message.Data[1];
        GetSetDataAddr()->red   = rx_message.Data[2];
        GetSetDataAddr()->green = rx_message.Data[3];
        GetSetDataAddr()->blue  = rx_message.Data[4];
        
        gFlagStatus.Flag_read_successed = SET;
    }
        
      
  }
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
  if(CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
  {
     CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
  }
}







