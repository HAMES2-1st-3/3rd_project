/*
 * Driver_Joystick.c
 *
 *  Created on: 2024. 5. 10.
 *      Author: howon
 */
/***********************************************************************/
/*Include*/
/***********************************************************************/
#include <Driver_Joystick.h>

#include <PortPinMapping.h>

#include "IfxVadc.h"
#include "IfxVadc_Adc.h"
#include "IfxPort_PinMap.h"

/***********************************************************************/
/*Define*/
/***********************************************************************/

/***********************************************************************/
/*Typedef*/
/***********************************************************************/
typedef struct
{
    IfxVadc_Adc vadc; /* VADC handle */
    IfxVadc_Adc_Group adcGroup;
} App_VadcAutoScan;


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

/***********************************************************************/
/*Static Function Prototype*/
/***********************************************************************/
static void init_joystick_move(void);
static void init_joystick_rotate(void);
static void init_group0_channel(uint32 vadc_ch_num);
static void init_group2_channel(uint32 vadc_ch_num);

/***********************************************************************/
/*Variable*/
/***********************************************************************/

static IfxVadc_Adc_Group s_adcGroup0;
static IfxVadc_Adc_Group s_adcGroup2;

static IfxVadc_Adc_Channel s_adc0_channel[ADC_GROUP0_MAX];
static IfxVadc_Adc_Channel s_adc2_channel[ADC_GROUP2_MAX];


/***********************************************************************/
/*Function*/
/***********************************************************************/

void init_joystick(void)
{
    /*ADC0 Converter Init*/
    init_joystick_move();
    init_joystick_rotate();
}

static void init_joystick_move(void)
{
    /* VADC Configuration */

    /* create configuration */
    IfxVadc_Adc_Config adcConfig;
    IfxVadc_Adc_initModuleConfig(&adcConfig, &MODULE_VADC);

    /* initialize module */
    IfxVadc_Adc vadc;
    IfxVadc_Adc_initModule(&vadc, &adcConfig);

    /* create group config */
    IfxVadc_Adc_GroupConfig adcGroupConfig;
    IfxVadc_Adc_initGroupConfig(&adcGroupConfig, &vadc);

    /* with group 0 */
    adcGroupConfig.groupId = _M_JOYSTICK_MOVE_ADC_GP_ID;
    adcGroupConfig.master  = adcGroupConfig.groupId;

    /* enable scan source */
    adcGroupConfig.arbiter.requestSlotScanEnabled = TRUE;

    /* enable auto scan */
    adcGroupConfig.scanRequest.autoscanEnabled = TRUE;

    /* enable all gates in "always" mode (no edge detection) */
    adcGroupConfig.scanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_always;

    /* initialize the group */
    /*IfxVadc_Adc_Group adcGroup;*/    //declared globally
    IfxVadc_Adc_initGroup(&s_adcGroup0, &adcGroupConfig);

    init_group0_channel(IfxVadc_ChannelId_3);
    init_group0_channel(IfxVadc_ChannelId_2); //channel init

    IfxVadc_Adc_startScan(&s_adcGroup0);

}

static void init_group0_channel(uint32 vadc_ch_num)
{
    IfxVadc_Adc_ChannelConfig g_adc_channel_configInfo;

    IfxVadc_Adc_initChannelConfig(&g_adc_channel_configInfo,&s_adcGroup0);

    g_adc_channel_configInfo.channelId = (IfxVadc_ChannelId)(vadc_ch_num);
    g_adc_channel_configInfo.resultRegister = (IfxVadc_ChannelResult)(vadc_ch_num);

    /*Initialize the channel*/
    IfxVadc_Adc_initChannel(&s_adc0_channel[vadc_ch_num],&g_adc_channel_configInfo);

    /*add to scan*/
    unsigned channels = (1 << vadc_ch_num);
    unsigned mask     = channels;
    IfxVadc_Adc_setScan(&s_adcGroup0, channels, mask);
}



static void init_joystick_rotate(void)
{
    /* VADC Configuration */

    /* create configuration */
    IfxVadc_Adc_Config adcConfig;
    IfxVadc_Adc_initModuleConfig(&adcConfig, &MODULE_VADC);

    /* initialize module */
    IfxVadc_Adc vadc;
    IfxVadc_Adc_initModule(&vadc, &adcConfig);

    /* create group config */
    IfxVadc_Adc_GroupConfig adcGroupConfig;
    IfxVadc_Adc_initGroupConfig(&adcGroupConfig, &vadc);

    /* with group 0 */
    adcGroupConfig.groupId = _M_JOYSTICK_ROTATE_ADC_GP_ID;
    adcGroupConfig.master  = adcGroupConfig.groupId;

    /* enable scan source */
    adcGroupConfig.arbiter.requestSlotScanEnabled = TRUE;

    /* enable auto scan */
    adcGroupConfig.scanRequest.autoscanEnabled = TRUE;

    /* enable all gates in "always" mode (no edge detection) */
    adcGroupConfig.scanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_always;

    /* initialize the group */
    /*IfxVadc_Adc_Group adcGroup;*/    //declared globally
    IfxVadc_Adc_initGroup(&s_adcGroup2, &adcGroupConfig);

    init_group2_channel(_M_JOYSTICK_ROTATE_X_ADC_CH_ID);
    init_group2_channel(_M_JOYSTICK_ROTATE_Y_ADC_CH_ID); //channel init

    IfxVadc_Adc_startScan(&s_adcGroup2);
}


static void init_group2_channel(uint32 vadc_ch_num)
{
    IfxVadc_Adc_ChannelConfig g_adc_channel_configInfo;

    IfxVadc_Adc_initChannelConfig(&g_adc_channel_configInfo,&s_adcGroup2);

    g_adc_channel_configInfo.channelId = (IfxVadc_ChannelId)(vadc_ch_num);
    g_adc_channel_configInfo.resultRegister = (IfxVadc_ChannelResult)(vadc_ch_num);

    /*Initialize the channel*/
    IfxVadc_Adc_initChannel(&s_adc2_channel[vadc_ch_num],&g_adc_channel_configInfo);

    /*add to scan*/
    unsigned channels = (1 << vadc_ch_num);
    unsigned mask     = channels;
    IfxVadc_Adc_setScan(&s_adcGroup2, channels, mask);
}

JoystickValue get_joystick_move_value(void){
    JoystickValue value;

    Ifx_VADC_RES conversion_res; /* wait for valid result */

    /* check results */
    do
    {
        conversion_res = IfxVadc_Adc_getResult(&s_adc0_channel[_M_JOYSTICK_MOVE_X_ADC_CH_ID]);
    } while (!conversion_res.B.VF);

    value.x = conversion_res.B.RESULT;

    /* check results */
    do
    {
        conversion_res = IfxVadc_Adc_getResult(&s_adc0_channel[_M_JOYSTICK_MOVE_Y_ADC_CH_ID]);
    } while (!conversion_res.B.VF);

    value.y = conversion_res.B.RESULT;
    value.sw = FALSE;

   return value;
}


JoystickData get_joystick_move_data(void) {
    JoystickValue value;
    JoystickData data;

    value = get_joystick_move_value();
    data.x = ((float32)(value.x) * 100 / JOYSTICK_MAX_VALUE);
    data.y = ((float32)(value.y) * 100 / JOYSTICK_MAX_VALUE);

    return data;
}


JoystickValue get_joystick_rotate_value(void){
    JoystickValue value;

    Ifx_VADC_RES conversion_res; /* wait for valid result */

    /* check results */
    do
    {
        conversion_res = IfxVadc_Adc_getResult(&s_adc2_channel[_M_JOYSTICK_ROTATE_X_ADC_CH_ID]);
    } while (!conversion_res.B.VF);

    value.x = conversion_res.B.RESULT;

    /* check results */
    do
    {
        conversion_res = IfxVadc_Adc_getResult(&s_adc2_channel[_M_JOYSTICK_ROTATE_Y_ADC_CH_ID]);
    } while (!conversion_res.B.VF);

    value.y = conversion_res.B.RESULT;
    value.sw = FALSE;

   return value;
}

JoystickData get_joystick_rotate_data(void) {
    JoystickValue value;
    JoystickData data;

    value = get_joystick_rotate_value();
    data.x = ((float32)(value.x) * 100 / JOYSTICK_MAX_VALUE);
    data.y = ((float32)(value.y) * 100 / JOYSTICK_MAX_VALUE);

    return data;
}
