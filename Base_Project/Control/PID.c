/*
 * PID.c
 *
 *  Created on: 2024. 5. 9.
 *      Author: user
 */

#include <Control/PID.h>


float32 PID(float32 ref, float32 cur, float32 kp, float32 ki, float32 Ts){
    static float32 error_int_old ;
    static float32 error_old = 0;
    float32 error = ref - cur;
    float32 error_int = error_int_old + (error)*Ts;
    float32 error_diff = (error - error_old) / Ts;
    error_int_old = error_int;
    error_old = error;

    if(error_int > 10)
        error_int = 10;

//    return (Kp * error + Ki * error_int + Kd* error_diff);
    return (kp * error + ki * error_int);
}
float32 LPF(float32 Y_fill_d,float32 u,float32 cf,float32 T){ // cf=cutoff

     return (1-T*cf)*Y_fill_d+T*cf*u;
}

float32 saturation(float32 bottom, float32 high, float32 target){
    if(target < bottom)
        return bottom;
    else if(target > high)
        return high;
    return target;
}

void direction(int dir){ // 1: forward , -1 : reverse
    if(dir == 1){
        IfxPort_setPinModeOutput(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
        IfxPort_setPinLow(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex);
    }
    else if(dir == -1){
        IfxPort_setPinModeOutput(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
        IfxPort_setPinHigh(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex);
    }
    else{
        IfxPort_setPinModeOutput(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
        IfxPort_setPinLow(IfxPort_P10_2.port, IfxPort_P10_2.pinIndex);
    }
}
