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

/*
 * in 'init_buzzer()', it 'use set_buzzer_intensity(50)'
 * if you want to use buzzer, you just use 'init_buzzer()' in main()
 */
void init_buzzer(void);                         /* initialize buzzer */

/*
 * if you want to set other buzzer intensity
 * set intensity 100~0
 */
void set_buzzer_intensity(float32 intensity);

void set_buzzer_toggle(void);
void set_buzzer_off(void);
void set_buzzer_on(void);
                           /* Function to set the duty cycle of the PWM    */
#endif /* DEVICEDRIVER_DRIVER_BUZZER_H_ */
