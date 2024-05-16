/*
 * Decision_State.c
 *
 *  Created on: 2024. 5. 10.
 *      Author: howon
 *  �븘�슂 �븿�닔 諛� 湲곕뒫
 *    int32 FUNC();
 *     - Joystick�쓽 媛�蹂����빆 x, y�쓽 踰붿쐞�뿉 �뵲�씪 state瑜� �뙋�떒�븯�뿬 return�븿
 *     - state 0~8�� �젣�뼱 諛⑺뼢, 9~10�� 李⑤웾�쓽 �쉶�쟾 諛⑺뼢�쓣 �굹���깂
 *     - �젣�뼱 諛⑺뼢怨� �쉶�쟾 諛⑺뼢 紐⑤몢 �븯�굹�쓽 議곗씠�뒪�떛�쑝濡� �꽕�젙�븯湲� �쐞�빐, 議곗씠�뒪�떛�쓽 �뒪�쐞移� �엯�젰 �떆 �젣�뼱 諛⑺뼢怨� �쉶�쟾 諛⑺뼢�쓣 �넗湲��븿
 *     - state 0~8 (8媛�吏� �젣�뼱 諛⑺뼢)
 *     - 0: 遺곸꽌履�  1: 遺곸そ  2: 遺곷룞履�
 *     - 3: �꽌履�   4: 媛��슫�뜲 5: �룞履�
 *     - 6: �궓�꽌履�  7: �궓履�  8: �궓�룞履�
 *
 *     - state 9~10 (2媛�吏� �쉶�쟾 諛⑺뼢)
 *     - 9: �꽌履�(�떆怨꾨갑�뼢)    10: �룞履�(諛섏떆怨꾨갑�뼢)
 *     -> Driver_Joystick.h �궗�슜
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
static uint8 s_state=0;

/***********************************************************************/
/*Function*/
/***********************************************************************/

//get state value from two joystick value(move: 8 direction , rotate: 2 direction)
uint8 get_state(JoystickValueSet joystick_data) //JoystickValueSet joystick_data
{

    JoystickValue move;
    JoystickValue rotate;
    move = joystick_data.move;
    rotate = joystick_data.rotate;
   // move=get_joystick_move_value();
    //rotate=get_joystick_rotate_value();
    if(move.x <= 1570 && move.y <= 1570){
        s_state = 2;
    }
    else if(move.x >=3350 && move.y <= 1570){
        s_state = 0;
    }
    else if(move.x <=1570 && move.y > 3350){
        s_state = 8;
    }
    else if(move.x >= 3350 && move.y > 3350){
        s_state = 6;
    }
    else if(move.x < 3350 && move.y < 1570){
        s_state = 1;
    }
    else if(move.x >= 3350 && move.y <= 3350){
        s_state = 3;
    }
    else if(move.x < 3350 && move.y > 3350){
        s_state = 7;
    }
    else if(move.x <= 1570 && move.y <= 3350){
        s_state = 5;
    }
    else {
        if(rotate.x>=3350 && rotate.y<3350 &&rotate.y>=1570){
            s_state = 10;
        }
        else if(rotate.x<=1570 &&rotate.y<3350 &&rotate.y>=1570){
            s_state = 9;
        }
        else s_state =4;
    }

    return s_state;
}
