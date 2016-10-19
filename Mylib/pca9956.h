#ifndef __PCA9956_H__
#define __PCA9956_H__

#include "stm32f10x.h"
#include "iic.h"
#include "delay.h"

//#include "main.h"


//the address of register
#define MODE1           0x00
#define MODE2           0x01

#define LEDOUT0         0x02
#define LEDOUT1         0x03
#define LEDOUT2         0x04
#define LEDOUT3         0x05
#define LEDOUT4         0x06
#define LEDOUT5         0x07

#define GRPPWM          0x08

#define 	      PWMALL         0x3F
#define        PWM0           0x0A
#define        PWM1           0x0B
#define        PWM2           0x0C
#define        PWM3           0x0D
#define        PWM4           0x0E
#define        PWM5           0x0F
#define        PWM6           0x10
#define        PWM7           0x11
#define        PWM8           0x12
#define        PWM9           0x13
#define        PWM10          0x14
#define        PWM11          0x15
#define        PWM12          0x16
#define        PWM13          0x17
#define        PWM14          0x18
#define        PWM15          0x19
#define        PWM16          0x1A
#define        PWM17          0x1B
#define        PWM18          0x1C
#define        PWM19          0x1D
#define        PWM20          0x1E
#define        PWM21          0x1F
#define        PWM22          0x20
#define        PWM23          0x21

#define        IREFALL         0x40

#define        OFFSET         0x3A

/**
  *@brief   auto increment register address
  */
typedef enum
{
    NoIncrement         = 0x00,     //no auto increment
    Auto_00H_3EH        = 0X04,     //auto increment 00-3eH
    Auto_Brightness     = 0x05,     //auto increment 0A-21H for PWM
    Auto_Mode1ToIref23  = 0x06,     //auto increment 00-39H for IREF
    Auto_Different      = 0x07      //auto increment 08-21H
}eAUTO_Increment;
/**
  *@brief   the address of driver chip
  *         three address pin, each have five result,can be composed of a total of 125 address
  */
typedef enum 
{
    ADDR_1      = 0x02,      //default LSB is low as a slave(write mode)
    ADDR_2      = 0x0A,
    ADDR_3      = 0x2A,
    ADDR_ALL    = 0xE0     //default address for all pca9956b device
}ADDR_DRIVER_CHIP; 
/**
  *@brief   Low power mode
  */
typedef enum
{
    SLEEPMODE = 0x01,
    NORMALMODE = 0
}eSLEEP;                 
/**
  *@brief   Device respond address in MODE1 register
  */
typedef enum
{
    SUB1 = 0x08,
    SUB2 = 0x04,
    SUB3 = 0x02,
    ALL_CALL = 0x01
}eADDRESS_RESPOND;       
/**
  *@brief  dimming and blinking group control  
  */
typedef enum
{
    Dimming  = 0x00,
    Blinking = 0x01
}eDiming_Blink_Control;      

/**
  *@brief   the value of LDR
  */
typedef enum
{
    LDRx_OFF              = 0x00,   //LED driver x is off
    LDRx_ON               = 0x01,   //led driver is fully on,the OE pin can be used control diming/blinking
    LDRx_BRIGHTNESS       = 0x02,   //This value is default,
                                    //led driver individual brightness can be controled through PWMx or PWMALL 
    LDRx_BRIGHTNESS_BLINK = 0x03    //led driver individual brightness and dimming/blinking with PWMx or GRPPWM
}eLDR;

/**
  *@brief   The group of led
  */
typedef enum
{
    RED = 1,
    GREEN = 2,
    BLUE = 3
}eLED_GROUP;                        
/**
  *@brief   each led delay time,this can reduce peak current and EMI.
  */
typedef enum
{
    DELAY_OFF = 0,                  //No delay ,all on ,all off
    DELAY1 = 0x01,                  //delay 125ns
    DELAY8 = 0x04,                  //delay 1us
    DELAY11 = 0X0B                  //delay 1.375us
}eOffset;                           

/**
  *@brief   Mode registers parameter
  */
typedef __packed struct
{
    uint8_t AutoIcrement;   //auto increment status,this param can be a value of @ eAUTO_Increment
    uint8_t Sleep;          //sleep mode,this param can be a value of @ eSLEEP
    uint8_t Address;        //Address mode,this param can be avalue of @ eADDRESS_RESPOND
    uint8_t Dmblink;        //0 is diming group control,1 is blinking group control
    uint8_t Clear;          //write 1 to clear Erro bit and flag in MODE2 and EEFLAGn register
    uint8_t Och;            //0 is outputs change on STOP command,1 is chang on ACK
}tMODE;
    
//function state ment
void WriteByteData(uint8_t dev_address,uint8_t reg_address,uint8_t data);
uint8_t ReadByteData(uint8_t dev_address);//,uint8_t reg_address);
void Device_ResetRegister(void);
void ModeConfig(uint8_t dev_address,tMODE* ModeStruct);
void PCA9956B_Init(void);
void GroupControlDimming(uint8_t group,uint8_t value);
void LedControl(void);


#endif


/************************** end of pca9956.h *******************************/


