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



/***********************************************************************/
/*Static Function Prototype*/
/***********************************************************************/
static void init_joystick_move(void);
static void init_joystick_rotate(void);
static void init_group0_channel(uint8 param_ChNum);
static void init_group2_channel(uint8 param_ChNum);

/***********************************************************************/
/*Variable*/
/***********************************************************************/
App_VadcAutoScan s_vadc_autoscan0;
App_VadcAutoScan s_vadc_autoscan2;
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
    IfxVadc_Adc_initModule(&s_vadc_autoscan0.vadc, &adcConfig);

    /* create group config */
    IfxVadc_Adc_GroupConfig adcGroupConfig;
    IfxVadc_Adc_initGroupConfig(&adcGroupConfig, &s_vadc_autoscan0.vadc);

    /* with group 0 */
    adcGroupConfig.groupId = IfxVadc_GroupId_0;
    adcGroupConfig.master  = adcGroupConfig.groupId;

    /* enable scan source */
    adcGroupConfig.arbiter.requestSlotScanEnabled = TRUE;

    /* enable auto scan */
    adcGroupConfig.scanRequest.autoscanEnabled = TRUE;

    /* enable all gates in "always" mode (no edge detection) */
    adcGroupConfig.scanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_always;

    /* initialize the group */
    /*IfxVadc_Adc_Group adcGroup;*/    //declared globally
    IfxVadc_Adc_initGroup(&s_vadc_autoscan0.adcGroup, &adcGroupConfig);

    init_group0_channel(ADC_GROUP0_CH3);
    init_group0_channel(ADC_GROUP0_CH2); //channel init

    IfxVadc_Adc_startScan(&s_vadc_autoscan0.adcGroup);

}

static void init_joystick_rotate(void)
{

    /* VADC Configuration */

    /* create configuration */
    IfxVadc_Adc_Config adcConfig;
    IfxVadc_Adc_initModuleConfig(&adcConfig, &MODULE_VADC);

    /* initialize module */
    IfxVadc_Adc_initModule(&s_vadc_autoscan2.vadc, &adcConfig);

    /* create group config */
    IfxVadc_Adc_GroupConfig adcGroupConfig;
    IfxVadc_Adc_initGroupConfig(&adcGroupConfig, &s_vadc_autoscan2.vadc);

    /* with group 0 */
    adcGroupConfig.groupId = IfxVadc_GroupId_2;
    adcGroupConfig.master  = adcGroupConfig.groupId;

    /* enable scan source */
    adcGroupConfig.arbiter.requestSlotScanEnabled = TRUE;

    /* enable auto scan */
    adcGroupConfig.scanRequest.autoscanEnabled = TRUE;

    /* enable all gates in "always" mode (no edge detection) */
    adcGroupConfig.scanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_always;

    /* initialize the group */
    /*IfxVadc_Adc_Group adcGroup;*/    //declared globally
    IfxVadc_Adc_initGroup(&s_vadc_autoscan2.adcGroup, &adcGroupConfig);

    init_group2_channel(ADC_GROUP2_CH5);
    init_group2_channel(ADC_GROUP2_CH4); //channel init

    IfxVadc_Adc_startScan(&s_vadc_autoscan2.adcGroup);

}


static void init_group0_channel(uint8 param_ChNum)
{
    IfxVadc_Adc_ChannelConfig g_adc_channel_configInfo;
    uint32 ulTemp = ((uint32)1u<<param_ChNum);

    IfxVadc_Adc_initChannelConfig(&g_adc_channel_configInfo,&s_vadc_autoscan0.adcGroup);

    g_adc_channel_configInfo.channelId = (IfxVadc_ChannelId)(param_ChNum);
    g_adc_channel_configInfo.resultRegister = (IfxVadc_ChannelResult)(param_ChNum);

    /*Initialize the channel*/
    IfxVadc_Adc_initChannel(&s_adc0_channel[param_ChNum],&g_adc_channel_configInfo);

    /*add to scan*/
    IfxVadc_Adc_setScan(&s_vadc_autoscan0.adcGroup,ulTemp,ulTemp);

}

static void init_group2_channel(uint8 param_ChNum)
{
    IfxVadc_Adc_ChannelConfig g_adc_channel_configInfo;
    uint32 ulTemp = ((uint32)1u<<param_ChNum);

    IfxVadc_Adc_initChannelConfig(&g_adc_channel_configInfo,&s_vadc_autoscan2.adcGroup);

    g_adc_channel_configInfo.channelId = (IfxVadc_ChannelId)(param_ChNum);
    g_adc_channel_configInfo.resultRegister = (IfxVadc_ChannelResult)(param_ChNum);

    /*Initialize the channel*/
    IfxVadc_Adc_initChannel(&s_adc2_channel[param_ChNum],&g_adc_channel_configInfo);

    /*add to scan*/
    IfxVadc_Adc_setScan(&s_vadc_autoscan2.adcGroup,ulTemp,ulTemp);

}

JoystickValue get_joystick_move_value(void){

    JoystickValue value;
    volatile Ifx_VADC_RES conversion_res;

    conversion_res=IfxVadc_Adc_getResult(&s_adc0_channel[ADC_GROUP0_CH3]); //x_val
    value.x=conversion_res.B.RESULT;
    conversion_res=IfxVadc_Adc_getResult(&s_adc0_channel[ADC_GROUP0_CH2]); //y_val
    value.y=conversion_res.B.RESULT;

    return value;
}


JoystickValue get_joystick_rotate_value(void){

    JoystickValue value;
    volatile Ifx_VADC_RES conversion_res;

    conversion_res=IfxVadc_Adc_getResult(&s_adc2_channel[ADC_GROUP2_CH5]); //x_val
    value.x=(uint32)conversion_res.B.RESULT;
    conversion_res=IfxVadc_Adc_getResult(&s_adc2_channel[ADC_GROUP2_CH4]); //y_val
    value.y=(uint32)conversion_res.B.RESULT;

//    usb_printf("I: rotate_x_val:%u,rotate_y_val:%u\n",value.x,value.y);

    return value;
}
