/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <AppMode.h>
#include <AppScheduling.h>

#include <InterruptPriority.h>

#include "IfxStm.h"
#include "IfxCpu_Irq.h"

#include <Driver_Wheels.h>
#include <Driver_Potentiometer.h>
#include <Driver_Bluetooth.h>
#include <Driver_USB.h>

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
} s_stm0; /**< \brief Stm global data */

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
#define CLOSED_LOOP 0

static sint32 s_dist;
static uint32 s_rpm_max;

static WheelRPMs s_wheel_rpms_ref;
static JoystickValues s_joystick_values;


#if  CLOSED_LOOP == 1
static WheelTicks s_wheel_ticks;
static WheelRPMs s_wheel_rpms_measured;
static WheelRPMs s_wheel_rpms_error;
#endif

static WheelDutycycles s_wheel_dutycycles;

static BuzzerState s_buzzer_state;

/***********************************************************************/
/*Function*/ 
/***********************************************************************/

static void AppTask1ms(void)
{

#if  CLOSED_LOOP == 1
    /* set wheel rpm measured, error, pid controll */
    s_wheel_ticks = get_wheel_ticks();
    s_wheel_rpms_measured = calc_wheel_rpms_measured(s_wheel_ticks, 0.001);
    s_wheel_rpms_error = calc_wheel_rpms_error(s_wheel_rpms_ref, s_wheel_rpms_measured);
    s_wheel_dutycycles = pid_controller(s_wheel_rpms_error);

#else
    s_wheel_dutycycles = open_loop_controller(s_wheel_rpms_ref);
#endif
    set_wheels_dutycycle(s_wheel_dutycycles);
}

static void AppTask10ms(void)
{

}
static void AppTask20ms(void)
{
    /* set wheel rpm reference */
    JoystickValue temp = get_joystick_move_value();
    s_joystick_values.move.x = temp.x;
    s_joystick_values.move.y = temp.y;

//    s_joystick_values = get_bluetooth_joystick_values();
    s_wheel_rpms_ref = calc_wheel_rpms_ref(s_rpm_max, s_joystick_values);

}
static void AppTask50ms(void)
{
    /* value test */
//    send_usb_printf("move_x: %d move_y: %d rotate_x: %d rotate_y: %d\n",
//            s_joystick_values.move.x,
//            s_joystick_values.move.y,
//            s_joystick_values.rotate.x,
//            s_joystick_values.rotate.y);

//    send_usb_printf("rpm_fl: %f\n", s_wheel_rpms_ref.fl);
//    send_usb_printf("rpm_fl: %.2f rpm_fr: %.2f rpm_rl: %.2f rpm_rr: %.2f\n",
//            s_wheel_rpms_ref.fl,
//            s_wheel_rpms_ref.rl,
//            s_wheel_rpms_ref.fr,
//            s_wheel_rpms_ref.rr);

    send_usb_printf("duty_fl: %.2f duty_fr: %.2f duty_rl: %.2f duty_rr: %.2f\n",
            s_wheel_dutycycles.fl,
            s_wheel_dutycycles.rl,
            s_wheel_dutycycles.fr,
            s_wheel_dutycycles.rr);



    /* set rpm_max */
    s_dist = get_tof_distance();
    s_rpm_max = get_potentiometer_value(); // 0 ~ 4095
//    s_rpm_max = get_max_rpm(s_dist, s_rpm_max);

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

    IfxStm_clearCompareFlag(s_stm0.stmSfr, s_stm0.stmConfig.comparator);
    IfxStm_increaseCompare(s_stm0.stmSfr, s_stm0.stmConfig.comparator, 100000u);

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

    s_stm0.stmSfr = &MODULE_STM0;
    IfxStm_initCompareConfig(&s_stm0.stmConfig);

    s_stm0.stmConfig.triggerPriority = ISR_PRIORITY_STMSR0;
    s_stm0.stmConfig.typeOfService   = IfxSrc_Tos_cpu0;
    s_stm0.stmConfig.ticks           = 100000u;

    IfxStm_initCompare(s_stm0.stmSfr, &s_stm0.stmConfig);

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);
}




