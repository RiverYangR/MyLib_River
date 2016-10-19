#include "display.h"
#include "dis_data.h"


tBMP_Info aBMP_INFO ;
tTimerElemt TimerDisplay = {0,15000};



void DisBootRun(void)
{
    aBMP_INFO.x = 0;
    aBMP_INFO.y = 0;
    aBMP_INFO.pbuf = BMP3;
    aBMP_INFO.bmpLength = 128;
    aBMP_INFO.bmpWidth = 64;
    aBMP_INFO.displayMode = OLED_DISPLAY_INVERSE;
    OledPrintfBMP(&aBMP_INFO); 
}

void DisWaitData(void)
{
    char wait_data[] = "Wating Data ...";
    OLED_Clear();
    OLED_Print_CharString(0,3,wait_data,OLED_DISPLAY_NORMAL);
}
static void DisScene(uint8_t scene)
{
    switch ( scene )
    {
        case READ:
            OLED_Print_CharString(0,2,"      READ",OLED_DISPLAY_NORMAL);
            break;
        case LIGHTING:
            OLED_Print_CharString(0,2,"    LIGHTING",OLED_DISPLAY_NORMAL);        
            break;
        case SCENE_RED:
            OLED_Print_CharString(0,2,"    SCENE_RED",OLED_DISPLAY_NORMAL);        
            break;
        case SCENE_GREEN:
            OLED_Print_CharString(0,2,"  SCENE_GREEN",OLED_DISPLAY_NORMAL);        
            break;
        case SCENE_BLUE:
            OLED_Print_CharString(0,2,"   SCENE_BLUE",OLED_DISPLAY_NORMAL);        
            break;
        case EXOTIC:
            OLED_Print_CharString(0,2,"      EXOTIC",OLED_DISPLAY_NORMAL);        
            break;
        case SELF_DEFINE:
            OLED_Print_CharString(0,2,"   SELF_DEFINE",OLED_DISPLAY_NORMAL);        
            break;
        default:
            OLED_Print_CharString(0,2," SCENE_UNVALID",OLED_DISPLAY_NORMAL);
            break;
    }   
}

void DisMain(tSetData* set_data)
{
    OLED_Clear();
    OLED_Printf(0,0,OLED_DISPLAY_NORMAL,"Switch     R:%3d",set_data->red);    
    DisScene(set_data->scene);   
    OLED_Printf(0,4,OLED_DISPLAY_NORMAL,"      L:%3d",set_data->light);
    OLED_Printf(0,6,OLED_DISPLAY_NORMAL,"B:%3d      G:%3d",set_data->blue,set_data->green);    
}

void DisLightAdjust(uint8_t scene,uint8_t light)
{
    OLED_Clear();
    OLED_Print_CharString(0,0,"S+        Light+",OLED_DISPLAY_NORMAL);
    DisScene(scene);
    OLED_Print_CharString(0,4,"     L: ",OLED_DISPLAY_NORMAL);
    OLED_Printf(64,4,OLED_DISPLAY_INVERSE,"%3d",light);
    OLED_Print_CharString(0,6,"S-        Light-",OLED_DISPLAY_NORMAL);
}

void DisSceneAdjust(uint8_t scene)
{
    OLED_Clear();
    OLED_Print_CharString(0,0,"S+        Scene+",OLED_DISPLAY_NORMAL);
    switch ( scene )
    {
        case READ:
            OLED_Print_CharString(48,2,"READ",OLED_DISPLAY_INVERSE);
            break;
        case LIGHTING:
            OLED_Print_CharString(32,2,"LIGHTING",OLED_DISPLAY_INVERSE);        
            break;
        case SCENE_RED:
            OLED_Print_CharString(32,2,"SCENE_RED",OLED_DISPLAY_INVERSE);        
            break;
        case SCENE_GREEN:
            OLED_Print_CharString(16,2,"SCENE_GREEN",OLED_DISPLAY_INVERSE);        
            break;
        case SCENE_BLUE:
            OLED_Print_CharString(24,2,"SCENE_BLUE",OLED_DISPLAY_INVERSE);        
            break;
        case EXOTIC:
            OLED_Print_CharString(48,2,"EXOTIC",OLED_DISPLAY_INVERSE);        
            break;
        case SELF_DEFINE:
            OLED_Print_CharString(24,2,"SELF_DEFINE",OLED_DISPLAY_INVERSE);        
            break;
        default:
            OLED_Print_CharString(8,2,"SCENE_UNVALID",OLED_DISPLAY_INVERSE);
            break;
    } 
    OLED_Print_CharString(0,6,"S-        Scene-",OLED_DISPLAY_NORMAL);
}

void DisRedAdjust(uint8_t scene,uint8_t red)
{
    OLED_Clear();
    OLED_Print_CharString(0,0,"S+            R+",OLED_DISPLAY_NORMAL);
    DisScene(scene);
    OLED_Print_CharString(0,4,"     R: ",OLED_DISPLAY_NORMAL);
    OLED_Printf(64,4,OLED_DISPLAY_INVERSE,"%3d",red);
    OLED_Print_CharString(0,6,"S-            R-",OLED_DISPLAY_NORMAL);
}
void DisGreenAdjust(uint8_t scene,uint8_t green)
{
    OLED_Clear();
    OLED_Print_CharString(0,0,"S+            G+",OLED_DISPLAY_NORMAL);
    DisScene(scene);
    OLED_Print_CharString(0,4,"     G: ",OLED_DISPLAY_NORMAL);
    OLED_Printf(64,4,OLED_DISPLAY_INVERSE,"%3d",green);
    OLED_Print_CharString(0,6,"S-            G-",OLED_DISPLAY_NORMAL);
}

void DisBlueAdjust(uint8_t scene,uint8_t blue)
{
    OLED_Clear();
    OLED_Print_CharString(0,0,"S+            B+",OLED_DISPLAY_NORMAL);
    DisScene(scene);
    OLED_Print_CharString(0,4,"     B: ",OLED_DISPLAY_NORMAL);
    OLED_Printf(64,4,OLED_DISPLAY_INVERSE,"%3d",blue);
    OLED_Print_CharString(0,6,"S-            B-",OLED_DISPLAY_NORMAL);
}
void DisReadOrSave(void)
{
    OLED_Clear();
    OLED_Print_CharString(0,0,"S+          Read",OLED_DISPLAY_NORMAL);
    OLED_Print_CharString(0,6,"S-          Save",OLED_DISPLAY_NORMAL);
}
void DisSaveData(void)
{
    OLED_Clear();
    OLED_Print_CharString(0,0,"S+          Read",OLED_DISPLAY_NORMAL);
    OLED_Print_CharString(16,2,"Save Data...",OLED_DISPLAY_INVERSE);
    OLED_Print_CharString(0,6,"S-          Save",OLED_DISPLAY_NORMAL);
    //还差校验保存是否成功
}




/**
  * @brief  Display UI        
  * @param  None
  * @retval None
  * @Edit   River
  */
void Displya_UI(void)
{
    if(gFlagStatus.Flag_UI_Changed == SET)
    {
        if(gFlagStatus.Flag_set == RESET)
        {
            DisMain(GetSetDataAddr());
            if(TimerDisplay.handle)
            {
                SoftTimer_SetInvalid(TimerDisplay.handle);
                TimerDisplay.handle = 0;
            }
        }
        else
        {
            if(TimerDisplay.handle == 0)            //开启按键超时检测
            {
                TimerDisplay.handle = SoftTimer_Req(TimerDisplay.delay);
            }                       
            switch ( GetKeyCntAddr()->KeySwitchCnt )
            {
                case LIGHT_DIS:
                    DisLightAdjust(GetSetDataAddr()->scene,GetSetDataAddr()->light);                    
                    break;
                case SCENE_DIS:
                    DisSceneAdjust(GetSetDataAddr()->scene);
                    break;
                case RED_DIS:
                    DisRedAdjust(GetSetDataAddr()->scene,GetSetDataAddr()->red);
                    break;
                case GREEN_DIS:
                    DisGreenAdjust(GetSetDataAddr()->scene,GetSetDataAddr()->green);
                    break;
                case BLUE_DIS:
                    DisBlueAdjust(GetSetDataAddr()->scene,GetSetDataAddr()->blue);
                    break;
                case  READ_SAVE_DIS:
                    DisReadOrSave();
                    break;
            }        
            
        }
        gFlagStatus.Flag_UI_Changed = RESET;
    }    
    if(TimerDisplay.handle)
    {
        if(SoftTimer_Check(TimerDisplay.handle) == SOFT_TIMER_TIMEOUT)      
        {
            gFlagStatus.Flag_set = RESET; 
            gFlagStatus.Flag_UI_Changed = SET;                    
        }
        if(CalmStatusJudge())  
        {
            SoftTimer_Update(TimerDisplay.handle,TimerDisplay.delay);                
        }       
    }
}
    



/******************************** end of display.c ***********************************/


