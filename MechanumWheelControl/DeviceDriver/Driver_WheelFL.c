/**********************************************************************************************************************
 * \file Driver_WheelFL.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 * 
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of 
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and 
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all 
 * derivative works of the Software, unless such copies or derivative works are solely in the form of 
 * machine-executable object code generated by a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN 
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 *********************************************************************************************************************/


/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include <Driver_WheelFL.h>
#include <InterruptPriority.h>
#include <PortPinMapping.h>


#include "IfxGtm_Tom_Pwm.h"
#include "IfxSrc.h"
#include "IfxScuEru.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/


#define MOTOR_FORWARD_DIR 0  /* 1 : motor CW(1) -> wheel forward, 0 : motor CCW(0) -> wheel forward */

#define PWM_PERIOD                  50000                                   /* PWM period for the TOM                       */

#define INTERRUPT_TRIGGER_CHANNEL   (IfxScuEru_InputNodePointer_0)    /* same with statement below  */
#define OUTPUT_CHANNEL              (IfxScuEru_OutputChannel_0)       /* same with statement above  */

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*--------------------------------------------Private Variables/Constants--------------------------------------------*/
/*********************************************************************************************************************/
static IfxGtm_Tom_Pwm_Config s_tomConfig_motor;                                  /* Timer configuration structure                */
static IfxGtm_Tom_Pwm_Driver s_tomDriver_motor;


static sint32 s_encoder_data = 0;  // 48 per rotate


/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/

static void init_motorFL(void);
static inline void set_motorFL_dutycycle(float32 dutycycle); // dutycycle : 100f ~ 0f
static inline void set_motorFL_direction(boolean clock_wise); // 1 : CW, 0: CCW

// 0% ~ 100%
static inline float32 get_motorFL_dutycycle(void);
// 1 : CW, 0 : CCW
static inline boolean get_motorFL_direction(void);

static void init_encoderFL(void);
static inline sint32 get_encoderFL_tick(void);


/*********************************************************************************************************************/
/*-------------------------------------------wheel driver Implementations--------------------------------------------*/
/*********************************************************************************************************************/


/* when looking at the motor shaft above,
 * The clockwise motion of the motor is positive quantity.
 * motor use 'CW', 'CCW' to refer direction.
 *      motor CW : positive, motor CCW ; negative
 *
 * while, wheel use 'forward' and 'backward'.
 *      wheel forward : positive, wheel backward : negative
 *
 * user only can access to wheel interface not the motor interface.
 */


void init_wheelFL(void){
    init_motorFL();
    init_encoderFL();
}


/* duty cycle resolution : 1/50000(PWM_PERIOD) = 0.002% */
void set_wheelFL_dutycycle(float32 dutycycle) { // dutycycle : 100f ~ -100f
#if MOTOR_FORWARD_DIR == 1
    if(dutycycle >= 0) {
        set_motorFL_direction(1);
        set_motorFL_dutycycle(dutycycle);
    } else {
        set_motorFL_direction(0);
        set_motorFL_dutycycle(-dutycycle);
    }
#else
    if(dutycycle >= 0) {
        set_motorFL_direction(0);
        set_motorFL_dutycycle(dutycycle);
    } else {
        set_motorFL_direction(1);
        set_motorFL_dutycycle(-dutycycle);
    }
#endif
}

// 100% ~ -100%
float32 get_wheelFL_dutycycle(void) {
#if MOTOR_FORWARD_DIR == 1
    if(get_motorFL_direction() == 1) {
        return get_motorFL_dutycycle();
    } else {
        return -get_motorFL_dutycycle();
    }
#else
    if(get_motorFL_direction() == 0) {
        return get_motorFL_dutycycle();
    } else {
        return -get_motorFL_dutycycle();
    }
#endif
}


sint32 get_wheelFL_tick(void) {
#if MOTOR_FORWARD_DIR == 1
    return get_encoderFL_tick();
#else
    return -get_encoderFL_tick();
#endif
}



/*********************************************************************************************************************/
/*-------------------------------------------motor driver Implementations--------------------------------------------*/
/*********************************************************************************************************************/


static void init_motorFL(void) {
    IfxPort_setPinMode(_P_MOTORFL_DIR, IfxPort_Mode_outputPushPullGeneral);

    /*============= init Gtm_Tom0_CH9_TOUT1, P02_1 ==============*/
    IfxGtm_enable(&MODULE_GTM);                                     /* Enable GTM                                   */

    IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);   /* Enable the FXU clock                         */

    /* Initialize the configuration structure with default parameters */
    IfxGtm_Tom_Pwm_initConfig(&s_tomConfig_motor, &MODULE_GTM);

    s_tomConfig_motor.tom = _M_MOTORFL_PWM_TOUTMAP.tom;                                      /* Select the TOM depending on the LED_PWM          */
    s_tomConfig_motor.tomChannel = _M_MOTORFL_PWM_TOUTMAP.channel;                           /* Select the channel depending on the LED_PWM      */
    s_tomConfig_motor.period = PWM_PERIOD;                                /* Set the timer period                         */
    s_tomConfig_motor.pin.outputPin = &_M_MOTORFL_PWM_TOUTMAP;                               /* Set the LED_PWM port pin as output               */
    s_tomConfig_motor.synchronousUpdateEnabled = TRUE;                    /* Enable synchronous update                    */

    IfxGtm_Tom_Pwm_init(&s_tomDriver_motor, &s_tomConfig_motor);                /* Initialize the GTM TOM                       */
    IfxGtm_Tom_Pwm_start(&s_tomDriver_motor, TRUE);                       /* Start the PWM                                */


    set_motorFL_dutycycle(0);
    set_motorFL_direction(TRUE);
}


static inline void set_motorFL_dutycycle(float32 dutycycle){ // 0% ~ 100%
    s_tomConfig_motor.dutyCycle = (uint16) (dutycycle / 100 * PWM_PERIOD);                 /* Change the value of the duty cycle           */
    IfxGtm_Tom_Pwm_init(&s_tomDriver_motor, &s_tomConfig_motor);
}

static inline void set_motorFL_direction(boolean clock_wise){ // 1 : CW, 0: CCW
    if(clock_wise == TRUE)
    {
        IfxPort_setPinState(_P_MOTORFL_DIR, IfxPort_State_low);
    }
    else
    {
        IfxPort_setPinState(_P_MOTORFL_DIR, IfxPort_State_high);
    }
}

// 0% ~ 100%
static inline float32 get_motorFL_dutycycle(void) {
    return (float32)(s_tomConfig_motor.dutyCycle) * 100 / PWM_PERIOD;
}

// 1 : CW, 0 : CCW
static inline boolean get_motorFL_direction(void){
    return IfxPort_getPinState(_P_MOTORFL_PWM);
}


/*********************************************************************************************************************/
/*------------------------------------------encoder driver Implementations-------------------------------------------*/
/*********************************************************************************************************************/

IFX_INTERRUPT(encoderFL_chA_ISR, 0, ISR_PRIORITY_SCUERU0);
void encoderFL_chA_ISR(void){
    if(IfxPort_getPinState(_P_ENCODERFL_CHA)) { // rising edge triggered
        if(IfxPort_getPinState(_P_ENCODERFL_CHB)) {
            s_encoder_data--;
        } else {
            s_encoder_data++;
        }
    } else { // falling edge triggered
        if(IfxPort_getPinState(_P_ENCODERFL_CHB)) {
            s_encoder_data++;
        } else {
            s_encoder_data--;
        }
    }
}


static void init_encoderFL(void) {
    IfxPort_setPinModeInput(_P_ENCODERFL_CHB, IfxPort_InputMode_pullDown);

    /*======= init P14_1, ERU input Ch3, ERU output Ch0, SRC_SCU_SCU_ERU0 interrupt =======*/
    /* input multiplexers of the ERU, ERS configuration */
    IfxScuEru_initReqPin(&_M_ENCODERFL_CHA_REQ_IN, IfxPort_InputMode_pullDown);     /* Initialize this pin with pull-down enabled */

    /* Input channel, ETL configuration */
    IfxScuEru_InputChannel input_channel;
    input_channel = (IfxScuEru_InputChannel)_M_ENCODERFL_CHA_REQ_IN.channelId;   /* Determine input channel depending on input pin */
    IfxScuEru_enableRisingEdgeDetection(input_channel);     /* Interrupt triggers on rising edge (Register RENx) and  */
    IfxScuEru_enableFallingEdgeDetection(input_channel);    /* on falling edge (Register FENx)  */
    IfxScuEru_enableTriggerPulse(input_channel);            /* Enable generation of trigger event (Register EIENx) */

    /* Connecting Matrix, Event Trigger Logic ETL block */
    /* Determination of output channel for trigger event (Register INPx) */
    IfxScuEru_InputNodePointer triggerSelect = INTERRUPT_TRIGGER_CHANNEL;
    IfxScuEru_connectTrigger(input_channel, triggerSelect);    /* Event from input ETL3 triggers output OGU0 (signal TRx0) */

    /* output channel, OGU configuration */
    IfxScuEru_OutputChannel outputChannel = OUTPUT_CHANNEL;
    IfxScuEru_setInterruptGatingPattern(outputChannel, IfxScuEru_InterruptGatingPattern_alwaysActive);

    /* Service request configuration */
    /* Get source pointer depending on outputChannel (SRC_SCUERU0 for outputChannel0) */
    volatile Ifx_SRC_SRCR *src = &MODULE_SRC.SCU.SCU.ERU[(int) outputChannel % 4]; /* Service request control */
    IfxSrc_init(src, IfxSrc_Tos_cpu0, ISR_PRIORITY_SCUERU0);
    IfxSrc_enable(src);
}

static inline sint32 get_encoderFL_tick(void) {
    return s_encoder_data;
}

