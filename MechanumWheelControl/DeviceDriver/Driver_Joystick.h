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

// joystick x, y value
typedef struct{ //센싱 변수
        uint32 x;
        uint32 y;
}JoystickValue;

// all joystick value set
typedef struct{
        JoystickValue move;
        JoystickValue rotate;
}JoystickValues;


/***********************************************************************/
/*Define*/
/***********************************************************************/
#define JOYSTICK_MAX_VALUE 4095 // 12bit VADC max value

/***********************************************************************/
/*External Variable*/
/***********************************************************************/
//IFX_EXTERN App_VadcAutoScan g_VadcAutoScan;
//extern volatile uint32_t adcDataResult[5];

/***********************************************************************/
/*Global Function Prototype*/
/***********************************************************************/

/* summary : initialize potentiometer with VADC group0 channel3,2 
             and group2 channel5,4
 * argu     >   none
 * return   >   none
 */
void init_joystick(void);

/* summary : get move joystick value
 * argu     >   none
 * return   >   (JoystickValue) : two uint32 value within 0 ~ 4095 ADC value
 */
JoystickValue get_joystick_move_value(void);

/* summary : get rotate joystick value
 * argu     >   none
 * return   >   (JoystickValue) : two uint32 value within 0 ~ 4095 ADC value
 */
JoystickValue get_joystick_rotate_value(void);

#endif /* DRIVER_JOYSTICK */
