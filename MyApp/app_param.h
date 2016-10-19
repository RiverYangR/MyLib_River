#ifndef __APP_PARAM_H__
#define __APP_PARAM_H__

#include "stm32f10x.h"

typedef __packed struct 
{
    uint8_t Flag_set;
    uint8_t Flag_UI_Changed;
    uint8_t Flag_Exe;
    uint8_t Flag_read_save;
    uint8_t Flag_read_successed;
}tFlagStatus;

typedef __packed struct
{
    uint8_t scene;
    uint8_t light;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}tSetData;

typedef enum
{
    FLAG_READ_SAVE_UNVALID,
    FLAG_READ,
    FLAG_SAVE
}eFlagSaveRead;

extern tFlagStatus gFlagStatus;



#endif
/******************************** end of app_param.h ***********************************/

