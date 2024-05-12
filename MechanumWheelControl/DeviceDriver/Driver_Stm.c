/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <Driver_Stm.h>

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct
{
    Ifx_STM             *stmSfr;            /**< \brief Pointer to Stm register base */
    IfxStm_CompareConfig stmConfig;         /**< \brief Stm Configuration structure */
    volatile uint8       LedBlink;          /**< \brief LED state variable */
    volatile uint32      counter;           /**< \brief interrupt counter */
} App_Stm;


/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/


/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
App_Stm g_stm; /**< \brief Stm global data */
uint32 g_counter_1ms = 0u;
SchedulingFlag g_scheduling_info;

/***********************************************************************/
/*Function*/ 
/***********************************************************************/
IFX_INTERRUPT(STM_Int0Handler, 0, 100);

void STM_Int0Handler(void)
{
    IfxCpu_enableInterrupts();    
    
    IfxStm_clearCompareFlag(g_stm.stmSfr, g_stm.stmConfig.comparator);
    IfxStm_increaseCompare(g_stm.stmSfr, g_stm.stmConfig.comparator, 100000u);

    g_counter_1ms++;


    if((g_counter_1ms % 1) == 0u)
    {
        g_scheduling_info.scheduling_flag_1ms = 1u;
    }  

    if((g_counter_1ms % 10) == 0u)
    {
        g_scheduling_info.scheduling_flag_10ms = 1u;
    }
    if((g_counter_1ms % 20) == 0u)
    {
        g_scheduling_info.scheduling_flag_20ms = 1u;
    }
    if(g_counter_1ms % 50 == 0u)
    {
        g_scheduling_info.scheduling_flag_50ms = 1u;
    }
    if((g_counter_1ms % 100) == 0u)
    {
        g_scheduling_info.scheduling_flag_100ms = 1u;
    }
    if((g_counter_1ms % 250) == 0u)
    {
        g_scheduling_info.scheduling_flag_250ms = 1u;
    }
    if((g_counter_1ms % 500) == 0u)
    {
        g_scheduling_info.scheduling_flag_500ms = 1u;
    }
}

void Driver_Stm_Init(void)
{
    /* disable interrupts */
    boolean interruptState = IfxCpu_disableInterrupts();

    IfxStm_enableOcdsSuspend(&MODULE_STM0);

    g_stm.stmSfr = &MODULE_STM0;
    IfxStm_initCompareConfig(&g_stm.stmConfig);

    g_stm.stmConfig.triggerPriority = 100u;
    g_stm.stmConfig.typeOfService   = IfxSrc_Tos_cpu0;
    g_stm.stmConfig.ticks           = 100000u;

    IfxStm_initCompare(g_stm.stmSfr, &g_stm.stmConfig);

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);
}

