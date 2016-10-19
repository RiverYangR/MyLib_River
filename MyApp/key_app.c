#include "key_app.h"
#include "key.h"


tKeyCnt KeyCnt;
tSetData SetData;


void ReadKeyStatus(void)
{
    if(GetKeyStatusAdddr()[LEFT_UP] == PRESS_LONG && KeyCnt.KeySwitchCnt == 0)
    {
        gFlagStatus.Flag_set = SET;
        KeyCnt.KeySwitchCnt = 1;
        gFlagStatus.Flag_UI_Changed = SET;
    }
    
    if(GetKeyStatusAdddr()[LEFT_DOWN] == PRESS_LONG && KeyCnt.KeySwitchCnt != 0)
    {
        gFlagStatus.Flag_set = RESET;
        KeyCnt.KeySwitchCnt = 0;
        gFlagStatus.Flag_UI_Changed = SET;
    }
    
    if(gFlagStatus.Flag_set == SET)
    {
        if(GetKeyStatusAdddr()[LEFT_UP] == PRESS_SHORT)
        {
            KeyCnt.KeySwitchCnt++;
            if(KeyCnt.KeySwitchCnt > READ_SAVE_DIS)
            {
                KeyCnt.KeySwitchCnt = 1;
            }
            gFlagStatus.Flag_UI_Changed = SET;            
        }
        if(GetKeyStatusAdddr()[LEFT_DOWN] == PRESS_SHORT)
        {
            KeyCnt.KeySwitchCnt--;
            if(KeyCnt.KeySwitchCnt == 0)
            {
                KeyCnt.KeySwitchCnt = 6;
            }
            gFlagStatus.Flag_UI_Changed = SET;
        }
        switch ( KeyCnt.KeySwitchCnt )
        {
            case LIGHT_DIS:     //adjust lightness
                if(GetKeyStatusAdddr()[RIGHT_UP] == PRESS_SHORT || GetKeyStatusAdddr()[RIGHT_UP] == PRESS_LONG)
                {
                    SetData.light++;
                    gFlagStatus.Flag_UI_Changed = SET;
                    gFlagStatus.Flag_Exe = SET;
                }
                if(GetKeyStatusAdddr()[RIGHT_DOWN] == PRESS_SHORT || GetKeyStatusAdddr()[RIGHT_DOWN] == PRESS_LONG)
                {
                    SetData.light--;
                    gFlagStatus.Flag_UI_Changed = SET;
                    gFlagStatus.Flag_Exe = SET;
                }                
                
                break;
            case SCENE_DIS:         //switch scene
                if(GetKeyStatusAdddr()[RIGHT_UP] == PRESS_SHORT || GetKeyStatusAdddr()[RIGHT_UP] == PRESS_LONG)
                {
                    SetData.scene = (SetData.scene>=MAX_SCENE-1) ? (1) : (++SetData.scene) ;
                    gFlagStatus.Flag_UI_Changed = SET;
                    gFlagStatus.Flag_Exe = SET;
                }
                if(GetKeyStatusAdddr()[RIGHT_DOWN] == PRESS_SHORT || GetKeyStatusAdddr()[RIGHT_DOWN] == PRESS_LONG)
                {
                    SetData.scene = (SetData.scene <= 1) ? (MAX_SCENE-1) : (--SetData.scene) ;
                    gFlagStatus.Flag_UI_Changed = SET;
                    gFlagStatus.Flag_Exe = SET;
                }
                break;
            case RED_DIS:         //set red color
                if(GetKeyStatusAdddr()[RIGHT_UP] == PRESS_SHORT || GetKeyStatusAdddr()[RIGHT_UP] == PRESS_LONG)
                {
                    SetData.red++;
                    gFlagStatus.Flag_UI_Changed = SET;
                    gFlagStatus.Flag_Exe = SET;
                }
                if(GetKeyStatusAdddr()[RIGHT_DOWN] == PRESS_SHORT || GetKeyStatusAdddr()[RIGHT_DOWN] == PRESS_LONG)
                {
                    SetData.red--;
                    gFlagStatus.Flag_UI_Changed = SET;
                    gFlagStatus.Flag_Exe = SET;
                }
                break;
            case GREEN_DIS:         //set green color
                if(GetKeyStatusAdddr()[RIGHT_UP] == PRESS_SHORT || GetKeyStatusAdddr()[RIGHT_UP] == PRESS_LONG)
                {
                    SetData.green++;
                    gFlagStatus.Flag_UI_Changed = SET;
                    gFlagStatus.Flag_Exe = SET;
                }
                if(GetKeyStatusAdddr()[RIGHT_DOWN] == PRESS_SHORT || GetKeyStatusAdddr()[RIGHT_DOWN] == PRESS_LONG)
                {
                    SetData.green--;
                    gFlagStatus.Flag_UI_Changed = SET;
                    gFlagStatus.Flag_Exe = SET;
                }
                break;
            case BLUE_DIS:         //set blue color
                if(GetKeyStatusAdddr()[RIGHT_UP] == PRESS_SHORT || GetKeyStatusAdddr()[RIGHT_UP] == PRESS_LONG)
                {
                    SetData.blue++;
                    gFlagStatus.Flag_UI_Changed = SET;
                    gFlagStatus.Flag_Exe = SET;
                }
                if(GetKeyStatusAdddr()[RIGHT_DOWN] == PRESS_SHORT || GetKeyStatusAdddr()[RIGHT_DOWN] == PRESS_LONG)
                {
                    SetData.blue--;
                    gFlagStatus.Flag_UI_Changed = SET;
                    gFlagStatus.Flag_Exe = SET;
                }
                break;
            case READ_SAVE_DIS:
                if(GetKeyStatusAdddr()[RIGHT_UP] == PRESS_SHORT)
                {
                    //read param
                    gFlagStatus.Flag_read_save = FLAG_READ;
                    gFlagStatus.Flag_UI_Changed = SET;
                    gFlagStatus.Flag_Exe = SET;
                }
                if(GetKeyStatusAdddr()[RIGHT_DOWN] == PRESS_SHORT)
                {
                    //save param
                    gFlagStatus.Flag_read_save = FLAG_SAVE;
                    gFlagStatus.Flag_UI_Changed = SET;
                    gFlagStatus.Flag_Exe = SET;
                }
                break;
            default:
                break;
        }        
    }
    else
    {
        KeyCnt.KeySwitchCnt = 0;        
    }
}

tKeyCnt* GetKeyCntAddr(void)
{
    return &KeyCnt;
}
tSetData* GetSetDataAddr(void)
{
    return &SetData;
}

/**
  * @brief  key calm status judge of accept once key sum        
  * @param  None
  * @retval 
            -Zero: no key press,key status is calm
            -No-zero: key status is not calm
  * @Edit   River
  */
uint8_t CalmStatusJudge(void)
{
   return SumKey();
}





/******************************** end of key_app.c ***********************************/

