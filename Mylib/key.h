#ifndef __KEY_H__
#define __kEY_H__

#include "stm32f10x.h"

#define KEY1_PIN        GPIO_Pin_1      //左上角按键
#define KEY2_PIN        GPIO_Pin_0      //左下角按键
#define KEY3_PIN        GPIO_Pin_15     //右上角按键
#define KEY4_PIN        GPIO_Pin_3      //右下角按键

#define KEY1_GPIO        GPIOA
#define KEY2_GPIO        GPIOA
#define KEY3_GPIO        GPIOA
#define KEY4_GPIO        GPIOB

#define TIME_KEY_PERIOD     20//ms
#define KEY_NUM         4
#define TIME_PRESS_SHORT    1000
#define TIME_PRESS_LONG     1000
#define TIME_PRESS_DOUBLE_INTERVAL  150

//条件宏
//#define Double_Press
//#define MANUAL_CLEAR_KEY

typedef enum
{
    UNVALID_KEY_VALUE,
    PRESS_SHORT,
    PRESS_LONG,
    PRESS_DOUBLE
}ePRESS_STATUS;

typedef __packed struct
{
    uint16_t press_time;                    //按下的时间
    uint16_t effect_time;                   //有效按键时间
    uint16_t release_time;                  //按键释放时间
    uint16_t interval_time;                 //两次按键的间隔时间
    uint8_t  KeyValue;
}tKeyData;

typedef enum
{
    LEFT_UP,
    LEFT_DOWN,
    RIGHT_UP,
    RIGHT_DOWN,
    All_KEY
}eKeyLocal;


void KeyInit(void);
void ScanKey(void);
void ClearKey(uint8_t _key_num);
uint8_t* GetKeyStatusAdddr(void);
uint8_t SumKey(void);










#endif
/******************************** end of key.h ***********************************/

