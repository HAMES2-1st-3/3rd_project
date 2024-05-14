/*
 * Control_.c
 *
 *  Created on: 2024. 5. 13.
 *      Author: taewon
 *
 *  �ʿ� �Լ� �� ���
 *    void FUNC(int32 state, int32 sub_state, float32 ref_rpm);
 *     - ref_rpm�� ���� ��ǥ rpm ����
 *     - state�� ���� �װ��� ���� ������ rpm �� ���� ����
 *     - sub_state�� ���� slow, stop�� ��� rpm �� ���� �缳��
 *
 *    wheel_rpms PID(wheel_rpms g_ref_rpm, wheel_rpms g_cur_rpm, float32 kp, float32 ki, float32 Ts)
 *     - PI ���� �Լ�
 *
 *    float32 LPF(float32 Y_prev,float32 u,float32 cf,float32 T){ // cf=cutoff
 *     - Low-Pass Filter ����
 *
 *     -> Driver_1.h, Driver_2.h, Driver_3.h, Driver_4.h
 */


/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Control_Motor.h"
#include <stdint.h>
/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct{ //value of reference RPM for each wheels
        sint32 fl;
        sint32 fr;
        sint32 rr;
        sint32 rl;
}wheel_rpms;

typedef struct {
    float32 fl_err;
    float32 fr_err;
    float32 rr_err;
    float32 rl_err;

    float32 fl_I_err;
    float32 fr_I_err;
    float32 rr_I_err;
    float32 rl_I_err;
} control_errors;

/***********************************************************************/
/*Static Function Prototype*/
/***********************************************************************/
void update_wheel_rpm(wheel_rpms sensor_rpm, float32 cf, float32 T);
float32 LPF(float32 Y_prev, float32 u, float32 cf, float32 T);
wheel_rpms PID(wheel_rpms g_ref_rpm, wheel_rpms g_cur_rpm, float32 kp, float32 ki, float32 Ts);

/***********************************************************************/

/*Variable*/
/***********************************************************************/

static control_errors g_errors = {0};// �� ������ ���� ������ ���������� �����ϱ� ���� ����ü
static wheel_rpms g_filtered_rpm;// value after LPF
static wheel_rpms g_ref_rpm;//value of reference RPM for each wheels
static wheel_rpms g_cur_rpm;//value of current RPM for each wheels
wheel_rpms control_output;//value of duty cycle for each wheels

/***********************************************************************/

/*Function*/ 
/***********************************************************************/

void update_wheel_rpm(wheel_rpms sensor_rpm, float32 cf, float32 T) {
    // update LPF to 4 wheels
    g_filtered_rpm.fl = LPF(g_filtered_rpm.fl, sensor_rpm.fl, cf, T);
    g_filtered_rpm.fr = LPF(g_filtered_rpm.fr, sensor_rpm.fr, cf, T);
    g_filtered_rpm.rl = LPF(g_filtered_rpm.rl, sensor_rpm.rl, cf, T);
    g_filtered_rpm.rr = LPF(g_filtered_rpm.rr, sensor_rpm.rr, cf, T);
}


float32 LPF(float32 Y_prev, float32 u, float32 cf, float32 T){
    // RC: 시간 상수 -> 필터의 응답 속도를 결정. RC 값이 크면 필터가 저주파에 더 민감해짐.
    // cf (cutoff frequency)가 작을수록 필터의 응답이 느려짐. 단위는 초(sec).
    float32 RC = 1.0 / (2.0 * 3.14159 * cf);

    // alpha는 필터 계수로, 0과 1 사이의 값 -> 필터의 감도를 결정
    // T가 작거나 같은 주기적인 샘플링에서 cf에 비해 충분히 작으면 alpha 값이 커져 필터의 효과가 감소
    float32 alpha = T / (T + RC);
    // Y_curr은 현재 필터된 출력값 -> 이전 출력값 Y_prev와 입력값 u 사이의 가중 평균으로 계산
    // alpha가 1에 가까울수록 입력 u에 더 가까워짐
    float32 Y_curr = Y_prev + alpha * (u - Y_prev);

    return Y_curr;
}



void pi_control(float32 kp, float32 ki, float32 Ts){

    g_errors.fl_err = g_ref_rpm.fl - g_cur_rpm.fl;
    g_errors.fl_I_err += g_errors.fl_err * Ts;
    control_output.fl = kp * g_errors.fl_err + ki * g_errors.fl_I_err;

    //
    g_errors.fr_err = g_ref_rpm.fr - g_cur_rpm.fr;
    g_errors.fr_I_err += g_errors.fr_err * Ts;
    control_output.fr = kp * g_errors.fr_err + ki * g_errors.fr_I_err;

    //
    g_errors.rl_err = g_ref_rpm.rl - g_cur_rpm.rl;
    g_errors.rl_I_err += g_errors.rl_err * Ts;
    control_output.rl = kp * g_errors.rl_err + ki * g_errors.rl_I_err;

    //
    g_errors.rr_err = g_ref_rpm.rr - g_cur_rpm.rr;
    g_errors.rr_I_err += g_errors.rr_err * Ts;
    control_output.rr = kp * g_errors.rr_err + ki * g_errors.rr_I_err;

    set_wheelFL_dutycycle((float32)(control_output.fl + (float32)g_ref_rpm.fl/ 5300 * 100.0));
    set_wheelFR_dutycycle((float32)(control_output.fr + (float32)g_ref_rpm.fr/ 5300 * 100.0));
    set_wheelRL_dutycycle((float32)(control_output.rl + (float32)g_ref_rpm.rl/ 5300 * 100.0));
    set_wheelRR_dutycycle((float32)(control_output.rr + (float32)g_ref_rpm.rr/ 5300 * 100.0));


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


void set_all_wheel(uint8 state, uint8 sub_state, sint32 goal_rpm)
{

    //fl_rpm
    if(state ==0 || state ==4|| state==8){
        g_ref_rpm.fl = 0;
    }else if(state ==1 ||state ==2||state ==5||state ==10){
        //IfxPort_setPinLow(IfxPort_P10_1.port, IfxPort_P10_1.pinIndex); //������
        if(sub_state ==1) g_ref_rpm.fl = goal_rpm/2;
        else if(sub_state ==2)g_ref_rpm.fl=0;
        else g_ref_rpm.fl = goal_rpm;
    }else {
        //IfxPort_setPinHigh(IfxPort_P10_1.port, IfxPort_P10_1.pinIndex); //������
        if(sub_state ==1) g_ref_rpm.fl = goal_rpm/2 * -1;
        else if(sub_state ==2)g_ref_rpm.fl=0;
        else g_ref_rpm.fl = goal_rpm * -1;
    }

    //fr_rpm
    if(state ==2 || state ==4|| state==6){
        g_ref_rpm.fr = 0;
    }else if(state ==0 ||state ==1||state ==3||state ==9){
       // IfxPort_setPinLow(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex); //������
        if(sub_state ==1) g_ref_rpm.fr = goal_rpm/2;
        else if(sub_state ==2)g_ref_rpm.fr=0;
        else g_ref_rpm.fr = goal_rpm;
    }else {
        //IfxPort_setPinHigh(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex); //������
        if(sub_state ==1) g_ref_rpm.fr = goal_rpm/2 *-1;
        else if(sub_state ==2)g_ref_rpm.fr=0;
        else g_ref_rpm.fr = goal_rpm *-1;
    }

    //rl_rpm
    if(state ==2 || state ==4|| state==6){
        g_ref_rpm.rl = 0;
    }else if(state ==0 ||state ==1||state ==3||state ==10){
        //IfxPort_setPinLow(IfxPort_P21_0.port, IfxPort_P21_0.pinIndex); //������
        if(sub_state ==1) g_ref_rpm.rl = goal_rpm/2;
        else if(sub_state ==2)g_ref_rpm.rl=0;
        else g_ref_rpm.rl = goal_rpm;
    }else {
        //IfxPort_setPinHigh(IfxPort_P21_0.port, IfxPort_P21_0.pinIndex); //������
        if(sub_state ==1) g_ref_rpm.rl = goal_rpm/2 *-1;
        else if(sub_state ==2)g_ref_rpm.rl=0;
        else g_ref_rpm.rl = goal_rpm *-1;
    }

    //rr_rpm
    if(state ==0 || state ==4 ||state==8){
        g_ref_rpm.rr = 0;
    }else if(state ==1 ||state ==2||state ==5||state ==9){
        //IfxPort_setPinLow(IfxPort_P33_1.port, IfxPort_P33_1.pinIndex); //������
        if(sub_state ==1) g_ref_rpm.rr = goal_rpm/2;
        else if(sub_state ==2)g_ref_rpm.rr=0;
        else g_ref_rpm.rr = goal_rpm;
    }else {
        //IfxPort_setPinHigh(IfxPort_P33_1.port, IfxPort_P33_1.pinIndex); //������
        if(sub_state ==1) g_ref_rpm.rr = goal_rpm/2 *-1;
        else if(sub_state ==2)g_ref_rpm.rr=0;
        else g_ref_rpm.rr = goal_rpm *-1;
    }

    g_cur_rpm = g_ref_rpm; //Test

}
