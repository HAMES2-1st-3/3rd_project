/*
 * Driver_Buzzer.h
 *
 *  Created on: 2024. 5. 10.
 *      Author: chanik
 */

#ifndef DEVICEDRIVER_DRIVER_BUZZER_H_
#define DEVICEDRIVER_DRIVER_BUZZER_H_

/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxPort.h"
#include "IfxPort_PinMap.h"
#include "IfxGtm_PinMap.h"
#include "IfxGtm_Tom_Pwm.h"
/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/

/***********************************************************************/
/*Define*/ 
/***********************************************************************/

/***********************************************************************/
/*External Variable*/ 
/***********************************************************************/

/***********************************************************************/
/*Global Function Prototype*/ 
/***********************************************************************/


void init_buzzer(void);
void makesound(uint16 soundIdx,float32 sound_intensity );
void toggle_buzzer(void);
void start_buzzer(void);
void stop_buzzer(void);
void set_buzzer_dutycycle(float32 sound_intensity);                               /* Function to set the duty cycle of the PWM    */
#endif /* DEVICEDRIVER_DRIVER_BUZZER_H_ */
