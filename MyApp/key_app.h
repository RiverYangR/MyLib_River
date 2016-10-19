#ifndef __KEY_APP_H__
#define __KEY_APP_H__

#include "app_param.h"
#include "display.h"

typedef __packed struct
{
    uint8_t KeySwitchCnt;
}tKeyCnt;

typedef enum
{
    MAIN_DIS,
    LIGHT_DIS,
    SCENE_DIS,
    RED_DIS,
    GREEN_DIS,
    BLUE_DIS,
    READ_SAVE_DIS
}eDIS_NUM;

tKeyCnt* GetKeyCntAddr(void);
void ReadKeyStatus(void);
tSetData* GetSetDataAddr(void);
uint8_t CalmStatusJudge(void);

#endif
/******************************** end of key_app.h ***********************************/

