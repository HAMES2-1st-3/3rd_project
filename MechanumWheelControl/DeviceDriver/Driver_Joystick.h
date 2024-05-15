/*
 * Driver_Joystick.h
 *
 *  Created on: 2024. 5. 10.
 *      Author: howon
 */

#ifndef DEVICEDRIVER_DRIVER_JOYSTICK_H_
#define DEVICEDRIVER_DRIVER_JOYSTICK_H_

/***********************************************************************/
/*Include*/
/***********************************************************************/
#include "Ifx_Types.h"


/***********************************************************************/
/*Typedef*/
/***********************************************************************/

typedef struct{ //센싱 변수
        uint32 x;
        uint32 y;
        boolean sw;
}JoystickValue;

typedef struct{ //센싱 변수
        float32 x;
        float32 y;
        boolean sw;
}JoystickData;


/***********************************************************************/
/*Define*/
/***********************************************************************/
#define JOYSTICK_MAX_VALUE 4095

/***********************************************************************/
/*External Variable*/
/***********************************************************************/
//IFX_EXTERN App_VadcAutoScan g_VadcAutoScan;
//extern volatile uint32_t adcDataResult[5];

/***********************************************************************/
/*Global Function Prototype*/
/***********************************************************************/

void init_joystick(void);

JoystickValue get_joystick_move_value(void);
JoystickData get_joystick_move_data(void);

JoystickValue get_joystick_rotate_value(void);
JoystickData get_joystick_rotate_data(void);

#endif /* DRIVER_JOYSTICK */
