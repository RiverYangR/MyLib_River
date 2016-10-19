#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

#define USART1_DR_Base  0x40013804		// 0x40013800 + 0x04 = 0x40013804
#define SENDBUFF_SIZE   5000
#define REBUFF_SIZE     44						//数据包一共44字节
// #define ROBOT_HP_SIZE   25						//战车血量开始字节




//外部变量声明
//定义外部变量，用于表示数据接收是否完成，0表示未完成，1表示完成
extern unsigned char RecieveEndFlag;
//解包成功标志，1表示解包成功，0表示未解包；
extern uint8_t  Decoding_success;
extern uint8_t robot_type[10];//解包数据
extern uint16_t robot_HP[10];//战车血量
extern uint16_t base_R_HP;//红方基地血量
extern uint16_t base_B_HP;//蓝方基地血量
extern uint16_t robot_R_HP_current;//红方战车当前血量
extern uint16_t robot_B_HP_current;//蓝方战车当前血量


extern uint8_t RecieveBuff[REBUFF_SIZE];//接收数据缓冲区





void USART1_Config(void);
void NVIC_Configuration(void);
void USART1_DMA_Config(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);

void Decoding(void);

#endif /* __USART1_H */
