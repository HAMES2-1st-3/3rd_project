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

/* summary : initialize buzzer device with TOM0_11_TOUT3, P02_3
 *           in 'init_buzzer()', it 'use set_buzzer_intensity(50)'
 * argu     >   none
 * return   >   none
 */
void init_buzzer(void);                         

/* summary : set buzzer intensity 100~0, 
 * argu     >   intensity   : this configurate dutycycle of PWM
 * return   >   none   
 */
void set_buzzer_intensity(float32 intensity);


/* summary : toggle buzzer with certain intensity
 * argu     >   none
 * return   >   none   
 */
void set_buzzer_toggle(void);

/* summary : off the buzzer sound
 * argu     >   none
 * return   >   none   
 */
void set_buzzer_off(void);

/* summary : on the buzzer sound with certain intensity
 * argu     >   none
 * return   >   none   
 */
void set_buzzer_on(void);
                           /* Function to set the duty cycle of the PWM    */
#endif /* DEVICEDRIVER_DRIVER_BUZZER_H_ */
