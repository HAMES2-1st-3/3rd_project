/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "AppScheduling.h"
#include "Driver_Stm.h"
#include "Driver_Adc.h"
#include "Driver_Asc.h"
#include "Driver_Gtm.h"
#include "AppMode.h"
#include "Driver_Port.h"
#include "Driver_encoder.h"

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct
{
    uint32_t u32nuCnt1ms;
    uint32_t u32nuCnt10ms;
    uint32_t u32nuCnt100ms;
}TestCnt;
;

/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/
static void AppTask1ms(void);
static void AppTask10ms(void);
static void AppTask100ms(void);
static float32 LPF(float32 Y_fill_d,float32 u,float32 cf,float32 T);
/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
TestCnt stTestCnt;
uint32 t;
float32 rad;
float32 rad_old;
int32_t Kp = 1.5;
int32_t Ki = 2.0;
float32 Ts = 0.001; // 1ms
float32 Vin;
float32 w;
float32 w_old;
float32 w_ref;
float32 w_rpm;
float32 error_w;
float32 error_w_int;
float32 error_w_int_old;
float32 Wd = 2; // rotate / s
uint32 t1 = 4, t2 = 19, t3 = 49, t4 = 64;
//App_GtmTomTimer g_GtmTomTimer;
/***********************************************************************/
/*Function*/ 
/***********************************************************************/
static void AppTask1ms(void)
{
    stTestCnt.u32nuCnt1ms++;

    rad = encoder_get_radians();
    w = (rad - rad_old) / Ts;
    // add low-pass filter
    w = LPF(w_old, w, 500, Ts);
    rad_old = rad;
    w_old = w;
    w_rpm = (uint32)(60*w / 2*PI);

    if(stTestCnt.u32nuCnt1ms % 1000 == 0)
        t++;


    if(t <= t1){
        w_ref = 0;
    }
    else if(t <= t2){
        w_ref = 0.06667*Wd*(2*PI)*(t-4);
    }
    else if(t <= t3){
        w_ref = Wd*(2*PI);
    }
    else if(t <= t4){
        w_ref = Wd*2*PI - 0.06667*Wd*(2*PI)*(t-26);
    }
    else{
        w_ref = 0;
    }
    // PI control
    if(t <= t1){
        Vin = 0;
    }
    else if(t < t4){
        error_w = w_ref - w;
        error_w_int = error_w_int_old + (error_w)*Ts;
        error_w_int_old = error_w_int;

        if(error_w_int > 10)
            error_w_int = 10;

        Vin = (Kp * error_w + Ki * error_w_int);

        if(Vin > 11)
            Vin = 11;
        else if (Vin <0)
            Vin =0;
    }
    else{
        Vin = 0;
    }
    g_GtmTomTimer.tOn[0] = Vin/12;
    GtmTomPwmHl_run();
}

static void AppTask10ms(void)
{
    
    stTestCnt.u32nuCnt10ms++;
    //Unit_Mode();
//    IfxPort_togglePin(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex);
    /*ADC Test*/
    //Driver_Adc0_ConvStart();

    /*ASC Test*/
    //Driver_Asc0_TxTest();

    /*PWM Test*/
    //Driver_Gtm_Tom0Test();
}

static void AppTask100ms(void)
{
    stTestCnt.u32nuCnt100ms++;
   // if(stTestCnt.u32nuCnt100ms%10==0){
   //     mypwmcnt1s=mypwmcnt1ms;
   // }
}
static void AppNoTask(){
    // PID control
}

static float32 LPF(float32 Y_fill_d,float32 u,float32 cf,float32 T){ // cf=cutoff

    return (1-T*cf)*Y_fill_d+T*cf*u;
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

        if(stSchedulingInfo.u8nuScheduling100msFlag == 1u)
        {
            stSchedulingInfo.u8nuScheduling100msFlag = 0u;
            AppTask100ms();
        }
    }
}
