/*
 * Driver_Joystick.h
 *
 *  Created on: 2024. 5. 10.
 *      Author: howon
 */

#ifndef DRIVER_JOYSTICK
#define DRIVER_JOYSTICK

/***********************************************************************/
/*Include*/
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxVadc.h"
#include "IfxVadc_Adc.h"
#include "IfxPort.h"
#include "IfxPort_PinMap.h"
/***********************************************************************/
/*Typedef*/
/***********************************************************************/

typedef enum{
    ADC_GROUP0_CH0=0u,
    ADC_GROUP0_CH1,
    ADC_GROUP0_CH2,
    ADC_GROUP0_CH3,
    ADC_GROUP0_CH4,
    ADC_GROUP0_CH5,
    ADC_GROUP0_CH6,
    ADC_GROUP0_CH7,
    ADC_GROUP0_MAX
}ADC_GROUP0;

typedef enum{
    ADC_GROUP2_CH0=0u,
    ADC_GROUP2_CH1,
    ADC_GROUP2_CH2,
    ADC_GROUP2_CH3,
    ADC_GROUP2_CH4,
    ADC_GROUP2_CH5,
    ADC_GROUP2_CH6,
    ADC_GROUP2_CH7,
    ADC_GROUP2_MAX
}ADC_GROUP2;


typedef struct{
        uint32 x;
        uint32 y;
}JoystickValue;

typedef struct{
        JoystickValue move;
        JoystickValue rotate;
}JoystickValueSet;


/***********************************************************************/
/*Define*/
/***********************************************************************/

/***********************************************************************/
/*External Variable*/
/***********************************************************************/

/***********************************************************************/
/*Global Function Prototype*/
/***********************************************************************/

void init_joystick(void);
JoystickValue get_joystick_move_value(void);
JoystickValue get_joystick_rotate_value(void);

#endif /* DRIVER_JOYSTICK */
