/*
 * Control_.c
 *
 *  Created on: 2024. 5. 10.
 *      Author: user
 *
 *  �ʿ� �Լ� �� ���
 *    void FUNC(int32 state, int32 sub_state, float32 ref_rpm);
 *     - ref_rpm�� ���� ��ǥ rpm ����
 *     - state�� ���� �װ��� ���� ������ rpm �� ���� ����
 *     - sub_state�� ���� slow, stop�� ��� rpm �� ���� �缳��
 *
 *    float32 PID(float32 ref, float32 cur, float32 kp, float32 ki, float32 Ts)
 *     - PI ���� �Լ�
 *
 *    float32 LPF(float32 Y_fill_d,float32 u,float32 cf,float32 T){ // cf=cutoff
 *     - Low-Pass Filter ����
 *
 *     -> Driver_1.h, Driver_2.h, Driver_3.h, Driver_4.h
 */


/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Control_Motor.h"

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct{ //���� ����
        uint32 fl_rpm;
        uint32 fr_rpm;
        uint32 rr_rpm;
        uint32 rl_rpm;
}setrpm;
/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/

/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
setrpm g_motor;
/***********************************************************************/
/*Function*/ 
/***********************************************************************/

void set_all_wheel(uint8 state, uint8 sub_state, uint32 ref_rpm)
{
    //fl_rpm
    if(state ==0 || state ==4|| state==8){
        g_motor.fl_rpm = 0;
    }else if(state ==1 ||state ==2||state ==5||state ==10){
        IfxPort_setPinLow(IfxPort_P10_1.port, IfxPort_P10_1.pinIndex); //������
        if(sub_state ==1) g_motor.fl_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.fl_rpm=0;
    }else {
        IfxPort_setPinHigh(IfxPort_P10_1.port, IfxPort_P10_1.pinIndex); //������
        if(sub_state ==1) g_motor.fl_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.fl_rpm=0;
    }

    //fr_rpm
    if(state ==2 || state ==4|| state==6){
        g_motor.fr_rpm = 0;
    }else if(state ==0 ||state ==1||state ==3||state ==9){
        IfxPort_setPinLow(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex); //������
        if(sub_state ==1) g_motor.fr_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.fr_rpm=0;
    }else {
        IfxPort_setPinHigh(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex); //������
        if(sub_state ==1) g_motor.fr_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.fr_rpm=0;
    }

    //rl_rpm
    if(state ==2 || state ==4|| state==6){
        g_motor.rl_rpm = 0;
    }else if(state ==0 ||state ==1||state ==3||state ==10){
        IfxPort_setPinLow(IfxPort_P21_0.port, IfxPort_P21_0.pinIndex); //������
        if(sub_state ==1) g_motor.rl_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.rl_rpm=0;
    }else {
        IfxPort_setPinHigh(IfxPort_P21_0.port, IfxPort_P21_0.pinIndex); //������
        if(sub_state ==1) g_motor.rl_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.rl_rpm=0;
    }

    //rr_rpm
    if(state ==0 || state ==4 ||state==8){
        g_motor.rr_rpm = 0;
    }else if(state ==1 ||state ==2||state ==5||state ==9){
        IfxPort_setPinLow(IfxPort_P33_1.port, IfxPort_P33_1.pinIndex); //������
        if(sub_state ==1) g_motor.rr_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.rr_rpm=0;
    }else {
        IfxPort_setPinHigh(IfxPort_P33_1.port, IfxPort_P33_1.pinIndex); //������
        if(sub_state ==1) g_motor.rr_rpm = ref_rpm/2;
        else if(sub_state ==2)g_motor.rr_rpm=0;
    }
}






