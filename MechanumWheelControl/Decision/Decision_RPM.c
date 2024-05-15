/*
 * Decision_RPM.c
 *
 *  Created on: 2024. 5. 10.
 *      Author: jaeeun
 *  필요 함수 및 기능
 *    float32 FUNC();
 *     - 가변저항의 값(0~1023)을 읽어 RPM 단위로 변환하여 return함
 *     - RPM 의 범위는 실험을 통해 설정하기
 *     -> Driver_Potentiometer.h 사용
 */

/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <Decision_RPM.h>
#include <Driver_USB.h>
#include <Driver_Joystick.h>
#include "stdlib.h"
/***********************************************************************/
/*Define*/ 
/***********************************************************************/
#define RPM_MAX 5300.0
#define POT_MAX 4095.0
/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/

/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/

/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
uint32 g_pot_raw;
uint32 g_rpm_max;
float32 ref_rpm;
uint8 state_val;

/*******************
 * ****************************************************/
/*Function*/ 
/***********************************************************************/
void get_rpm_max(void)
{
    g_pot_raw = get_potentiometer_value();
    g_rpm_max = (uint32)((g_pot_raw / POT_MAX )* RPM_MAX);
}

float32 get_rpm_reference(uint8 state)
{
    get_rpm_max();
    state_val = state;

    static JoystickValue move;
    static JoystickValue rotate;

    move = get_joystick_move_value();
    rotate = get_joystick_rotate_value();
    //usb_printf("O: rotate_x_val:%u,rotate_y_val:%u\n",rotate.x,rotate.y);
    if(state_val ==2 ||state_val ==1 || state_val ==0) //state 0,1,2
    {
        ref_rpm = abs((float)move.y - 1570)/(1570.0) * g_rpm_max;
    }
    else if(state_val ==3) //state 3
    {
        ref_rpm = abs((float)move.x-3350)/(4095.0-3350.0)*g_rpm_max;
    }
    else if(state_val ==4) //state 4
    {
        ref_rpm = 0;
    }
    else if(state_val ==5) //state 5
    {
        ref_rpm = abs((float)move.x-1570)/(1570.0)*g_rpm_max;
    }
    else if(state_val ==8 ||state_val ==7 ||state_val ==6) //state 6 7 8
    {
        ref_rpm = abs((float)move.y-3350)/(4095.0-3350.0)*g_rpm_max;
    }
    else if(state_val ==9) //state 9
    {
        ref_rpm = abs((float)rotate.x-1570)/(1570.0)*g_rpm_max;
    }
    else //state 10
    {
        ref_rpm = abs((float)rotate.x-3350)/(4095.0-3350.0)*g_rpm_max;
    }
    //usb_printf("rotate_x_val:%u,rotate_y_val:%u\n",rotate.x,rotate.y);
    //usb_printf("ref_rpm:%.3f",ref_rpm);
    return ref_rpm;
}

