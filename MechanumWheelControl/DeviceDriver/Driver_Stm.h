#ifndef DRIVER_STM
#define DRIVER_STM

/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxStm.h"
#include "IfxCpu_Irq.h"
#include "IfxPort.h"
#include "IfxPort_PinMap.h"
/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct
{
    uint8 scheduling_flag_1ms;
    uint8 scheduling_flag_10ms;
    uint8 scheduling_flag_20ms;
    uint8 scheduling_flag_50ms;
    uint8 scheduling_flag_100ms;
    uint8 scheduling_flag_250ms;
    uint8 scheduling_flag_500ms;
}SchedulingFlag;

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*External Variable*/ 
/***********************************************************************/
extern SchedulingFlag g_scheduling_info;


/***********************************************************************/
/*Global Function Prototype*/ 
/***********************************************************************/
void Driver_Stm_Init(void);


#endif /* DRIVER_STM */
