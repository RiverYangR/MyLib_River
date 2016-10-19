#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "oled.h"
#include "app_param.h"
#include "key_app.h"
#include "SysTime.h"

typedef enum
{
    UNVALID,
    READ,               //暖色阅读模式
    LIGHTING,           //高亮白灯
    SCENE_RED,          //红色光
    SCENE_GREEN,        //绿色光
    SCENE_BLUE,         //蓝色光
    EXOTIC,             //情调色
    SELF_DEFINE,        //自定义颜色
    MAX_SCENE           //最大场景编号
}tSceneColor;

void DisBootRun(void);
void DisWaitData(void);
void DisMain(tSetData* set_data);
void DisLightAdjust(uint8_t scene,uint8_t light);
void DisSceneAdjust(uint8_t scene);
void DisRedAdjust(uint8_t scene,uint8_t red);
void DisGreenAdjust(uint8_t scene,uint8_t green);
void DisBlueAdjust(uint8_t scene,uint8_t blue);
void DisReadOrSave(void);
void DisSaveData(void);
void Displya_UI(void);








#endif
/******************************** end of key.h ***********************************/

