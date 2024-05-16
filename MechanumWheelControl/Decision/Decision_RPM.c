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
//호원

typedef struct{
    float32 dist;
    float32 total_dist;
}Joystickdist;
Joystickdist value;
static JoystickValue move;
static JoystickValue rotate;

/*******************
 * ****************************************************/
/*Function*/ 
/***********************************************************************/
void get_rpm_max(void)
{
    g_pot_raw = get_potentiometer_value();
    g_rpm_max = (uint32)((g_pot_raw / POT_MAX )* RPM_MAX);
}
/*
float32 get_rpm_reference(uint8 state)
{
    get_rpm_max();

    static JoystickValue move;
    static JoystickValue rotate;

    move = get_joystick_move_value();
    rotate = get_joystick_rotate_value();
    //usb_printf("O: rotate_x_val:%u,rotate_y_val:%u\n",rotate.x,rotate.y);
    if(state ==2 ||state ==1 || state ==0) //state 0,1,2
    {
        ref_rpm = fabs((float)move.y - 1570)/(1570) * g_rpm_max;
    }
    else if(state ==3) //state 3
    {
        ref_rpm = fabs((float)move.x-3350)/(4095-3350)*g_rpm_max;
    }
    else if(state ==4) //state 4
    {
        ref_rpm = 0;
    }
    else if(state ==5) //state 5
    {
        ref_rpm = fabs((float)move.x-1570)/(1570)*g_rpm_max;
    }
    else if(state ==8 ||state ==7 ||state ==6) //state 6 7 8
    {
        ref_rpm = fabs((float)move.y-3350)/(4095-3350)*g_rpm_max;
    }
    else if(state ==9) //state 9
    {
        ref_rpm = fabs((float)rotate.x-1570)/(1570)*g_rpm_max;
    }
    else //state 10
    {
        ref_rpm = fabs((float)rotate.x-3350)/(4095-3350)*g_rpm_max;
    }

    return ref_rpm;
}*/

//호원


float32 get_rpm_reference(uint8 state)
{
    get_rpm_max();

    move = get_joystick_move_value();
    rotate = get_joystick_rotate_value();
    //usb_printf("O: rotate_x_val:%u,rotate_y_val:%u\n",rotate.x,rotate.y);
    //_usb_printf("state:%u,ref_rpm:%.3f\n",state,ref_rpm);
    if(state ==1) //state 0,1,2
    {
        ref_rpm = fabs((float)move.y - 1570)/(1570) * g_rpm_max;
    }
    else if(state ==0||state==2||state==6||state==8) //state 3
    {
        get_distance(state);
        ref_rpm = (value.dist/value.total_dist)*g_rpm_max;
    }
    else if(state ==3){
        ref_rpm = fabs((float)move.x-3350)/(4095-3350)*g_rpm_max;
    }
    else if(state ==4) //state 4
    {
        ref_rpm = 0;
    }
    else if(state ==5) //state 5
    {
        ref_rpm = fabs((float)move.x-1570)/(1570)*g_rpm_max;
    }
    else if(state ==7 ) //state 6 7 8
    {
        ref_rpm = fabs((float)move.y-3350)/(4095-3350)*g_rpm_max;
    }
    else if(state ==9) //state 9
    {
        ref_rpm = fabs((float)rotate.x-1570)/(1570)*g_rpm_max;
    }
    else //state 10
    {
        ref_rpm = fabs((float)rotate.x-3350)/(4095-3350)*g_rpm_max;
    }

    return ref_rpm;
}

void get_distance(uint8 state)
{

    double m=0;
    double a=0;
    double b=0;

    switch(state){
        case 0:
            value.dist = sqrtf((move.x-3350.0)*(move.x-3350.0) +(move.y-1570.0)*(move.y-1570.0));
            m = ((float)move.y - 1570) / (move.x - 3350.0);
            b = 1570 - m * 3350;
            if(-1570.0/(4095-3350)*(move.x-4095)>=move.y){ // y=0과 cross
                value.total_dist = sqrtf((-b / m - 3350.0) * ((-b / m) - 3350.0) + (0 - 1570.0) * (0 - 1570.0));
            }
            else{  // x=4095와 cross
                value.total_dist = sqrtf((4095.0 - 3350) * (4095.0 - 3350) + ((m * 4095 + b) - 1570.0) * ((m * 4095 + b) - 1570.0));
            }
           // _usb_printf("value.total:%.3f,value.dist:%.3f\n",value.total_dist,value.dist);
            break;
        case 2:
            value.dist = sqrtf((move.x-1570.0)*(move.x-1570.0) +(move.y-1570.0)*(move.y-1570.0));
            m = ((float)move.y-1570)/(move.x-1570.0);
            b = 1570-m*1570;
            a =-b/m;
            if(move.x>=move.y){ // x=0과 cross
              value.total_dist = sqrtf((1570.0-m)*(1570.0-m)+(0-1570.0)*(0-1570.0));
            }else{ //y=0과 cross
              value.total_dist = sqrtf((0-1570.0)*(0-1570.0)+(b-1570.0)*(b-1570.0));
            }
           // _usb_printf("value.total:%.3f,value.dist:%.3f\n",value.total_dist,value.dist);
            break;
        case 6:
            value.dist = sqrtf((move.x-3350.0)*(move.x-3350.0) +(move.y-3350.0)*(move.y-3350.0));
            m = ((float)move.y - 3350) / (move.x - 3350.0);
            b = 3350.0 - m * 3350;
            if(move.x>=move.y){//x=4095와 cross
                value.total_dist = sqrtf((4095 - 3350.0) * (4095 - 3350.0) + (m*(4095-3350.0)) * (m*(4095-3350.0)));
            }else{ //y=4095와 cross
                value.total_dist = sqrtf(((4095.0 - 3350.0) / m) * ((4095.0 - 3350.0) / m) + (4095.0 - 3350) * (4095.0 - 3350));
            }
            //_usb_printf("value.total:%.3f,value.dist:%.3f\n",value.total_dist,value.dist);
            break;
        case 8:
            value.dist = sqrtf((move.x-1570.0)*(move.x-1570.0)+(move.y-3350.0)*(move.y-3350.0));
            m = ((float)move.y - 3350) / (move.x - 1570.0);
            b = 3350 - m * 1570;
            if(move.y<=(4095-3350.0)/(-1570)*move.x+4095){ //x=0과 cross
                value.total_dist = sqrtf((0 - 1570.0) * (0 - 1570.0) + (b - 3350.0) * (b - 3350.0));
            }else { //y=4095와 cross
                value.total_dist = sqrtf(((4095 - 3350) / m) * ((4095 - 3350) / m) + (4095 - 3350.0) * (4095 - 3350.0));
            }
            //_usb_printf("value.total:%.3f,value.dist:%.3f\n",value.total_dist,value.dist);
            break;
        default:
            break;
    }
}

