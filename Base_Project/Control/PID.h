/*
 * PID.h
 *
 *  Created on: 2024. 5. 9.
 *      Author: user
 */

#ifndef CONTROL_PID_H_
#define CONTROL_PID_H_

#include "Ifx_Types.h"
#include "IfxPort.h"
#include "IfxPort_PinMap.h"

float32 PID(float32 ref, float32 cur, float32 kp, float32 ki, float32 Ts);
float32 LPF(float32 Y_fill_d,float32 u,float32 cf,float32 T);
float32 saturation(float32 bottom, float32 high, float32 target);
void direction(int dir);
#endif /* CONTROL_PID_H_ */
