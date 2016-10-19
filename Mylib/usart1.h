#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

#define USART1_DR_Base  0x40013804		// 0x40013800 + 0x04 = 0x40013804
#define SENDBUFF_SIZE   5000
#define REBUFF_SIZE     44						//���ݰ�һ��44�ֽ�
// #define ROBOT_HP_SIZE   25						//ս��Ѫ����ʼ�ֽ�




//�ⲿ��������
//�����ⲿ���������ڱ�ʾ���ݽ����Ƿ���ɣ�0��ʾδ��ɣ�1��ʾ���
extern unsigned char RecieveEndFlag;
//����ɹ���־��1��ʾ����ɹ���0��ʾδ�����
extern uint8_t  Decoding_success;
extern uint8_t robot_type[10];//�������
extern uint16_t robot_HP[10];//ս��Ѫ��
extern uint16_t base_R_HP;//�췽����Ѫ��
extern uint16_t base_B_HP;//��������Ѫ��
extern uint16_t robot_R_HP_current;//�췽ս����ǰѪ��
extern uint16_t robot_B_HP_current;//����ս����ǰѪ��


extern uint8_t RecieveBuff[REBUFF_SIZE];//�������ݻ�����





void USART1_Config(void);
void NVIC_Configuration(void);
void USART1_DMA_Config(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);

void Decoding(void);

#endif /* __USART1_H */
