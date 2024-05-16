/*
 * Decision_RPM.c
 *
 *  Created on: 2024. 5. 10.
 *      Author: jaeeun
 *  �븘�슂 �븿�닔 諛� 湲곕뒫
 *    float32 FUNC();
 *     - 媛�蹂����빆�쓽 媛�(0~1023)�쓣 �씫�뼱 RPM �떒�쐞濡� 蹂��솚�븯�뿬 return�븿
 *     - RPM �쓽 踰붿쐞�뒗 �떎�뿕�쓣 �넻�빐 �꽕�젙�븯湲�
 *     -> Driver_Potentiometer.h �궗�슜
 */

/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <Decision_RPM.h>
#include "math.h"
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
static JoystickValue s_move;
static JoystickValue s_rotate;

/*******************
 * ****************************************************/
/*Function*/ 
/***********************************************************************/


void get_rpm_max(void) //get max rpm value(0~5300) from potentiometer value(0~4095)
{
    g_pot_raw = get_potentiometer_value();
    g_rpm_max = (uint32)((g_pot_raw / POT_MAX )* RPM_MAX);
}

//호원

//get reference rpm value depending on two joystick values(move, rotate)
// when state is 0,2,6,8 , it is exceptional case
float32 get_rpm_reference(uint8 state,JoystickValueSet s_joystick_data)//JoystickValueSet s_joystick_data
{
    get_rpm_max();
    s_move = s_joystick_data.move;
    s_rotate = s_joystick_data.rotate;
   // s_move=get_joystick_move_value();
   // s_rotate=get_joystick_rotate_value();

    if(state ==1) //state 0,1,2
    {
        ref_rpm = fabs((float)s_move.y - 1570.0)/(1570.0) * g_rpm_max;
    }
    else if(state ==0||state==2||state==6||state==8) //state 3
    {
        get_distance(state);
        ref_rpm = (value.dist/value.total_dist)*g_rpm_max;
    }
    else if(state ==3){
        ref_rpm = fabs((float)s_move.x-3350.0)/(4095.0-3350)*g_rpm_max;
    }
    else if(state ==4) //state 4
    {
        ref_rpm = 0;
    }
    else if(state ==5) //state 5
    {
        ref_rpm = fabs((float)s_move.x-1570.0)/(1570.0)*g_rpm_max;
    }
    else if(state ==7 ) //state 6 7 8
    {
        ref_rpm = fabs((float)s_move.y-3350.0)/(4095-3350.0)*g_rpm_max;
    }
    else if(state ==9) //state 9
    {
        ref_rpm = fabs((float)s_rotate.x-1570.0)/(1570.0)*g_rpm_max;
    }
    else //state 10
    {
        ref_rpm = fabs((float)s_rotate.x-3350.0)/(4095.0-3350)*g_rpm_max;
    }
    return ref_rpm;
}

//get distance by slope formula when state is 0,2,6,8 (exceptional case) which is diagonal direction
void get_distance(uint8 state)
{
    double m=0;
    double a=0;
    double b=0;
    switch(state){
        case 0:
            value.dist = sqrtf((s_move.x-3350.0)*(s_move.x-3350.0) +(s_move.y-1570.0)*(s_move.y-1570.0));
            m = ((float)s_move.y - 1570) / (s_move.x - 3350.0);
            b = 1570 - m * 3350;
            if(-1570.0/(4095-3350)*(s_move.x-4095)>=s_move.y){ // y=0과 cross
                value.total_dist = sqrtf((-b / m - 3350.0) * ((-b / m) - 3350.0) + (0 - 1570.0) * (0 - 1570.0));
            }
            else{  // x=4095와 cross
                value.total_dist = sqrtf((4095.0 - 3350) * (4095.0 - 3350) + ((m * 4095 + b) - 1570.0) * ((m * 4095 + b) - 1570.0));
            }
           // send_usb_printf("value.total:%.3f,value.dist:%.3f\n",value.total_dist,value.dist);
            break;
        case 2:
            value.dist = sqrtf((s_move.x-1570.0)*(s_move.x-1570.0) +(s_move.y-1570.0)*(s_move.y-1570.0));
            m = ((float)s_move.y-1570)/(s_move.x-1570.0);
            b = 1570-m*1570;
            a =-b/m;
            if(s_move.x>=s_move.y){ // x=0과 cross
              value.total_dist = sqrtf((1570.0-m)*(1570.0-m)+(0-1570.0)*(0-1570.0));
            }else{ //y=0과 cross
              value.total_dist = sqrtf((0-1570.0)*(0-1570.0)+(b-1570.0)*(b-1570.0));
            }
            //send_usb_printf("value.total:%.3f,value.dist:%.3f\n",value.total_dist,value.dist);
            break;
        case 6:
            value.dist = sqrtf((s_move.x-3350.0)*(s_move.x-3350.0) +(s_move.y-3350.0)*(s_move.y-3350.0));
            m = ((float)s_move.y - 3350) / (s_move.x - 3350.0);
            b = 3350.0 - m * 3350;
            if(s_move.x>=s_move.y){//x=4095와 cross
                value.total_dist = sqrtf((4095 - 3350.0) * (4095 - 3350.0) + (m*(4095-3350.0)) * (m*(4095-3350.0)));
            }else{ //y=4095와 cross
                value.total_dist = sqrtf(((4095.0 - 3350.0) / m) * ((4095.0 - 3350.0) / m) + (4095.0 - 3350) * (4095.0 - 3350));
            }
            //send_usb_printf("value.total:%.3f,value.dist:%.3f\n",value.total_dist,value.dist);
            break;
        case 8:
            value.dist = sqrtf((s_move.x-1570.0)*(s_move.x-1570.0)+(s_move.y-3350.0)*(s_move.y-3350.0));
            m = ((float)s_move.y - 3350) / (s_move.x - 1570.0);
            b = 3350 - m * 1570;
            if(s_move.y<=(4095-3350.0)/(-1570)*s_move.x+4095){ //x=0과 cross
                value.total_dist = sqrtf((0 - 1570.0) * (0 - 1570.0) + (b - 3350.0) * (b - 3350.0));
            }else { //y=4095와 cross
                value.total_dist = sqrtf(((4095 - 3350) / m) * ((4095 - 3350) / m) + (4095 - 3350.0) * (4095 - 3350.0));
            }
            //send_usb_printf("value.total:%.3f,value.dist:%.3f\n",value.total_dist,value.dist);
            break;
        default:
            break;
    }
}

