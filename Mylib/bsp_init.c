#include "bsp_init.h"


void BspInit(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    CAN1_ConfigInit();
    OLED_Configuration();
    KeyInit();
    TIM2_ConfigInit();
    TIM2_Start();
    SoftTimer_Init();
    DisBootRun();
    TrasmitReadParam();    
}



/******************************** end of bsp_init.h ***********************************/

