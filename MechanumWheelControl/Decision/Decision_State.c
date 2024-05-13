/*
 * Decision_State.c
 *
 *  Created on: 2024. 5. 10.
 *      Author: howon
 *  필요 함수 및 기능
 *    int32 FUNC();
 *     - Joystick의 가변저항 x, y의 범위에 따라 state를 판단하여 return함
 *     - state 0~8은 제어 방향, 9~10은 차량의 회전 방향을 나타냄
 *     - 제어 방향과 회전 방향 모두 하나의 조이스틱으로 설정하기 위해, 조이스틱의 스위치 입력 시 제어 방향과 회전 방향을 토글함
 *     - state 0~8 (8가지 제어 방향)
 *     - 0: 북서쪽  1: 북쪽  2: 북동쪽
 *     - 3: 서쪽   4: 가운데 5: 동쪽
 *     - 6: 남서쪽  7: 남쪽  8: 남동쪽
 *
 *     - state 9~10 (2가지 회전 방향)
 *     - 9: 서쪽(시계방향)    10: 동쪽(반시계방향)
 *     -> Driver_Joystick.h 사용
 */

/***********************************************************************/
/*Include*/
/***********************************************************************/
#include <Driver_Joystick.h>
/***********************************************************************/
/*Define*/
/***********************************************************************/

/***********************************************************************/
/*Typedef*/
/***********************************************************************/

/***********************************************************************/
/*Static Function Prototype*/
/***********************************************************************/

/***********************************************************************/
/*Variable*/
/***********************************************************************/
uint32 g_move_x_val =0;
uint32 g_move_y_val =0;
uint32 g_rotate_x_val =0;
uint32 g_rotate_y_val =0;
static uint32 s_state=0;

/***********************************************************************/
/*Function*/
/***********************************************************************/
int get_state(void)
{

    /*ADC Test*/
    get_adc_group0_raw();
    get_adc_group2_raw();

    SensorAdcRaw* move_raw   = get_mid_adc_group2_raw();
    SensorAdcRaw* rotate_raw = get_mid_adc_group0_raw();

    g_move_x_val= move_raw->UlSSense1_Raw;
    g_move_y_val= move_raw->UlSSense2_Raw;

    if(g_move_x_val <= 1570 && g_move_y_val <= 1570){
        s_state = 0;
    }
    else if(g_move_x_val >=3350 && g_move_y_val <= 1570){
        s_state = 2;
    }
    else if(g_move_x_val <=1570 && g_move_y_val > 3350){
        s_state = 6;
    }
    else if(g_move_x_val >= 3350 && g_move_y_val > 3350){
        s_state = 8;
    }
    else if(g_move_x_val < 3350 && g_move_y_val < 1570){
        s_state = 1;
    }
    else if(g_move_x_val >= 3350 && g_move_y_val <= 3350){
        s_state = 5;
    }
    else if(g_move_x_val < 3350 && g_move_y_val > 3350){
        s_state = 7;
    }
    else if(g_move_x_val <= 1570 && g_move_y_val <= 3350){
        s_state = 3;
    }
    else {
        g_rotate_x_val= rotate_raw->UlSSense1_Raw;
        g_rotate_y_val= rotate_raw->UlSSense2_Raw;
        if(g_rotate_x_val>=3350 && g_rotate_y_val<3350 &&g_rotate_y_val>=1570){
            s_state = 9;
        }
        else if(g_rotate_x_val<=1570 &&g_rotate_y_val<3350 &&g_rotate_y_val>=1570){
            s_state = 10;
        }
        else s_state =4;
    }

    return s_state;
}
