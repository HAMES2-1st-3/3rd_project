/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <App/AppMode.h>
#include <App/AppScheduling.h>
#include <Device_driver/Driver_Adc.h>
#include <Device_driver/Driver_Asc.h>
#include <Device_driver/Driver_encoder.h>
#include <Device_driver/Driver_Gtm.h>
#include <Device_driver/Driver_Port.h>
#include <Device_driver/Driver_Stm.h>
#include "math.h"
#include <Control/PID.h>
#include <Control/Driver_serialPlotter.h>

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
/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/
static void AppTask1ms(void);
static void AppTask10ms(void);
static void AppTask100ms(void);
/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
TestCnt stTestCnt;
uint32 t;                                   // count every 1s
float32 rad;                                // radian from encoder
float32 rad_ref;                            // radian reference
float32 rad_old;                            // radian old value
float32 Kp_r = 2.0;                         // Kp for radian
float32 Ki_r = 1.8;                         // Ki for radian
float32 Ts = 0.001;                         // 1ms
float32 w;                                  // current angular velocity
float32 w_old;                              // old angular velocity
float32 w_ref;                              // angular velocity reference
float32 w_rpm;                              // current rpm
float32 Kp_w = 1.0;                         // Kp for angular velocity
float32 Ki_w = 2.0;                         // Ki for angular velocity
float32 Vin;                                // V
float32 Wd = 1;                             // 1 rotate/s (page.141)
uint32 t1 = 4, t2 = 19, t3 = 29, t4 = 44;   // time (page.141)
int flag = 1;                               // debugging Vin for USB_printf
/***********************************************************************/
/*Function*/ 
/***********************************************************************/

static void AppTask1ms(void)
{
    stTestCnt.u32nuCnt1ms++;

    rad_ref = sin(2*PI*stTestCnt.u32nuCnt1ms*Ts*0.1)*2*PI;  // period: 10s, max: 2*PI, min: -2*PI
    rad = encoder_get_radians();                            // get radian from encoder
//    rad = LPF(rad_old, rad, 500, Ts); // weird...
    w_ref = PID(rad_ref, rad, Kp_r, Ki_r, Ts);

    w = (rad - rad_old) / Ts;
    w = LPF(w_old, w, 500, Ts);
    w_rpm = (uint32)(60*w / 2*PI);

    rad_old = rad;
    w_old = w;
    if(stTestCnt.u32nuCnt1ms % 1000 == 0) // Count 1s
        t++;
/*
     // Refrence w (book page.141)
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
        Vin = PID(w_ref, w, Kp_w, Ki_w, Ts);
        Vin = saturation(0, 11, Vin);
    }
    else{
        Vin = 0;
    }*/
    Vin = PID(w_ref, w,Kp_w, Ki_w, Ts);
    Vin = saturation(-11,11, Vin);
    if(Vin < 0){
        direction(-1);
        Vin = -Vin;
        flag = -1; // for USBprintf
    }else{
        direction(1);
        flag = 1; // for USBprintf
    }
    g_GtmTomTimer.tOn[0] = Vin/12;      // Set duty value (0.0~1.0)
    GtmTomPwmHl_run();                  // Control motor using PWM
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
static void AppTask50ms(void)
{
//    USB_printf("time:%d, rad:%lf, rad_ref:%lf, w_ref:%lf, w: %lf, Vin : %lf\n", t, rad, rad_ref, w_ref, w, Vin*flag);

    USB_printf("rad:%lf, rad_ref:%lf\n", rad, rad_ref);
}
static void AppTask100ms(void)
{
    stTestCnt.u32nuCnt100ms++;
}
static void AppNoTask(){
    // PID control
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
    }
}
