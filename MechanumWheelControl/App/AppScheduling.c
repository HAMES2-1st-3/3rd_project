/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <AppMode.h>
#include <AppScheduling.h>

#include <InterruptPriority.h>

#include "IfxStm.h"
#include "IfxCpu_Irq.h"

#include <Driver_Wheels.h>

/********** control, Decision **********/
#include <Control_Buzzer.h>
#include <Control_Wheels.h>

//#include <Decision_RPM.h>
//#include <Decision_State.h>
//#include <Decision_Sub_State.h>
/***************************************/

///*******for test DD header files********/
//#include <Driver_Joystick.h>
//#include <Driver_Potentiometer.h>
//#include <Driver_ToF.h>
//#include <Driver_USB.h>
//#include <Driver_Bluetooth.h>
//
//#include <Driver_Buzzer.h>
//
//
///***************************************/


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

sint32 s_dist;
uint32 s_rpm_max;

WheelRPMs s_wheel_rpms_ref;
JoystickValues s_joystick_values;

WheelTicks s_wheel_ticks;
WheelRPMs s_wheel_rpms_measured;
WheelRPMs s_wheel_rpms_error;
WheelDutycycles s_wheel_dutycycles;

BuzzerState s_buzzer_state;

/***********************************************************************/
/*Function*/ 
/***********************************************************************/

static void AppTask1ms(void)
{
    /* set wheel rpm measured, error, pid controll */
    s_wheel_ticks = get_wheel_ticks();
    s_wheel_rpms_measured = calc_wheel_rpms_measured(s_wheel_ticks, 0.001);
    s_wheel_rpms_error = calc_wheel_rpms_error(s_wheel_rpms_ref, s_wheel_rpms_measured);
    s_wheel_dutycycles = pid_controller(s_wheel_rpms_error);
}

static void AppTask10ms(void)
{

}
static void AppTask20ms(void)
{
    /* set wheel rpm reference */
    s_joystick_values = get_bluetooth_joystick_values();
    s_wheel_rpms_ref = calc_wheel_rpms_ref(s_rpm_max, s_joystick_values);

}
static void AppTask50ms(void)
{
//    JoystickValues joystick_values = get_bluetooth_joystick_values();
//    send_usb_printf("move_x: %d move_y: %d rotate_x: %d rotate_y: %d\n",
//            joystick_values.move.x,
//            joystick_values.move.y,
//            joystick_values.rotate.x,
//            joystick_values.rotate.y);

    /* set rpm_max */
    s_dist = get_tof_distance();
    s_rpm_max = get_potentialmeter_value();
    s_rpm_max = get_max_rpm(s_dist, s_rpm_max);

    s_buzzer_state = set_buzzer_state(s_dist);
    set_buzzer_mode_on_off(s_buzzer_state);
}
static void AppTask100ms(void)
{

}
static void AppTask250ms(void)
{
    set_buzzer_mode_250ms(s_buzzer_state);
}
static void AppTask500ms(void)
{
    set_buzzer_mode_500ms(s_buzzer_state);
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




