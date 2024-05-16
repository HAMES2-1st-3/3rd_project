/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <AppMode.h>
#include <AppScheduling_cpu1.h>

#include <InterruptPriority.h>

#include "IfxStm.h"
#include "IfxCpu_Irq.h"

#include <Driver_Bluetooth.h>


/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/


/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/
static void AppTask1ms(void);
static void AppTask10ms(void);
static void AppTask20ms(void);
static void AppTask50ms(void);
static void AppTask100ms(void);
static void AppTask250ms(void);
static void AppTask500ms(void);
/***********************************************************************/
/*System Variable*/
/***********************************************************************/


struct
{
    Ifx_STM             *stmSfr;            /**< \brief Pointer to Stm register base */
    IfxStm_CompareConfig stmConfig;         /**< \brief Stm Configuration structure */
    volatile uint8       LedBlink;          /**< \brief LED state variable */
    volatile uint32      counter;           /**< \brief interrupt counter */
} g_stm_1; /**< \brief Stm global data */

struct
{
    uint8 scheduling_flag_1ms;
    uint8 scheduling_flag_10ms;
    uint8 scheduling_flag_20ms;
    uint8 scheduling_flag_50ms;
    uint8 scheduling_flag_100ms;
    uint8 scheduling_flag_250ms;
    uint8 scheduling_flag_500ms;
}g_scheduling_info_1;

uint32 g_counter_1_1ms = 0u;

/***********************************************************************/
/*User Variable*/
/***********************************************************************/

/***********************************************************************/
/*Function*/ 
/***********************************************************************/

static void AppTask1ms(void)
{
    receive_bluetooth_joystick_values();
}

static void AppTask10ms(void)
{

}
static void AppTask20ms(void)
{

}
static void AppTask50ms(void)
{

}
static void AppTask100ms(void)
{

}
static void AppTask250ms(void)
{

}
static void AppTask500ms(void)
{

}
static void AppNoTask()
{

}

void AppScheduling_cpu1(void)
{
    AppNoTask();
    if(g_scheduling_info_1.scheduling_flag_1ms == 1u)
    {
        g_scheduling_info_1.scheduling_flag_1ms = 0u;
        AppTask1ms();
        
        if(g_scheduling_info_1.scheduling_flag_10ms == 1u)
        {
            g_scheduling_info_1.scheduling_flag_10ms = 0u;
            AppTask10ms();
        }
        if(g_scheduling_info_1.scheduling_flag_20ms == 1u)
        {
            g_scheduling_info_1.scheduling_flag_20ms = 0u;
            AppTask20ms();
        }
        if(g_scheduling_info_1.scheduling_flag_50ms == 1u)
        {
            g_scheduling_info_1.scheduling_flag_50ms = 0u;
            AppTask50ms();
        }
        if(g_scheduling_info_1.scheduling_flag_100ms == 1u)
        {
            g_scheduling_info_1.scheduling_flag_100ms = 0u;
            AppTask100ms();
        }
        if(g_scheduling_info_1.scheduling_flag_250ms == 1u)
        {
            g_scheduling_info_1.scheduling_flag_250ms = 0u;
            AppTask250ms();
        }
        if(g_scheduling_info_1.scheduling_flag_500ms == 1u)
        {
            g_scheduling_info_1.scheduling_flag_500ms = 0u;
            AppTask500ms();
        }
    }
}



IFX_INTERRUPT(stm1_comp_ir0_isr, 1, ISR_PRIORITY_STMSR1);
void stm1_comp_ir0_isr(void)
{
    IfxCpu_enableInterrupts();

    IfxStm_clearCompareFlag(g_stm_1.stmSfr, g_stm_1.stmConfig.comparator);
    IfxStm_increaseCompare(g_stm_1.stmSfr, g_stm_1.stmConfig.comparator, 100000u);

    g_counter_1_1ms++;


    if((g_counter_1_1ms % 1) == 0u)
    {
        g_scheduling_info_1.scheduling_flag_1ms = 1u;
    }

    if((g_counter_1_1ms % 10) == 0u)
    {
        g_scheduling_info_1.scheduling_flag_10ms = 1u;
    }
    if((g_counter_1_1ms % 20) == 0u)
    {
        g_scheduling_info_1.scheduling_flag_20ms = 1u;
    }
    if(g_counter_1_1ms % 50 == 0u)
    {
        g_scheduling_info_1.scheduling_flag_50ms = 1u;
    }
    if((g_counter_1_1ms % 100) == 0u)
    {
        g_scheduling_info_1.scheduling_flag_100ms = 1u;
    }
    if((g_counter_1_1ms % 250) == 0u)
    {
        g_scheduling_info_1.scheduling_flag_250ms = 1u;
    }
    if((g_counter_1_1ms % 500) == 0u)
    {
        g_scheduling_info_1.scheduling_flag_500ms = 1u;
    }
}


void init_appscheduling_cpu1(void) /* use STM0 */
{
    /* disable interrupts */
    boolean interruptState = IfxCpu_disableInterrupts();

    IfxStm_enableOcdsSuspend(&MODULE_STM1);

    g_stm_1.stmSfr = &MODULE_STM1;
    IfxStm_initCompareConfig(&g_stm_1.stmConfig);

    g_stm_1.stmConfig.triggerPriority = ISR_PRIORITY_STMSR1;
    g_stm_1.stmConfig.typeOfService   = IfxSrc_Tos_cpu1;
    g_stm_1.stmConfig.ticks           = 100000u;

    IfxStm_initCompare(g_stm_1.stmSfr, &g_stm_1.stmConfig);

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);
}




