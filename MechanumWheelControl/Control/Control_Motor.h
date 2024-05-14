/*
 * Control_Motor.h
 *
 *  Created on: 2024. 5. 10.
 *      Author: user
 *
 *  �븘�슂 �븿�닔 諛� 湲곕뒫
 *    void FUNC(int32 state, int32 sub_state, float32 rpm_ref);
 *     - rpm_ref瑜� �넻�빐 紐⑺몴 rpm �꽕�젙
 *     - state瑜� �넻�빐 �꽕媛쒖쓽 紐⑦꽣 媛곴컖�쓽 rpm 諛� 諛⑺뼢 �꽕�젙
 *     - sub_state瑜� �넻�빐 slow, stop�씤 寃쎌슦 rpm 諛� 諛⑺뼢 �옱�꽕�젙
 *
 *    float32 PID(float32 ref, float32 cur, float32 kp, float32 ki, float32 Ts)
 *     - PI �젣�뼱 �븿�닔
 *
 *    float32 LPF(float32 Y_fill_d,float32 u,float32 cf,float32 T){ // cf=cutoff
 *     - Low-Pass Filter 援ы쁽
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
#include "Driver_WheelFL.h"
#include "Driver_WheelFR.h"
#include "Driver_WheelRL.h"
#include "Driver_WheelRR.h"
#include "Driver_USB.h"

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
void set_all_wheel(uint8 state, uint8 sub_state, sint32 ref_rpm);

#endif /* CONTROL_CONTROL_MOTOR_H_ */
