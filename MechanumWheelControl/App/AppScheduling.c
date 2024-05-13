/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <AppMode.h>
#include <AppScheduling.h>

#include <Driver_Stm.h>

#include <Control_Buzzer.h>
#include <Control_Motor.h>

#include <Decision_RPM.h>
#include <Decision_State.h>
#include <Decision_Sub_State.h>

/*****test******/

#include <Driver_Joystick.h>
#include <Driver_Potentiometer.h>
#include <Driver_ToF.h>
#include <Driver_USB.h>

#include <Driver_Buzzer.h>
#include <Driver_WheelFL.h>
#include <Driver_WheelFR.h>
#include <Driver_WheelRL.h>
#include <Driver_WheelRR.h>



/*****************/


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
/*Variable*/ 
/***********************************************************************/

uint8 g_sub_state;
uint32 g_rpm_ref;
/***********************************************************************/
/*Function*/ 
/***********************************************************************/

static void AppTask1ms(void)
{

}

static void AppTask10ms(void)
{
//    g_sub_state = get_sub_state(); // 0: normal / 1: slow / 2: stop
}
static void AppTask20ms(void)
{
//    g_rpm_ref = get_rpm_reference();


}
static void AppTask50ms(void)
{
    { // for test
    uint32 temp = get_potentiometer_value();

    set_wheelFL_dutycycle((float32)temp / 4095 * 200 - 100);
    set_wheelFR_dutycycle((float32)temp / 4095 * 200 - 100);
    set_wheelRL_dutycycle((float32)temp / 4095 * 200 - 100);
    set_wheelRR_dutycycle((float32)temp / 4095 * 200 - 100);

//    usb_printf("P:%d, FL:%d, FR:%d, RL:%d, RR:%d, Tof:%d\n",
//            get_potentiometer_value(),
//            get_wheelFL_tick(),
//            get_wheelFR_tick(),
//            get_wheelRL_tick(),
//            get_wheelRR_tick(),
//            get_tof_distance());
    usb_printf("JoyM_x:%u, JoyM_y:%u, JoyR_x:%u, JoyR_y:%u\n",
            get_mid_adc_group0_raw()->UlSSense1_Raw,
            get_mid_adc_group0_raw()->UlSSense2_Raw,
            get_mid_adc_group2_raw()->UlSSense1_Raw,
            get_mid_adc_group2_raw()->UlSSense2_Raw);
    }

}
static void AppTask100ms(void)
{

}
static void AppTask250ms(void)
{

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
