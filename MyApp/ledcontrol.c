#include "ledcontrol.h"



static void TrasmitScene(void)
{
    uint8_t TrasmitData[2] = {0};
    TrasmitData[0] = COMMOND_SWITCH_SCENE;
    TrasmitData[1] = GetSetDataAddr()->scene;
    Can_Send_StdMsg( 0x2FF,TrasmitData,sizeof(TrasmitData) );   
    TrasmitReadParam();
}

static void TrasmitLight(void)
{
    uint8_t TrasmitData[2] = {0};
    TrasmitData[0] = COMMOND_ADJUST_LIGHTNESS;
    TrasmitData[1] = GetSetDataAddr()->light;
    Can_Send_StdMsg( 0x2FF,TrasmitData,sizeof(TrasmitData) );    
}

static void TrasmitColor(void)
{
    uint8_t TrasmitData[4] = {0};
    TrasmitData[0] = COMMOND_SET_COLOR;
    TrasmitData[1] = GetSetDataAddr()->red;
    TrasmitData[2] = GetSetDataAddr()->green;
    TrasmitData[3] = GetSetDataAddr()->blue;
    Can_Send_StdMsg( 0x2FF,TrasmitData,sizeof(TrasmitData) );    
}

void TrasmitReadParam(void)
{
    uint8_t TrasmitData[1] = {0};
    TrasmitData[0] = COMMOND_READ_PARAM;
    
    gFlagStatus.Flag_read_successed = RESET;
    while( gFlagStatus.Flag_read_successed == RESET)    //确保读取参数成功
    {
        Can_Send_StdMsg( 0x2FF,TrasmitData,sizeof(TrasmitData) );    
        delay_ms(50);
    }  
}

void TrasmitSaveParam(void)
{
    uint8_t TrasmitData[1] = {0};
    TrasmitData[0] = COMMOND_SAVE_PARAM;
    Can_Send_StdMsg( 0x2FF,TrasmitData,sizeof(TrasmitData) );    
}

/**
  * @brief  Execute the corresponding function        
  * @param  None
  * @retval None
  * @Edit   River
  */
void ExecuteFun(void)
{
    if(gFlagStatus.Flag_Exe == SET)
    {
        switch ( GetKeyCntAddr()->KeySwitchCnt )
        {
            case LIGHT_DIS:
                TrasmitLight();                   
                break;
            case SCENE_DIS:
                TrasmitScene(); 
                break;
            case RED_DIS:
                TrasmitColor();
                break;
            case GREEN_DIS:
                TrasmitColor();
                break;
            case BLUE_DIS:
                TrasmitColor();
                break;
            case  READ_SAVE_DIS:
                if(gFlagStatus.Flag_read_save == FLAG_READ)
                {
                    TrasmitReadParam();
                    gFlagStatus.Flag_read_save = FLAG_READ_SAVE_UNVALID;
                }
                else if(gFlagStatus.Flag_read_save == FLAG_SAVE)
                {
                    TrasmitSaveParam();
                    gFlagStatus.Flag_read_save = FLAG_READ_SAVE_UNVALID;
                }
                break;
        }           
        gFlagStatus.Flag_Exe = RESET;
    }
}





/******************************** end of ledcontrol.c ***********************************/

