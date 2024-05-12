/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <App/AppMode.h>
#include <App/AppScheduling.h>
#include <DeviceDriver/Driver_Stm.h>

#include <Control/Control_Buzzer.h>

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
TestCnt stTestCnt;



uint8 state=5;
uint8 substate;

/***********************************************************************/
/*Function*/ 
/***********************************************************************/
float32 pi=3.141592;

static void AppTask1ms(void)
{
    stTestCnt.u32nuCnt1ms++;


    if(stTestCnt.u32nuCnt1ms>0&&stTestCnt.u32nuCnt1ms<=5000){
        substate=0;
    }
    else if(stTestCnt.u32nuCnt1ms>5000&&stTestCnt.u32nuCnt1ms<=10000){
        substate=1; //slow
    }
    else if(stTestCnt.u32nuCnt1ms>10000&&stTestCnt.u32nuCnt1ms<=15000){
        substate=2;
    }
    else{
        state=4;
    }
}

static void AppTask10ms(void)
{
    stTestCnt.u32nuCnt10ms++;

}
static void AppTask20ms(void)
{

}
static void AppTask50ms(void)
{

}
static void AppTask100ms(void)
{
    stTestCnt.u32nuCnt100ms++;

}
static void AppTask250ms(void){
    set_buzzer_250ms(state,substate);
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
    if(stSchedulingInfo.u8nuScheduling1msFlag == 1u)
    {
        stSchedulingInfo.u8nuScheduling1msFlag = 0u;
        AppTask1ms();
        
        if(stSchedulingInfo.u8nuScheduling10msFlag == 1u)
        {
            stSchedulingInfo.u8nuScheduling10msFlag = 0u;
            AppTask10ms();
        }
        if(stSchedulingInfo.u8nuScheduling20msFlag == 1u)
        {
            stSchedulingInfo.u8nuScheduling20msFlag = 0u;
            AppTask20ms();
        }
        if(stSchedulingInfo.u8nuScheduling50msFlag == 1u)
        {
            stSchedulingInfo.u8nuScheduling50msFlag = 0u;
            AppTask50ms();
        }
        if(stSchedulingInfo.u8nuScheduling100msFlag == 1u)
        {
            stSchedulingInfo.u8nuScheduling100msFlag = 0u;
            AppTask100ms();
        }
        if(stSchedulingInfo.u8nuScheduling250msFlag == 1u)
        {
            stSchedulingInfo.u8nuScheduling250msFlag = 0u;
            AppTask250ms();
        }
        if(stSchedulingInfo.u8nuScheduling500msFlag == 1u)
        {
            stSchedulingInfo.u8nuScheduling500msFlag = 0u;
            AppTask500ms();
        }
    }
}

