/*
 * Control_Motor.h
 *
 *  Created on: 2024. 5. 10.
 *      Author: user
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
typedef struct{ //value of reference RPM for each wheels
        float32 fl;
        float32 fr;
        float32 rr;
        float32 rl;
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

//add
typedef struct {
    float32 fl_err_old;
    float32 fr_err_old;
    float32 rr_err_old;
    float32 rl_err_old;

    float32 fl_I_err_old;
    float32 fr_I_err_old;
    float32 rr_I_err_old;
    float32 rl_I_err_old;
} control_errors_old;

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*External Variable*/ 
/***********************************************************************/
extern wheel_rpms g_ref_rpm;//value of reference RPM for each wheels
extern wheel_rpms g_cur_rpm;//value of current RPM for each wheels
extern wheel_rpms control_output;
/***********************************************************************/
/*Global Function Prototype*/ 
/***********************************************************************/
void set_all_wheel(uint8 state, uint8 sub_state, float32 goal_rpm);
void opened_loop_control(void);
void closed_loop_control(float32 kp, float32 ki, float32 Ts);
void observer_theta_fl(void);
#endif /* CONTROL_CONTROL_MOTOR_H_ */