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

#define ADC_CH_ID 1
#define ADC_GR_ID IfxVadc_GroupId_3

/***********************************************************************/
/*Typedef*/
/***********************************************************************/


/***********************************************************************/
/*Static Function Prototype*/
/***********************************************************************/

/***********************************************************************/
/*Variable*/
/***********************************************************************/
static IfxVadc_Adc_Channel   s_adc0Channel[8];


/***********************************************************************/
/*Function*/
/***********************************************************************/


void init_potentiometer(void)
{
    // variable register : AD6
    // luminous intensity : AD5

    uint32    chnIx = ADC_CH_ID;

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
    adcGroupConfig.groupId = ADC_GR_ID;
    adcGroupConfig.master  = adcGroupConfig.groupId;

    /* enable scan source */
    adcGroupConfig.arbiter.requestSlotScanEnabled = TRUE;

    /* enable auto scan */
    adcGroupConfig.scanRequest.autoscanEnabled = TRUE;

    /* enable all gates in "always" mode (no edge detection) */
    adcGroupConfig.scanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_always;

    /* initialize the group */
    /*IfxVadc_Adc_Group adcGroup;*/    //declared globally
    IfxVadc_Adc_Group adcGroup;
    IfxVadc_Adc_initGroup(&adcGroup, &adcGroupConfig);



    /*************************************channel init*******************************************/
    IfxVadc_Adc_ChannelConfig s_adc0ChannelConfig[8];

    IfxVadc_Adc_initChannelConfig(&s_adc0ChannelConfig[chnIx], &adcGroup);

    s_adc0ChannelConfig[chnIx].channelId      = (IfxVadc_ChannelId)(chnIx);
    s_adc0ChannelConfig[chnIx].resultRegister = (IfxVadc_ChannelResult)(chnIx);  /* use dedicated result register */

    /* initialize the channel */
    IfxVadc_Adc_initChannel(&s_adc0Channel[chnIx], &s_adc0ChannelConfig[chnIx]);

    /* add to scan */
    unsigned channels = (1 << s_adc0ChannelConfig[chnIx].channelId);
    unsigned mask     = channels;
    IfxVadc_Adc_setScan(&adcGroup, channels, mask);
    /********************************************************************************************/

    IfxVadc_Adc_startScan(&adcGroup);
}


uint32 get_potentiometer_value(void)
{
    uint32 adc0DataResult[8];

    uint32    chnIx = ADC_CH_ID;
    Ifx_VADC_RES conversionResult; /* wait for valid result */

    /* check results */
    do
    {
        conversionResult = IfxVadc_Adc_getResult(&s_adc0Channel[chnIx]);
    } while (!conversionResult.B.VF);

    adc0DataResult[chnIx] = conversionResult.B.RESULT;
    return adc0DataResult[chnIx];
}


