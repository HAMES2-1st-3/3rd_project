#ifndef DRIVER_GTM
#define DRIVER_GTM

/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxGtm_Tom_Timer.h"
#include "IfxGtm_Tom_PwmHl.h"


/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct
{
    struct
    {
        float32 gtmFreq;
        float32 gtmGclkFreq;
        //추가
        float32 gtmCmuClk0Freq; //cmu clk0 freq
        Ifx_TimerValue timerValue;
    }info;
    float32 tOn[3];
    struct
    {
        IfxGtm_Tom_Timer timerOneMs;        /**< \brief Timer driver */
        IfxGtm_Tom_PwmHl pwm;
    }drivers;
    struct
    {
        uint32 slotOneMs;
    }isrCounter;
} App_GtmTomTimer;

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*External Variable*/ 
/***********************************************************************/
IFX_EXTERN App_GtmTomTimer g_GtmTomTimer;


/***********************************************************************/
/*Global Function Prototype*/ 
/***********************************************************************/
extern void Driver_Gtm_Init(void);
extern void Driver_Gtm_Tom0Test(float32_t fDuty);
extern void GtmTomPwmHl_run(void);
#endif /* DRIVER_STM */
