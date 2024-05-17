/*
 * Driver_Buzzer.c
 *
 *  Created on: 2024. 5. 10.
 *      Author: chanik
 */


/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <Driver_Buzzer.h>
#include <PortPinMapping.h>

#include "IfxPort.h"
#include "IfxPort_PinMap.h"
#include "IfxGtm_PinMap.h"
#include "IfxGtm_Tom_Pwm.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
#define PWM_PERIOD                  50000                                   /* PWM period for the TOM                       */



/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
static IfxGtm_Tom_Pwm_Config s_tom_config;                                  /* Timer configuration structure                */
static IfxGtm_Tom_Pwm_Driver s_tomDriver;                                  /* Timer Driver structure                       */

static float32 s_buzzer_intensity = 0; // 100% ~ 0%

/*********************************************************************************************************************/
/*-----------------------------------------------Function Prototypes-------------------------------------------------*/
/*********************************************************************************************************************/
static void set_buzzer_dutycycle(float32 sound_intensity);


/*********************************************************************************************************************/
/*--------------------------------------------Function Implementations-----------------------------------------------*/
/*********************************************************************************************************************/
/* This function initializes the TOM */
void init_buzzer(void)
{

    IfxGtm_enable(&MODULE_GTM);                                     /* Enable GTM                                   */

    IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);   /* Enable the FXU clock                         */

    /* Initialize the configuration structure with default parameters */
    IfxGtm_Tom_Pwm_initConfig(&s_tom_config, &MODULE_GTM);

    // arduino D7 , p2.5
    s_tom_config.tom = _M_BUZZER_TOUTMAP.tom;                                      /* Select the TOM depending on the BUZZER         */
    s_tom_config.tomChannel =_M_BUZZER_TOUTMAP.channel;                           /* Select the channel depending on the BUZZER     */
    s_tom_config.period = PWM_PERIOD;                                /* Set the timer period                         */
    s_tom_config.pin.outputPin = &_M_BUZZER_TOUTMAP;                               /* Set the BUZZER port pin as output               */
    s_tom_config.synchronousUpdateEnabled = TRUE;                    /* Enable synchronous update                    */

    IfxGtm_Tom_Pwm_init(&s_tomDriver, &s_tom_config);                /* Initialize the GTM TOM                       */
    IfxGtm_Tom_Pwm_start(&s_tomDriver, TRUE);                       /* Start the PWM                                */

    set_buzzer_intensity(80); // default intensity = 50%
}


void set_buzzer_intensity(float32 intensity){ // 100%~0%
    s_buzzer_intensity = intensity;
}

/* This function sets the duty cycle of the PWM */
static void set_buzzer_dutycycle(float32 sound_intensity) // 100%~0%
{
    s_tom_config.dutyCycle = (uint16)((sound_intensity / 100) * PWM_PERIOD);                              /* Change the value of the duty cycle           */
    IfxGtm_Tom_Pwm_init(&s_tomDriver, &s_tom_config);                /* Re-initialize the PWM                        */
}

void set_buzzer_toggle(void){
    if(s_tom_config.dutyCycle==0){
        set_buzzer_dutycycle(s_buzzer_intensity);
    }
    else{
        set_buzzer_dutycycle(0);
    }
}


void set_buzzer_off(void){
    set_buzzer_dutycycle(0);
}
void set_buzzer_on(void){ // start default 0.5
    set_buzzer_dutycycle(s_buzzer_intensity);
}
