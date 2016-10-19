#ifndef __USART_H__
#define __USART_H__

extern int PWM_data;             
extern int Speed_data;                
extern int PWM_Location_PWM_data;
extern int PWM_Location_Destination_data;
extern int Speed_Location_Speed_data;
extern int Speed_Location_Destination_data;

extern unsigned char First_Start;
extern unsigned char First_Enter_Usart_flag;

void USART2_Configuration(void);
void USART2_SendChar(char b);

#endif
