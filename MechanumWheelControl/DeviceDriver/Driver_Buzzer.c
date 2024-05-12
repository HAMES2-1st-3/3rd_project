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

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/

#define BUZZER IfxGtm_TOM0_11_TOUT3_P02_3_OUT /* passive buzzer*/
/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
IfxGtm_Tom_Pwm_Config g_tom_config;                                  /* Timer configuration structure                */
IfxGtm_Tom_Pwm_Driver g_tomDriver;                                  /* Timer Driver structure                       */

static uint16 s_period;
/*********************************************************************************************************************/
/*-----------------------------------------------Function Prototypes-------------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*--------------------------------------------Function Implementations-----------------------------------------------*/
/*********************************************************************************************************************/
/* This function initializes the TOM */
void init_buzzer(void)
{

    IfxGtm_enable(&MODULE_GTM);                                     /* Enable GTM                                   */

    IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);   /* Enable the FXU clock                         */

    /* Initialize the configuration structure with default parameters */
    IfxGtm_Tom_Pwm_initConfig(&g_tom_config, &MODULE_GTM);
    //기본 음 444hz  index=12
    //freq  도 레 미 파 솔 높낮이
    float fBuzz[21]={130.812,146.832,164.813,174.614,195.997,220.000,246.941,261.686,293.724,329.724
                ,349.309,392.089,440.000,493.858,523.225,587.275,659.187,698.376,783.884,880.000
        ,987.609
        };
    s_period=(unsigned int )(10000000/fBuzz[12]);


    // arduino D7 , p2.5
    g_tom_config.tom = BUZZER.tom;                                      /* Select the TOM depending on the BUZZER         */
    g_tom_config.tomChannel =BUZZER.channel;                           /* Select the channel depending on the BUZZER     */
    g_tom_config.period = s_period;                                /* Set the timer period                         */
    g_tom_config.pin.outputPin = &BUZZER;                               /* Set the BUZZER port pin as output               */
    g_tom_config.synchronousUpdateEnabled = TRUE;                    /* Enable synchronous update                    */

    IfxGtm_Tom_Pwm_init(&g_tomDriver, &g_tom_config);                /* Initialize the GTM TOM                       */
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

    s_period=(unsigned int )(10000000/fBuzz[soundIdx]);

    g_tom_config.period=s_period;
    g_tom_config.dutyCycle=(uint16)(s_period*sound_intensity); // sound amplitude (don't change,otherwise noisy)
    IfxGtm_Tom_Pwm_init(&g_tomDriver, &g_tom_config);

}

/* This function sets the duty cycle of the PWM */
void set_buzzer_dutycycle(float32 sound_intensity) // 0~1
{
    g_tom_config.dutyCycle = (uint16)(s_period*sound_intensity);                              /* Change the value of the duty cycle           */
    IfxGtm_Tom_Pwm_init(&g_tomDriver, &g_tom_config);                /* Re-initialize the PWM                        */
}

void toggle_buzzer(void){
    if(g_tom_config.dutyCycle==0){
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
