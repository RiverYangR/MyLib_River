#ifndef __SYSTIME_H__
#define __SYSTIME_H__

#ifdef __cplusplus
exter "C"
{
#endif

#include <stdint.h>
#include "stm32f10x.h"
#include "stm32f10x_tim.h"

//************************************************************************************
//! Function Parameters
//************************************************************************************

#define TIMER_ELEMENT_NUM_MAX               20          //!< Maxium SoftTimer
#define SOFT_TIMER_SUCCESS                  0           //!< Operate success
#define SOFT_TIMER_ING                      1           //!< Timer Counting
#define SOFT_TIMER_TIMEOUT                  2           //!< Timer Timeout
#define SOFT_TIMER_ERR                      3           //!< Timer erro   

//************************************************************************************
//! Typedef
//************************************************************************************    
typedef struct
{
    uint16_t handle;
    uint32_t delay;
}tTimerElemt;

//************************************************************************************
//! Public API
//************************************************************************************
uint16_t SoftTimer_Init(void);
uint16_t SoftTimer_Destroy(void);
uint16_t SoftTimer_Req(uint32_t Tick);
uint16_t SoftTimer_Update(uint32_t Id,uint32_t Tick);
uint16_t SoftTimer_SetInvalid(uint32_t Id);
uint16_t SoftTimer_Check(uint16_t Handle);

#ifdef __cplusplus
}
#endif



#endif
/******************************** end of SysTime.h ***********************************/

