/*
 * Control_Motor.h
 *
 *  Created on: 2024. 5. 10.
 *      Author: user
 *
 *  필요 함수 및 기능
 *    void FUNC(int32 state, int32 sub_state, float32 rpm_ref);
 *     - rpm_ref를 통해 목표 rpm 설정
 *     - state를 통해 네개의 모터 각각의 rpm 및 방향 설정
 *     - sub_state를 통해 slow, stop인 경우 rpm 및 방향 재설정
 *
 *    float32 PID(float32 ref, float32 cur, float32 kp, float32 ki, float32 Ts)
 *     - PI 제어 함수
 *
 *    float32 LPF(float32 Y_fill_d,float32 u,float32 cf,float32 T){ // cf=cutoff
 *     - Low-Pass Filter 구현
 *
 *     -> Driver_Motor1.h, Driver_Motor2.h, Driver_Motor3.h, Driver_Motor4.h
 */

#ifndef CONTROL_CONTROL_MOTOR_H_
#define CONTROL_CONTROL_MOTOR_H_

/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxPort.h"
#include "IfxPort_PinMap.h"
/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*External Variable*/ 
/***********************************************************************/

/***********************************************************************/
/*Global Function Prototype*/ 
/***********************************************************************/
void set_all_wheel(uint8 state, uint8 sub_state, uint32 ref_rpm);

#endif /* CONTROL_CONTROL_MOTOR_H_ */
