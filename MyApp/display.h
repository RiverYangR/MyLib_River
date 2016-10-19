#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "oled.h"
#include "app_param.h"
#include "key_app.h"
#include "SysTime.h"

typedef enum
{
    UNVALID,
    READ,               //ůɫ�Ķ�ģʽ
    LIGHTING,           //�����׵�
    SCENE_RED,          //��ɫ��
    SCENE_GREEN,        //��ɫ��
    SCENE_BLUE,         //��ɫ��
    EXOTIC,             //���ɫ
    SELF_DEFINE,        //�Զ�����ɫ
    MAX_SCENE           //��󳡾����
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

