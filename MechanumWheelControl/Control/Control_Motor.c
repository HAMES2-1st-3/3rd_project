/*
 * Control_.c
 *
 *  Created on: 2024. 5. 10.
 *      Author: user
 *
 *  �븘�슂 �븿�닔 諛� 湲곕뒫
 *    void FUNC(int32 state, int32 sub_state, float32 ref_rpm);
 *     - ref_rpm瑜� �넻�빐 紐⑺몴 rpm �꽕�젙
 *     - state瑜� �넻�빐 �꽕媛쒖쓽 紐⑦꽣 媛곴컖�쓽 rpm 諛� 諛⑺뼢 �꽕�젙
 *     - sub_state瑜� �넻�빐 slow, stop�씤 寃쎌슦 rpm 諛� 諛⑺뼢 �옱�꽕�젙
 *
 *    float32 PID(float32 ref, float32 cur, float32 kp, float32 ki, float32 Ts)
 *     - PI �젣�뼱 �븿�닔
 *
 *    float32 LPF(float32 Y_fill_d,float32 u,float32 cf,float32 T){ // cf=cutoff
 *     - Low-Pass Filter 援ы쁽
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
#define CUT_DEFAULT 60
#define CUT_FOR_RL  30
/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/

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

/*Variable*/
/***********************************************************************/
/*
 * fl : front left
 * fr : front right
 * rl : rear left
 * rr : rear right
 * */
static control_errors g_errors = {0};// errors and Integral errors for fl,fr,rl,rr
static control_errors_old g_errors_old={0,};// previous errors and Integral errors for fl,fr,rl,rr

wheel_rpms g_ref_rpm;//value of reference RPM for each wheels
wheel_rpms g_cur_rpm;//value of current RPM for each wheels
wheel_rpms control_output;//value of duty cycle for each wheels   : each one value : -100~100

//values related to observer
float32 L1=21.8017;
float32 L2= 41.5731;
float32 L3=-0.5167;

float32 J=2.09*e-5;
float32 Kb=0.042;
float32 Kt=0.042;
float32 R=8.4;
float32 L=1.16;
float32 B=0.00002;
//estimate state variable
estimate_state_var g_estimate_state_var;
/***********************************************************************/

/*Function*/ 
/***********************************************************************/
/*Function*/
// func call flow : set_all_wheel() ->closed_loop_control()->observer_theta_fl()
// goal :  check if theta_tilde value(=estimate error) is 0
/***********************************************************************/
//make observers

//observer for theta about front left motor
//only call this func on 20ms
estimate_state_var observer_theta_fl(float32 Ts){ //frontLeft motor theta observer(goal : follow theta_hat)
    static float32 theta_hat=0; //init value : 0 as estimate value , integral value
    static float32 omega_hat=0; //init value : 0 as estimate value
    static float32 current_hat=0; //init value : 0 as estimate value
    static float32 theta_tilde=0; //Estimate error (goal: theta_tilde=0)

    //previous value for integral
    static float32 theta_hat_old=0;
    static float32 omega_hat_old=0;
    static float32 current_hat_old=0;

    //theta_tilde=(theta:encoder's ticks)-theta_hat;
    theta_tilde=(float32)(get_wheelFL_tick())-theta_hat;

    //estimate state variable(hat)
    theta_hat=theta_hat_old+ (omega_hat+(L1)*theta_tilde)*Ts;
    omega_hat=omega_hat_old+(current_hat*(Kt/J)-omega_hat*(B/J)+theta_tilde*(L2))*Ts;
    //explicit input : get_vin(control_output.fl)
    current_hat=current_hat_old+(-current_hat*(R/L)-omega_hat*(Kb/L)+(1/L)*get_vin(control_output.fl)+(L3)*theta_tilde)*Ts;

    //remove noises
    theta_hat=LPF(theta_hat_old,theta_hat,100,Ts); //tuning plz
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
//func for open-loop
void opened_loop_control(uint32 ms)
{
    set_wheelFL_dutycycle((float32)((float32)g_ref_rpm.fl/ RPM_MAX * 100.0));
    set_wheelFR_dutycycle((float32)((float32)g_ref_rpm.fr/ RPM_MAX * 100.0));
//    float32 duty = ms/50000.0 * 100; // 10s -> 100duty
//    if(duty >= 100){
//        duty = 100;
//    }
    //g_cur_rpm.fl=get_derivative(get_wheelFL_tick(),0.001)*(60/22.0);
    set_wheelRL_dutycycle((float32)((float32)g_ref_rpm.rl/ RPM_MAX * 100.0));
    set_wheelRR_dutycycle((float32)((float32)g_ref_rpm.rr/ RPM_MAX * 100.0));
}
//func for closed-loop :  Kp ,Ki is fixed value
void closed_loop_control(float32 kp, float32 ki, float32 Ts)
{
    //get current rpm from encoder ticks
    // ticks -> derivative
    g_cur_rpm.fl=get_derivative_fl(get_wheelFL_tick(),Ts)*(60/22.0); // multiply rpm scailing 60/22.0

    g_cur_rpm.fr=get_derivative_fr(get_wheelFR_tick(),Ts)*(60/22.0); // multiply rpm scailing
    g_cur_rpm.rl=get_derivative_rl(get_wheelRL_tick(),Ts)*(60/22.0); // multiply rpm scailing
    //g_cur_rpm.rl=get_derivative_rl(get_wheelFR_tick(),Ts)*(60/22.0); // multiply rpm scailing  bring fr data
    g_cur_rpm.rr=get_derivative_rr(get_wheelRR_tick(),Ts)*(60/22.0); // multiply rpm scailing

    //watch only fl

    //usb_printf("g_ref_rpm.fr:%lf, g_cur_rpm.fr:%lf\n", g_ref_rpm.fr,g_cur_rpm.fr);
    //usb_printf("g_ref_rpm.rl:%lf, g_cur_rpm.rl:%lf\n", g_ref_rpm.rl,g_cur_rpm.rl);
    //usb_printf("g_ref_rpm.rl:%lf, g_cur_rpm.rl:%lf\n", g_ref_rpm.rl,g_cur_rpm.rl);

    //PID

    //errors fl
    g_errors.fl_err = g_ref_rpm.fl - g_cur_rpm.fl;
    g_errors.fl_I_err = g_errors_old.fl_I_err_old + (g_errors.fl_err) * Ts; // integral

    g_errors_old.fl_I_err_old=g_errors.fl_I_err; //save
    g_errors_old.fl_err_old=g_errors.fl_err;    //save

    //g_errors.fl_I_err=saturation(-130.0, 130.0, g_errors.fl_I_err);


    //errors fr
    g_errors.fr_err = g_ref_rpm.fr - g_cur_rpm.fr;
    g_errors.fr_I_err = g_errors_old.fr_I_err_old + (g_errors.fr_err) * Ts; // integral

    g_errors_old.fr_I_err_old=g_errors.fr_I_err; //save
    g_errors_old.fr_err_old=g_errors.fr_err;    //save

    //g_errors.fr_I_err=saturation(-150.0, 150.0, g_errors.fr_I_err);

    //errors rl
    g_errors.rl_err = g_ref_rpm.rl - g_cur_rpm.rl;
    g_errors.rl_I_err = g_errors_old.rl_I_err_old + (g_errors.rl_err) * Ts; // integral

    g_errors_old.rl_I_err_old=g_errors.rl_I_err; //save
    g_errors_old.rl_err_old=g_errors.rl_err;    //save

    //g_errors.rl_I_err=saturation(-140.0, 140.0, g_errors.rl_I_err);

    //errors rr
    g_errors.rr_err = g_ref_rpm.rr - g_cur_rpm.rr;
    g_errors.rr_I_err = g_errors_old.rr_I_err_old + (g_errors.rr_err) * Ts; // integral

    g_errors_old.rr_I_err_old=g_errors.rr_I_err; //save
    g_errors_old.rr_err_old=g_errors.rr_err;    //save

    //g_errors.rr_I_err=saturation(-130.0, 130.0, g_errors.rr_I_err); //limit error difference value

    // make output using Kp Ki
    control_output.fl = kp * (g_errors.fl_err) + ki * (g_errors.fl_I_err); //return
    control_output.fr = kp * (g_errors.fr_err) + ki * (g_errors.fr_I_err);//return
    control_output.rl = kp * (g_errors.rl_err) + ki * (g_errors.rl_I_err);//return
    control_output.rr = kp * (g_errors.rr_err) + ki * (g_errors.rr_I_err);//return

   // usb_printf("cont: %lf, fl_err:%lf,fl_i_err:%lf\n",control_output.fl,
  //          g_errors.fl_err, g_errors.fl_I_err);
    //saturation due to dutycycle range : -100~100
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

// make all motors have ref_rpm depending on state and substate
void set_all_wheel(uint8 state, uint8 sub_state, float32 goal_rpm)
{

    //fl_rpm
    if(state ==0 || state ==4|| state==8){
        g_ref_rpm.fl = 0;
    }else if(state ==1 ||state ==2||state ==5||state ==10){
        //IfxPort_setPinLow(IfxPort_P10_1.port, IfxPort_P10_1.pinIndex); //占쏙옙占쏙옙占쏙옙
        if(sub_state ==1) g_ref_rpm.fl = goal_rpm/2.0;
        else if(sub_state ==2)g_ref_rpm.fl=0;
        else g_ref_rpm.fl = goal_rpm;
    }else {
        //IfxPort_setPinHigh(IfxPort_P10_1.port, IfxPort_P10_1.pinIndex); //占쏙옙占쏙옙占쏙옙
        if(sub_state ==1) g_ref_rpm.fl = goal_rpm/2.0 * (-1);
        else if(sub_state ==2)g_ref_rpm.fl=0;
        else g_ref_rpm.fl = goal_rpm * (-1);
    }

    //fr_rpm
    if(state ==2 || state ==4|| state==6){
        g_ref_rpm.fr = 0;
    }else if(state ==0 ||state ==1||state ==3||state ==9){
       // IfxPort_setPinLow(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex); //占쏙옙占쏙옙占쏙옙
        if(sub_state ==1) g_ref_rpm.fr = goal_rpm/2.0;
        else if(sub_state ==2)g_ref_rpm.fr=0;
        else g_ref_rpm.fr = goal_rpm;
    }else {
        //IfxPort_setPinHigh(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex); //占쏙옙占쏙옙占쏙옙
        if(sub_state ==1) g_ref_rpm.fr = goal_rpm/2.0 *(-1);
        else if(sub_state ==2)g_ref_rpm.fr=0;
        else g_ref_rpm.fr = goal_rpm *(-1);
    }

    //rl_rpm
    if(state ==2 || state ==4|| state==6){
        g_ref_rpm.rl = 0;
    }else if(state ==0 ||state ==1||state ==3||state ==10){
        //IfxPort_setPinLow(IfxPort_P21_0.port, IfxPort_P21_0.pinIndex); //占쏙옙占쏙옙占쏙옙
        if(sub_state ==1) g_ref_rpm.rl = goal_rpm/2.0;
        else if(sub_state ==2)g_ref_rpm.rl=0;
        else g_ref_rpm.rl = goal_rpm;
    }else {
        //IfxPort_setPinHigh(IfxPort_P21_0.port, IfxPort_P21_0.pinIndex); //占쏙옙占쏙옙占쏙옙
        if(sub_state ==1) g_ref_rpm.rl = goal_rpm/2.0 *(-1);
        else if(sub_state ==2)g_ref_rpm.rl=0;
        else g_ref_rpm.rl = goal_rpm *(-1);
    }

    //rr_rpm
    if(state ==0 || state ==4 ||state==8){
        g_ref_rpm.rr = 0;
    }else if(state ==1 ||state ==2||state ==5||state ==9){
        //IfxPort_setPinLow(IfxPort_P33_1.port, IfxPort_P33_1.pinIndex); //占쏙옙占쏙옙占쏙옙
        if(sub_state ==1) g_ref_rpm.rr = goal_rpm/2.0;
        else if(sub_state ==2)g_ref_rpm.rr=0;
        else g_ref_rpm.rr = goal_rpm;
    }else {
        //IfxPort_setPinHigh(IfxPort_P33_1.port, IfxPort_P33_1.pinIndex); //占쏙옙占쏙옙占쏙옙
        if(sub_state ==1) g_ref_rpm.rr = goal_rpm/2.0 *(-1);
        else if(sub_state ==2)g_ref_rpm.rr=0;
        else g_ref_rpm.rr = goal_rpm *(-1);
    }


    //g_cur_rpm = g_ref_rpm; //Test
    //g_cur_rpm.fl=get_derivative(get_encoderFL_tick())*( rpm 占쏙옙占쏙옙占싹몌옙 占쏙옙占쏙옙);
}
//derivative for ticks + remove noise
static float32 get_derivative_fl(sint32 ticks,float32 Ts){
    static sint32 ticks_old=0;
    static float32 w_old=0;
    float32 w=0;
    sint32 cutoff = CUT_DEFAULT;
    //if(ticks > 0)
     //   cutoff = 10;

    w=(ticks-ticks_old)/Ts;

    w=LPF(w_old,w,(float32)cutoff,Ts); //500: freq ==> need change& tunning
    //usb_printf("ticks:%d, ticks_old:%d, w: %lf\n,",ticks,ticks_old,w);

    ticks_old=ticks;
    w_old=w;

    return w;
}
//derivative for ticks + remove noise
static float32 get_derivative_fr(sint32 ticks,float32 Ts){
    static sint32 ticks_old=0;
    static float32 w_old=0;
    float32 w=0;
    sint32 cutoff = CUT_DEFAULT;
//   / if(ticks > 0)
   //     cutoff = 10;

    w=(ticks-ticks_old)/Ts;

    w=LPF(w_old,w,(float32)cutoff,Ts); //500: freq ==> need change& tunning
    //usb_printf("ticks:%d, ticks_old:%d, w: %lf\n,",ticks,ticks_old,w);

    ticks_old=ticks;
    w_old=w;

    return w;
}
//derivative for ticks + remove noise
static float32 get_derivative_rl(sint32 ticks,float32 Ts){
    static sint32 ticks_old=0;
    static float32 w_old=0;
    float32 w=0;
    sint32 cutoff = CUT_DEFAULT;
    if(ticks > 0)
       cutoff = CUT_FOR_RL;

    w=(ticks-ticks_old)/Ts;

    w=LPF(w_old,w,(float32)cutoff,Ts); //500: freq ==> need change& tunning
    //usb_printf("ticks:%d, ticks_old:%d, w: %lf\n,",ticks,ticks_old,w);

    ticks_old=ticks;
    w_old=w;

    return w;
}
//derivative for ticks + remove noise
static float32 get_derivative_rr(sint32 ticks,float32 Ts){
    static sint32 ticks_old=0;
    static float32 w_old=0;
    float32 w=0;
    sint32 cutoff = CUT_DEFAULT;
   // if(ticks > 0)
     //   cutoff = 10;

    w=(ticks-ticks_old)/Ts;

    w=LPF(w_old,w,(float32)cutoff,Ts); //500: freq ==> need change& tunning
    //usb_printf("ticks:%d, ticks_old:%d, w: %lf\n,",ticks,ticks_old,w);

    ticks_old=ticks;
    w_old=w;

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

//expect Vin(Volatage) value from dutycycle
static float32 get_vin(float32 dutycycle){ // input example: control_output.fl
    float32 vin;
    vin=dutycycle/100.0*12; //absolute -> scailing-> voltage(0~12V)
    return vin;
}
