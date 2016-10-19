#ifndef __OLED_H__
#define __OLED_H__

#include "stm32f10x.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "delay.h"

typedef __packed struct
{
    uint8_t  x;             //x coordinates : 0~128;
    uint8_t  y;             //y coordinates : 0~7;
    uint8_t* pbuf;          //the pointer of buf
    uint8_t  bmpLength;     //the BMP picture horizontal length:0~128
    uint8_t  bmpWidth;      //the BMP picture vertical width:0~64
    uint8_t  displayMode;   //the display mode : @eOLED_DISPLAY_MODE
}tBMP_Info;

typedef enum
{
    OLED_DISPLAY_NORMAL,
    OLED_DISPLAY_INVERSE,       //显示的当前字符反白选中
    OLED_DISPLAY_ROW_SELECT     //显示的这行反白选中
}eOLED_DISPLAY_MODE;
typedef enum
{
    RightHorizontal = 0x00,
    LeftHorizontal = 0x01
}eOledScrollDirection;          //Oled Scroll Direction Of Continuous Horizontal command
typedef enum
{
    ScrollFrame5    = 0x00,
    ScrollFrame64   = 0x01,
    ScrollFrame128  = 0x02,
    ScrollFrame256  = 0x03,
    ScrollFrame3    = 0x04,
    ScrollFrame4    = 0x05,
    ScrollFrame25   = 0x06,
    ScrollFrame2    = 0x07   
}eOledScrollIntervalFrames;

    
void OLED_Configuration(void); 
void OLED_Print_CharString(unsigned int x, unsigned char y, char* ch,uint8_t mode);
void OLED_Printf(unsigned char x,unsigned char y,uint8_t mode,const char *fmt,...);
void OledPrintfBMP(tBMP_Info* bmp_info);
void OLED_Clear(void);
void SetHorizontalCcrolling(uint8_t direction,uint8_t start_page,uint8_t end_page,uint8_t gear_frame);

#endif 
/************************************ end of oled.h *************************************************/

