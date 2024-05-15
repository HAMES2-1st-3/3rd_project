/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <AppMode.h>
#include <AppScheduling.h>

#include "IfxStm.h"
#include "IfxCpu_Irq.h"

/********** control, Decision **********/
#include <Control_Buzzer.h>
#include <Control_Motor.h>

#include <Decision_RPM.h>
#include <Decision_State.h>
#include <Decision_Sub_State.h>
/***************************************/

/*******for test DD header files********/
#include <Driver_Joystick.h>
#include <Driver_Potentiometer.h>
#include <Driver_ToF.h>
#include <Driver_USB.h>

#include <Driver_Buzzer.h>
#include <Driver_WheelFL.h>
#include <Driver_WheelFR.h>
#include <Driver_WheelRL.h>
#include <Driver_WheelRR.h>
#include <InterruptPriority.h>
/***************************************/


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
} g_stm; /**< \brief Stm global data */

struct
{
    uint8 scheduling_flag_1ms;
    uint8 scheduling_flag_10ms;
    uint8 scheduling_flag_20ms;
    uint8 scheduling_flag_50ms;
    uint8 scheduling_flag_100ms;
    uint8 scheduling_flag_250ms;
    uint8 scheduling_flag_500ms;
}g_scheduling_info;

uint32 g_counter_1ms = 0u;

/***********************************************************************/
/*User Variable*/
/***********************************************************************/
uint8 g_state;
uint8 g_sub_state;
float32 g_rpm_ref;
float32 theta_tilde;
uint32  cnt;
/***********************************************************************/
/*Function*/ 
/***********************************************************************/

static void AppTask1ms(void)
{
    cnt++;
    set_all_wheel(g_state, g_sub_state, g_rpm_ref);
    //opened_loop_control(cnt);
    closed_loop_control(0.043, 0.3, 0.001);
   // theta_tilde=observer_theta_fl(0.001).theta_tilde;
}

static void AppTask10ms(void)
{
    g_state = get_state();
    g_sub_state = 0; // 0: normal / 1: slow / 2: stop
}
static void AppTask20ms(void)
{
    g_rpm_ref = get_rpm_reference(g_state);


}
static void AppTask50ms(void)
{
    //_usb_printf("theta_tilde(theta-theta_hat)=%lf\n",theta_tilde);
    _usb_printf("g_ref_rpm.rl:%lf, g_cur_rpm.rl:%lf\n",g_ref_rpm.rl,g_cur_rpm.rl);
//    float32 poten = get_potentiometer_data(); // 100% ~ 0%
//    JoystickData JM = get_joystick_move_data(); // 100% ~ 0%
//    JoystickData JR = get_joystick_rotate_data(); // 100% ~ 0%
//    sint32 dist = get_tof_distance(); // mm value
//
//    _usb_printf("poten:%f, dist:%d, JoyM_x:%f, JoyM_y:%f, JoyR_x:%f, JoyR_y:%f\n",
//            poten,
//            dist,
//            JM.x,
//            JM.y,
//            JR.x,
//            JR.y);
//
//    set_wheelFL_dutycycle(poten*2-100);
//    set_wheelFR_dutycycle(poten*2-100);
//    set_wheelRL_dutycycle(poten*2-100);
//    set_wheelRR_dutycycle(poten*2-100);

}
static void AppTask100ms(void)
{

}
static void AppTask250ms(void)
{
//    set_buzzer_toggle();
}
static void AppTask500ms(void)
{
//    toggle_buzzer();
}
static void AppNoTask()
{

}

void AppScheduling(void)
{
    AppNoTask();
    if(g_scheduling_info.scheduling_flag_1ms == 1u)
    {
        g_scheduling_info.scheduling_flag_1ms = 0u;
        AppTask1ms();
        
        if(g_scheduling_info.scheduling_flag_10ms == 1u)
        {
            g_scheduling_info.scheduling_flag_10ms = 0u;
            AppTask10ms();
        }
        if(g_scheduling_info.scheduling_flag_20ms == 1u)
        {
            g_scheduling_info.scheduling_flag_20ms = 0u;
            AppTask20ms();
        }
        if(g_scheduling_info.scheduling_flag_50ms == 1u)
        {
            g_scheduling_info.scheduling_flag_50ms = 0u;
            AppTask50ms();
        }
        if(g_scheduling_info.scheduling_flag_100ms == 1u)
        {
            g_scheduling_info.scheduling_flag_100ms = 0u;
            AppTask100ms();
        }
        if(g_scheduling_info.scheduling_flag_250ms == 1u)
        {
            g_scheduling_info.scheduling_flag_250ms = 0u;
            AppTask250ms();
        }
        if(g_scheduling_info.scheduling_flag_500ms == 1u)
        {
            g_scheduling_info.scheduling_flag_500ms = 0u;
            AppTask500ms();
        }
    }
}



IFX_INTERRUPT(stm0_comp_ir0_isr, 0, ISR_PRIORITY_STMSR0);
void stm0_comp_ir0_isr(void)
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


void init_appscheduling(void) /* use STM0 */
{
    /* disable interrupts */
    boolean interruptState = IfxCpu_disableInterrupts();

    IfxStm_enableOcdsSuspend(&MODULE_STM0);

    g_stm.stmSfr = &MODULE_STM0;
    IfxStm_initCompareConfig(&g_stm.stmConfig);

    g_stm.stmConfig.triggerPriority = ISR_PRIORITY_STMSR0;
    g_stm.stmConfig.typeOfService   = IfxSrc_Tos_cpu0;
    g_stm.stmConfig.ticks           = 100000u;

    IfxStm_initCompare(g_stm.stmSfr, &g_stm.stmConfig);

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);
}



