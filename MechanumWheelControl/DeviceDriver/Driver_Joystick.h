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

#define get_mid_adc_group2_raw() (&g_adc2_raw)
#define get_mid_adc_group0_raw() (&g_adc0_raw)


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


typedef struct{ //���� ����
        uint32 UlSSense1_Raw;
        uint32 UlSSense2_Raw;
        uint32 UlSSense3_Raw;
}SensorAdcRaw;

typedef struct{ //센싱 변수
        uint32 x;
        uint32 y;
        boolean sw;
}JoystickValue;

extern SensorAdcRaw g_adc0_raw; //���� ���� ����
extern SensorAdcRaw g_adc2_raw; //���� ���� ����

/***********************************************************************/
/*Define*/
/***********************************************************************/

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
JoystickValue get_joystick_rotate_value(void);

#endif /* DRIVER_JOYSTICK */
