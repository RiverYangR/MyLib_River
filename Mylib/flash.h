#ifndef __FLASH_H__
#define __FLASH_H__

extern unsigned char ID_CHOOSE_MODE;
extern unsigned char ID_PWM_MODE;    
extern unsigned char ID_SPEED_MODE;       
extern unsigned char ID_PWM_LOCATION_MODE;
extern unsigned char ID_SPEED_LOCATION_MODE; 

void Flash_Write_Motor_ID(void);
void Flash_Read_Motor_ID(void);


void Flash_Read_Motor_Dir(void);
void Flash_Write_Motor_Dir(void);
void Flash_Write_Encoder_Dir(void);
void Flash_Read_Encoder_Dir(void);

#endif
