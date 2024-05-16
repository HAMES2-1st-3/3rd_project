/*
 * Control_.c
 *
 *  Created on: 2024. 5. 10.
 *      Author: user
 *
 *  필요 함수 및 기능
 *    void FUNC(int32 state, int32 sub_state, float32 ref_rpm);
 *     - ref_rpm를 통해 목표 rpm 설정
 *     - state를 통해 네개의 모터 각각의 rpm 및 방향 설정
 *     - sub_state를 통해 slow, stop인 경우 rpm 및 방향 재설정
 *
 *    float32 PID(float32 ref, float32 cur, float32 kp, float32 ki, float32 Ts)
 *     - PI 제어 함수
 *
 *    float32 LPF(float32 Y_fill_d,float32 u,float32 cf,float32 T){ // cf=cutoff
 *     - Low-Pass Filter 구현
 *
 *     -> Driver_1.h, Driver_2.h, Driver_3.h, Driver_4.h
 */


/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Control_Motor.h"
#include "Driver_WheelFL.h"
#include "Driver_WheelFR.h"
#include "Driver_WheelRL.h"
#include "Driver_WheelRR.h"
#include "Driver_USB.h"
#include <stdint.h>

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

#define RPM_MAX 5300.0
#define e 2.718281
/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct{ //센싱 변수
        uint32 fl_rpm;
        uint32 fr_rpm;
        uint32 rr_rpm;
        uint32 rl_rpm;
}setrpm;
/***********************************************************************/
/*Static Function Prototype*/
/***********************************************************************/
static float32 LPF(float32 Y_fill_d,float32 u,float32 cf,float32 T);
static float32 get_derivative_fl(sint32 ticks,float32 Ts);
static float32 get_derivative_fr(sint32 ticks,float32 Ts);
static float32 get_derivative_rl(sint32 ticks,float32 Ts);
static float32 get_derivative_rr(sint32 ticks,float32 Ts);
static float32 saturation(float32 bottom, float32 high, float32 target);
static float32 get_vin(float32 dutycycle);
/***********************************************************************/

/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
setrpm g_motor;
/***********************************************************************/
/*Function*/ 
/***********************************************************************/

//need integral -> Ts=0.001
estimate_state_var observer_theta_fl(float32 Ts){ //frontLeft motor theta observer(goal : follow theta_hat)
    static float32 theta_hat=0; //init value : 0 as estimate value , integral value
    static float32 omega_hat=0; //init value : 0 as estimate value
    static float32 current_hat=0; //init value : 0 as estimate value
    static float32 theta_tilde=0; //Estimate error (goal: theta_tilde=0)

    //integral
    static float32 theta_hat_old=0;
    static float32 omega_hat_old=0;
    static float32 current_hat_old=0;

    //�̰� �´��� ��
    // Previous inputs for HPF
    //static float32 previous_theta_hat_input = 0;
    //static float32 previous_omega_hat_input = 0;
    //static float32 previous_current_hat_input = 0;

    //theta_tilde=(theta:encoder's ticks)-theta_hat;
    theta_tilde=(float32)(get_wheelFL_tick())-theta_hat;

    theta_hat=theta_hat_old+ (omega_hat+(L1)*theta_tilde)*Ts;
    omega_hat=omega_hat_old+(current_hat*(Kt/J)-omega_hat*(B/J)+theta_tilde*(L2))*Ts;
    //explicit input : get_vin(control_output.fl)
    current_hat=current_hat_old+(-current_hat*(R/L)-omega_hat*(Kb/L)+(1/L)*get_vin(control_output.fl)+(L3)*theta_tilde)*Ts;

    theta_hat=LPF(theta_hat_old,theta_hat,100,Ts);
    omega_hat=LPF(omega_hat_old,omega_hat,100,Ts);
    current_hat=LPF(current_hat_old,current_hat,100,Ts);


    theta_hat_old=theta_hat;
    omega_hat_old=omega_hat;
    current_hat_old=current_hat;

    g_estimate_state_var.theta_hat=theta_hat;
    g_estimate_state_var.omega_hat=omega_hat;
    g_estimate_state_var.current_hat=current_hat;
    g_estimate_state_var.theta_tilde=theta_tilde;

    return g_estimate_state_var; //goal: theta_tilde->0 (error->0)
}
void opened_loop_control(uint32 ms)
{
    set_wheelFL_dutycycle((float32)((float32)g_ref_rpm.fl/ RPM_MAX * 100.0));
    //set_wheelFR_dutycycle((float32)((float32)g_ref_rpm.fr/ RPM_MAX * 100.0));
    float32 duty = ms/50000.0 * 100; // 10s -> 100duty
    if(duty >= 100){
        duty = 100;
    }
    //g_cur_rpm.fl=get_derivative(get_wheelFL_tick(),0.001)*(60/22.0);
    //set_wheelFL_dutycycle(duty);
    //set_wheelRR_dutycycle((float32)((float32)g_ref_rpm.rr/ RPM_MAX * 100.0));
}
void closed_loop_control(float32 kp, float32 ki, float32 Ts)
{
    //get current rpm from encoder ticks
    //����ȯ �ʿ�?
    g_cur_rpm.fl=get_derivative_fl(get_wheelFL_tick(),Ts)*(60/22.0); // multiply rpm scailing 60/22.0

    g_cur_rpm.fr=get_derivative_fr(get_wheelFR_tick(),Ts)*(60/22.0); // multiply rpm scailing
   // g_cur_rpm.rl=get_derivative_rl(get_wheelRL_tick(),Ts)*(60/22.0); // error->
    g_cur_rpm.rl=get_derivative_rl(get_wheelFR_tick(),Ts)*(60/22.0); // multiply rpm scailing
    g_cur_rpm.rr=get_derivative_rr(get_wheelRR_tick(),Ts)*(60/22.0); // multiply rpm scailing

    //watch only fl

    //usb_printf("g_ref_rpm.fr:%lf, g_cur_rpm.fr:%lf\n", g_ref_rpm.fr,g_cur_rpm.fr);
    //usb_printf("g_ref_rpm.rl:%lf, g_cur_rpm.rl:%lf\n", g_ref_rpm.rl,g_cur_rpm.rl);
    //usb_printf("g_ref_rpm.rl:%lf, g_cur_rpm.rl:%lf\n", g_ref_rpm.rl,g_cur_rpm.rl);

    //PID

    //fl
    g_errors.fl_err = g_ref_rpm.fl - g_cur_rpm.fl;
    g_errors.fl_I_err = g_errors_old.fl_I_err_old + (g_errors.fl_err) * Ts; // integral

    g_errors_old.fl_I_err_old=g_errors.fl_I_err; //save
    g_errors_old.fl_err_old=g_errors.fl_err;    //save

   // g_errors.fl_I_err=saturation(-100.0, 100.0, g_errors.fl_I_err);


    //fr
    g_errors.fr_err = g_ref_rpm.fr - g_cur_rpm.fr;
    g_errors.fr_I_err = g_errors_old.fr_I_err_old + (g_errors.fr_err) * Ts; // integral

    g_errors_old.fr_I_err_old=g_errors.fr_I_err; //save
    g_errors_old.fr_err_old=g_errors.fr_err;    //save

    //g_errors.fr_I_err=saturation(-100.0, 100.0, g_errors.fr_I_err);

    //rl
    g_errors.rl_err = g_ref_rpm.rl - g_cur_rpm.rl;
    g_errors.rl_I_err = g_errors_old.rl_I_err_old + (g_errors.rl_err) * Ts; // integral

    g_errors_old.rl_I_err_old=g_errors.rl_I_err; //save
    g_errors_old.rl_err_old=g_errors.rl_err;    //save

    //g_errors.rl_I_err=saturation(-100.0, 100.0, g_errors.rl_I_err);

    //rr
    g_errors.rr_err = g_ref_rpm.rr - g_cur_rpm.rr;
    g_errors.rr_I_err = g_errors_old.rr_I_err_old + (g_errors.rr_err) * Ts; // integral

    g_errors_old.rr_I_err_old=g_errors.rr_I_err; //save
    g_errors_old.rr_err_old=g_errors.rr_err;    //save

   // g_errors.rr_I_err=saturation(-100.0, 100.0, g_errors.rr_I_err);

    //����ȯ �ʿ�?
    control_output.fl = kp * (g_errors.fl_err) + ki * (g_errors.fl_I_err); //return
    control_output.fr = kp * (g_errors.fr_err) + ki * (g_errors.fr_I_err);//return
    control_output.rl = kp * (g_errors.rl_err) + ki * (g_errors.rl_I_err);//return
    control_output.rr = kp * (g_errors.rr_err) + ki * (g_errors.rr_I_err);//return

   // usb_printf("cont: %lf, fl_err:%lf,fl_i_err:%lf\n",control_output.fl,
  //          g_errors.fl_err, g_errors.fl_I_err);
    control_output.fl=saturation(-100.0, 100.0, control_output.fl);
    control_output.fr=saturation(-100.0, 100.0, control_output.fr);
    control_output.rl=saturation(-100.0, 100.0, control_output.rl);
    control_output.rr=saturation(-100.0, 100.0, control_output.rr);

    set_wheelFL_dutycycle(control_output.fl);
    set_wheelFR_dutycycle(control_output.fr);
    set_wheelRL_dutycycle(control_output.rl);
    set_wheelRR_dutycycle(control_output.rr);
    return;
}

/*- state 0 ~ 10
      - 0: Northwestward    1: Northward    2: Northeastward
      - 3: Westward         4: Stop         5: Eastward
      - 6: Southwestward    7: Southward    8: Southeastward

      - 9: Counter-Clockwise    10: Clockwise

    sub_state 0~2
      - 0 : normal          1: slow         2: stop
*/
void set_all_wheel(uint8 state, uint8 sub_state, float32 goal_rpm)
{

    //fl_rpm
    if(state ==0 || state ==4|| state==8){
        g_motor.fl_rpm = 0;
    }else if(state ==1 ||state ==2||state ==5||state ==10){
        IfxPort_setPinLow(IfxPort_P10_1.port, IfxPort_P10_1.pinIndex); //순방향
        if(sub_state ==1) g_motor.fl_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.fl_rpm=0;
    }else {
        IfxPort_setPinHigh(IfxPort_P10_1.port, IfxPort_P10_1.pinIndex); //역방향
        if(sub_state ==1) g_motor.fl_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.fl_rpm=0;
    }

    //fr_rpm
    if(state ==2 || state ==4|| state==6){
        g_motor.fr_rpm = 0;
    }else if(state ==0 ||state ==1||state ==3||state ==9){
        IfxPort_setPinLow(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex); //순방향
        if(sub_state ==1) g_motor.fr_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.fr_rpm=0;
    }else {
        IfxPort_setPinHigh(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex); //역방향
        if(sub_state ==1) g_motor.fr_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.fr_rpm=0;
    }

    //rl_rpm
    if(state ==2 || state ==4|| state==6){
        g_motor.rl_rpm = 0;
    }else if(state ==0 ||state ==1||state ==3||state ==10){
        IfxPort_setPinLow(IfxPort_P21_0.port, IfxPort_P21_0.pinIndex); //순방향
        if(sub_state ==1) g_motor.rl_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.rl_rpm=0;
    }else {
        IfxPort_setPinHigh(IfxPort_P21_0.port, IfxPort_P21_0.pinIndex); //역방향
        if(sub_state ==1) g_motor.rl_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.rl_rpm=0;
    }

    //rr_rpm
    if(state ==0 || state ==4 ||state==8){
        g_motor.rr_rpm = 0;
    }else if(state ==1 ||state ==2||state ==5||state ==9){
        IfxPort_setPinLow(IfxPort_P33_1.port, IfxPort_P33_1.pinIndex); //순방향
        if(sub_state ==1) g_motor.rr_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.rr_rpm=0;
    }else {
        IfxPort_setPinHigh(IfxPort_P33_1.port, IfxPort_P33_1.pinIndex); //역방향
        if(sub_state ==1) g_motor.rr_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.rr_rpm=0;
    }
}
static float32 get_derivative_fl(sint32 ticks,float32 Ts){
    static sint32 ticks_old=0;
    static float32 w_old=0;
    float32 w=0;
    sint32 cutoff = 70;
    if(ticks > 0)
        cutoff = 10;

    w=(ticks-ticks_old)/Ts;

    w=LPF(w_old,w,cutoff,Ts); //500: freq ==> need change& tunning
    //usb_printf("ticks:%d, ticks_old:%d, w: %lf\n,",ticks,ticks_old,w);

    ticks_old=ticks;
    w_old=w;

    return w;
}
static float32 get_derivative_fr(sint32 ticks,float32 Ts){
    static sint32 ticks_old=0;
    static float32 w_old=0;
    float32 w=0;
    sint32 cutoff = 70;
    if(ticks > 0)
        cutoff = 10;

    w=(ticks-ticks_old)/Ts;

    w=LPF(w_old,w,cutoff,Ts); //500: freq ==> need change& tunning
    //usb_printf("ticks:%d, ticks_old:%d, w: %lf\n,",ticks,ticks_old,w);

    ticks_old=ticks;
    w_old=w;

    return w;
}
static float32 get_derivative_rl(sint32 ticks,float32 Ts){
    static sint32 ticks_old=0;
    static float32 w_old=0;
    float32 w=0;
    sint32 cutoff = 70;
    if(ticks > 0)
        cutoff = 10;

    w=(ticks-ticks_old)/Ts;

    w=LPF(w_old,w,cutoff,Ts); //500: freq ==> need change& tunning
    //usb_printf("ticks:%d, ticks_old:%d, w: %lf\n,",ticks,ticks_old,w);

    ticks_old=ticks;
    w_old=w;

    return w;
}
static float32 get_derivative_rr(sint32 ticks,float32 Ts){
    static sint32 ticks_old=0;
    static float32 w_old=0;
    float32 w=0;
    sint32 cutoff = 70;
    if(ticks > 0)
        cutoff = 10;




    return w;
}
static float32 saturation(float32 bottom, float32 high, float32 target){
    if(target < bottom)
        return bottom;
    else if(target > high)
        return high;
    return target;
}
static float32 LPF(float32 Y_fill_d,float32 u,float32 cf,float32 T){ // cf=cutoff
    return (1-T*cf)*Y_fill_d+T*cf*u;
}

static float32 get_vin(float32 dutycycle){ // input example: control_output.fl
    float32 vin;
    vin=dutycycle/100.0*12; //absolute -> scailing-> voltage(0~12V)
    return vin;
}
