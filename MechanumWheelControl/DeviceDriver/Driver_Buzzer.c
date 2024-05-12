/*
 * Driver_Motor1.c
 *
 *  Created on: 2024. 5. 10.
 *      Author: user
 */


/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <DeviceDriver/Driver_Buzzer.h>
#include "IfxPort.h"
#include "IfxPort_PinMap.h"
#include "IfxGtm_PinMap.h"

#include "Ifx_Types.h"
#include "IfxGtm_Tom_Pwm.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/

#define BUZZER IfxGtm_TOM0_11_TOUT3_P02_3_OUT /* passive buzzer*/
/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
IfxGtm_Tom_Pwm_Config g_tomConfig;                                  /* Timer configuration structure                */
IfxGtm_Tom_Pwm_Driver g_tomDriver;                                  /* Timer Driver structure                       */

static uint16 s_uPeriod;
/*********************************************************************************************************************/
/*-----------------------------------------------Function Prototypes-------------------------------------------------*/
/*********************************************************************************************************************/
void set_buzzer_dutycycle(float32 sound_intensity);                               /* Function to set the duty cycle of the PWM    */

/*********************************************************************************************************************/
/*--------------------------------------------Function Implementations-----------------------------------------------*/
/*********************************************************************************************************************/
/* This function initializes the TOM */
void init_buzzer(void)
{

    IfxGtm_enable(&MODULE_GTM);                                     /* Enable GTM                                   */

    IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);   /* Enable the FXU clock                         */

    /* Initialize the configuration structure with default parameters */
    IfxGtm_Tom_Pwm_initConfig(&g_tomConfig, &MODULE_GTM);
    //기본 음 444hz  index=12
    //freq  도 레 미 파 솔 높낮이
    float fBuzz[21]={130.812,146.832,164.813,174.614,195.997,220.000,246.941,261.686,293.724,329.724
                ,349.309,392.089,440.000,493.858,523.225,587.275,659.187,698.376,783.884,880.000
        ,987.609
        };
    s_uPeriod=(unsigned int )(10000000/fBuzz[12]);


    // arduino D7 , p2.5
    g_tomConfig.tom = BUZZER.tom;                                      /* Select the TOM depending on the BUZZER         */
    g_tomConfig.tomChannel =BUZZER.channel;                           /* Select the channel depending on the BUZZER     */
    g_tomConfig.period = s_uPeriod;                                /* Set the timer period                         */
    g_tomConfig.pin.outputPin = &BUZZER;                               /* Set the BUZZER port pin as output               */
    g_tomConfig.synchronousUpdateEnabled = TRUE;                    /* Enable synchronous update                    */

    IfxGtm_Tom_Pwm_init(&g_tomDriver, &g_tomConfig);                /* Initialize the GTM TOM                       */
    IfxGtm_Tom_Pwm_start(&g_tomDriver, TRUE);                       /* Start the PWM                                */
}

/* not use makesound()*/
void makesound(uint16 soundIdx,float32 sound_intensity ){

    //freq  도 레 미 파 솔 높낮이
    float fBuzz[21]={130.812,146.832,164.813,174.614,195.997,220.000,246.941
                ,
                261.686,293.724,329.724,349.309,392.089,440.000,493.858,
                523.225,587.275,659.187,
        698.376,783.884,880.000,987.609
        };

    s_uPeriod=(unsigned int )(10000000/fBuzz[soundIdx]);

    g_tomConfig.period=s_uPeriod;
    g_tomConfig.dutyCycle=(uint16)(s_uPeriod*sound_intensity); // sound amplitude (don't change,otherwise noisy)
    IfxGtm_Tom_Pwm_init(&g_tomDriver, &g_tomConfig);

}

/* This function sets the duty cycle of the PWM */
void set_buzzer_dutycycle(float32 sound_intensity) // 0~1
{
    g_tomConfig.dutyCycle = (uint16)(s_uPeriod*sound_intensity);                              /* Change the value of the duty cycle           */
    IfxGtm_Tom_Pwm_init(&g_tomDriver, &g_tomConfig);                /* Re-initialize the PWM                        */
}

void toggle_buzzer(void){
    if(g_tomConfig.dutyCycle==0){
        set_buzzer_dutycycle(0.5);
    }
    else{
        set_buzzer_dutycycle(0);
    }
}
void stop_buzzer(void){
    set_buzzer_dutycycle(0);
}
void start_buzzer(void){ // start default 0.5
    set_buzzer_dutycycle(0.5);
}
