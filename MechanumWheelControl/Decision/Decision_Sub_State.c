/*
 * Decision_RPM.c
 *
 *  Created on: 2024. 5. 10.
 *      Author: jekim
 *  필요 함수 및 기능
 *    int32 FUNC();
 *     - ToF센서를 통해 거리값을 입력 받아 sub_state를 판단하여 return함
 *     - sub_state: 0,1,2
 *     - distance 가 50cm 초과일 경우 normal(0)
 *     - distance 가 10cm 초과 && 50cm 이하일 경우 slow(1)
 *     - distance 가 10cm 이하일 경우 stop(2)
 *     - distance 가 음수와 같은 에러값일 경우 이전의 sub_state 리턴
 *
 *     -> Driver_ToF.h 사용
 */


/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <Decision/Decision_Sub_State.h>
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
static uint32  s_sub_state;
sint32 dist;
/***********************************************************************/
/*Function*/ 
/***********************************************************************/
uint8 get_sub_state(void){
    dist = get_tof_distance();
    if(dist >= 0 && dist <= 105) // Include +5mm error
    {
        s_sub_state = 2;
    }
    else if(dist <= 505)         // Include +5mm error
    {
        s_sub_state = 1;
    }
    else if(dist == -1 || dist == -2) // error distance
    {
        ; // return previous sub_state
    }
    else{
        s_sub_state = 0;
    }
    return s_sub_state;
}


