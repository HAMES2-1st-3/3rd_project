/*
 * Driver_Potentiometer.h
 *
 *  Created on: 2024. 5. 10.
 *      Author: jaeeun
 */

#ifndef DEVICEDRIVER_DRIVER_POTENTIOMETER_H_
#define DEVICEDRIVER_DRIVER_POTENTIOMETER_H_

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
    ADC_GROUP4_CH0=0u,
    ADC_GROUP4_CH1,
    ADC_GROUP4_CH2,
    ADC_GROUP4_CH3,
    ADC_GROUP4_CH4,
    ADC_GROUP4_CH5,
    ADC_GROUP4_CH6,
    ADC_GROUP4_CH7,
    ADC_GROUP4_MAX
}ADC_GROUP4;

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*External Variable*/ 
/***********************************************************************/

/***********************************************************************/
/*Global Function Prototype*/ 
/***********************************************************************/
void init_potentiometer(void);
uint32 get_potentiometer_value(void);



#endif /* DEVICEDRIVER_DRIVER_POTENTIOMETER_H_ */
