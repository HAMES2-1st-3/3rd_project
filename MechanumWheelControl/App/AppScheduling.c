/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <AppMode.h>
#include <AppScheduling.h>
/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct
{
    uint32 u32nuCnt1ms;
    uint32 u32nuCnt10ms;
    uint32 u32nuCnt100ms;
}TestCnt;
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
TestCnt g_test_cnt;
uint32 g_sub_state;
uint32 g_rpm_ref;
/***********************************************************************/
/*Function*/ 
/***********************************************************************/

static void AppTask1ms(void)
{
    g_test_cnt.u32nuCnt1ms++;
}

static void AppTask10ms(void)
{
    g_test_cnt.u32nuCnt10ms++;
    g_sub_state = get_sub_state(); // 0: normal / 1: slow / 2: stop
}
static void AppTask20ms(void)
{
    g_rpm_ref = get_rpm_reference();
}
static void AppTask50ms(void)
{

}
static void AppTask100ms(void)
{
    g_test_cnt.u32nuCnt100ms++;
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
            AppTask100ms();
        }
        if(g_scheduling_info.scheduling_flag_500ms == 1u)
        {
            g_scheduling_info.scheduling_flag_500ms = 0u;
            AppTask500ms();
        }
    }
}
