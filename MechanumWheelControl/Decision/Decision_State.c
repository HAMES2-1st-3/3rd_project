/*
 * Decision_State.c
 *
 *  Created on: 2024. 5. 10.
 *      Author: user
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
#include "Driver_Joystick.h"
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
uint32 move_x_val =0;
uint32 move_y_val =0;
uint32 rotate_x_val =0;
uint32 rotate_y_val =0;
uint32 state_val=0;

/***********************************************************************/
/*Function*/
/***********************************************************************/
int Get_Joystick_value(void)
{

    /*ADC Test*/
    DrvAdc_GetAdcRawGroup0();
    DrvAdc_GetAdcRawGroup2();

    SensorAdcRaw* moveAdcRaw = MidAdc_GetAdcGroup2SenseRaw();
    SensorAdcRaw* rotateAdcRaw = MidAdc_GetAdcGroup0SenseRaw();

    move_x_val= moveAdcRaw->UlSSense1_Raw;
    move_y_val= moveAdcRaw->UlSSense2_Raw;

    if(move_x_val <= 1570 && move_y_val <= 1570){
        state_val = 0;
    }
    else if(move_x_val >=3350 && move_y_val <= 1570){
        state_val = 2;
    }
    else if(move_x_val <=1570 && move_y_val > 3350){
        state_val = 6;
    }
    else if(move_x_val >= 3350 && move_y_val > 3350){
        state_val = 8;
    }
    else if(move_x_val < 3350 && move_y_val < 1570){
        state_val = 1;
    }
    else if(move_x_val >= 3350 && move_y_val <= 3350){
        state_val = 5;
    }
    else if(move_x_val < 3350 && move_y_val > 3350){
        state_val = 7;
    }
    else if(move_x_val <= 1570 && move_y_val <= 3350){
        state_val = 3;
    }
    else {
        rotate_x_val= rotateAdcRaw->UlSSense1_Raw;
        rotate_y_val= rotateAdcRaw->UlSSense2_Raw;
        if(rotate_x_val>=3350 && rotate_y_val<3350 &&rotate_y_val>=1570){
            state_val = 9;
        }
        else if(rotate_x_val<=1570 &&rotate_y_val<3350 &&rotate_y_val>=1570){
            state_val = 10;
        }
        else state_val =4;
    }

    return state_val;
}
