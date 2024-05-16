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

/***********************************************************************/
/*Typedef*/
/***********************************************************************/

/***********************************************************************/
/*Define*/
/***********************************************************************/

#define POTENTIOMETER_MAX_VALUE 4095    // 12bit VADC max value


/***********************************************************************/
/*External Variable*/
/***********************************************************************/

/***********************************************************************/
/*Global Function Prototype*/
/***********************************************************************/

/* summary : initialize potentiometer with VADC group4 channel7 
 * argu     >   none
 * return   >   none
 */
void init_potentiometer(void);

/* summary : get potentiometer value
 * argu     >   none
 * return   >   (uint32)    : 0 ~ 4095 ADC value
 */
uint32 get_potentiometer_value(void);

/* summary : get potentiometer data in percentage 0f~100f
 * argu     >   none
 * return   >   (float32)   : 0f~100f percentage
 */
float32 get_potentiometer_data(void);

#endif /* DEVICEDRIVER_DRIVER_POTENTIOMETER_H_ */
