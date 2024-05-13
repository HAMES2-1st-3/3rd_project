/*
 * Driver_Potentiometer.c
 *
 *  Created on: 2024. 5. 10.
 *      Author: jaeeun
 */

/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <Driver_Potentiometer.h>
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
static void init_channel(uint8 param_ChNum);
/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
static App_VadcAutoScan s_vadc_autoscan;
static IfxVadc_Adc_Channel s_adc0_channel[ADC_GROUP4_MAX];
uint32 potentiometer_raw;

/***********************************************************************/
/*Function*/ 
/***********************************************************************/

void init_potentiometer(void)
{
    /* VADC Configuration */

    /* create configuration */
    IfxVadc_Adc_Config adc_config;
    IfxVadc_Adc_initModuleConfig(&adc_config, &MODULE_VADC);

    /* initialize module */
    IfxVadc_Adc_initModule(&s_vadc_autoscan.vadc, &adc_config);

    /* create group config */
    IfxVadc_Adc_GroupConfig adc_group_config;
    IfxVadc_Adc_initGroupConfig(&adc_group_config, &s_vadc_autoscan.vadc);

    /* with group 0 */
    adc_group_config.groupId = IfxVadc_GroupId_4;
    adc_group_config.master  = adc_group_config.groupId;

    /* enable scan source */
    adc_group_config.arbiter.requestSlotScanEnabled = TRUE;

    /* enable auto scan */
    adc_group_config.scanRequest.autoscanEnabled = TRUE;

    /* enable all gates in "always" mode (no edge detection) */
    adc_group_config.scanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_always;

    /* initialize the group */
    /*IfxVadc_Adc_Group adcGroup;*/    //declared globally
    IfxVadc_Adc_initGroup(&s_vadc_autoscan.adcGroup, &adc_group_config);

    init_channel(ADC_GROUP4_CH7); //channel init

    IfxVadc_Adc_startScan(&s_vadc_autoscan.adcGroup);

}

static void init_channel(uint8 param_ChNum)
{
    IfxVadc_Adc_ChannelConfig adc_channel_config_info;
    uint32 ul_temp = ((uint32)1u << param_ChNum);

    IfxVadc_Adc_initChannelConfig(&adc_channel_config_info,&s_vadc_autoscan.adcGroup);

    adc_channel_config_info.channelId = (IfxVadc_ChannelId)(param_ChNum);
    adc_channel_config_info.resultRegister = (IfxVadc_ChannelResult)(param_ChNum);

    /*Initialize the channel*/
    IfxVadc_Adc_initChannel(&s_adc0_channel[param_ChNum],&adc_channel_config_info);

    /*add to scan*/
    IfxVadc_Adc_setScan(&s_vadc_autoscan.adcGroup,ul_temp,ul_temp);
}

uint32 get_potentiometer_value(void){ // 0 ~ 4095
    Ifx_VADC_RES conversion_result;

    conversion_result=IfxVadc_Adc_getResult(&s_adc0_channel[ADC_GROUP4_CH7]); //x_val
    potentiometer_raw = conversion_result.B.RESULT;

    return potentiometer_raw;
}
