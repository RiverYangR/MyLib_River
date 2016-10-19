#ifndef __CAN_H__
#define __CAN_H__

#include <string.h>
#include "stm32f10x.h"
#include "key_app.h"


#define CANID_BROADCAST_RX     0x1FFu
#define CANID_BROADCAST_TX     0x2FFu
#define CAN_MASTER_ID_LIGHT         0x100u
#define CAN_SLAVE_ID_LIGHT         0x200u

/************************* Commond List start*******************************/
#define COMMOND_SWITCH_SCENE        0x21
#define COMMOND_ADJUST_LIGHTNESS    0x22
#define COMMOND_SET_COLOR           0x23
#define COMMOND_READ_PARAM          0x24
#define COMMOND_SAVE_PARAM          0x25
/************************* Commond List end *******************************/

void CAN1_ConfigInit(void);
void Can_Send_StdMsg(uint16_t stdid, uint8_t* msg, uint8_t len);























#endif //__CAN_H__
