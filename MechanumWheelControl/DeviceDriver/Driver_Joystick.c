/***********************************************************************/
/*Include*/
/***********************************************************************/
#include "Driver_Joystick.h"

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
static void Driver_Adc0_Init(void);
static void Driver_Adc2_Init(void);
static void DrvAdc_Group0ChannelInit(uint8 param_ChNum);
static void DrvAdc_Group2ChannelInit(uint8 param_ChNum);

/***********************************************************************/
/*Variable*/
/***********************************************************************/
App_VadcAutoScan g_VadcAutoScan; //ADC 모듈 변수
App_VadcAutoScan g_VadcAutoScan2; //ADC 모듈 변수
IfxVadc_Adc_Channel adc0Channel[ADC_GROUP0_MAX];
SensorAdcRaw stSensorAdc0Raw; //adc raw 값 받을 변수
SensorAdcRaw stSensorAdc2Raw; //adc raw 값 받을 변수
IfxVadc_Adc_ChannelConfig adcChannelConfig[ADC_GROUP0_MAX];

/***********************************************************************/
/*Function*/
/***********************************************************************/

void Init_Joystick(void)
{
    /*ADC0 Converter Init*/
    Driver_Adc0_Init();
    Driver_Adc2_Init();
}

static void Driver_Adc2_Init(void)
{

    /* VADC Configuration */

    /* create configuration */
    IfxVadc_Adc_Config adcConfig;
    IfxVadc_Adc_initModuleConfig(&adcConfig, &MODULE_VADC);

    /* initialize module */
    IfxVadc_Adc_initModule(&g_VadcAutoScan.vadc, &adcConfig);

    /* create group config */
    IfxVadc_Adc_GroupConfig adcGroupConfig;
    IfxVadc_Adc_initGroupConfig(&adcGroupConfig, &g_VadcAutoScan.vadc);

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
    IfxVadc_Adc_initGroup(&g_VadcAutoScan.adcGroup, &adcGroupConfig);

    DrvAdc_Group2ChannelInit(ADC_GROUP2_CH5);
    DrvAdc_Group2ChannelInit(ADC_GROUP2_CH4); //channel init

    IfxVadc_Adc_startScan(&g_VadcAutoScan.adcGroup);

}

static void Driver_Adc0_Init(void)
{

    /* VADC Configuration */

    /* create configuration */
    IfxVadc_Adc_Config adcConfig;
    IfxVadc_Adc_initModuleConfig(&adcConfig, &MODULE_VADC);

    /* initialize module */
    IfxVadc_Adc_initModule(&g_VadcAutoScan2.vadc, &adcConfig);

    /* create group config */
    IfxVadc_Adc_GroupConfig adcGroupConfig;
    IfxVadc_Adc_initGroupConfig(&adcGroupConfig, &g_VadcAutoScan2.vadc);

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
    IfxVadc_Adc_initGroup(&g_VadcAutoScan2.adcGroup, &adcGroupConfig);

    DrvAdc_Group0ChannelInit(ADC_GROUP0_CH3);
    DrvAdc_Group0ChannelInit(ADC_GROUP0_CH2); //channel init

    IfxVadc_Adc_startScan(&g_VadcAutoScan2.adcGroup);

}



static void DrvAdc_Group2ChannelInit(uint8 param_ChNum)
{
    IfxVadc_Adc_ChannelConfig adcChannelConfigInfo;
    uint32 ulTemp = ((uint32)1u<<param_ChNum);

    IfxVadc_Adc_initChannelConfig(&adcChannelConfigInfo,&g_VadcAutoScan.adcGroup);

    adcChannelConfigInfo.channelId = (IfxVadc_ChannelId)(param_ChNum);
    adcChannelConfigInfo.resultRegister = (IfxVadc_ChannelResult)(param_ChNum);

    /*Initialize the channel*/
    IfxVadc_Adc_initChannel(&adc0Channel[param_ChNum],&adcChannelConfigInfo);

    /*add to scan*/
    IfxVadc_Adc_setScan(&g_VadcAutoScan.adcGroup,ulTemp,ulTemp);

}

static void DrvAdc_Group0ChannelInit(uint8 param_ChNum)
{
    IfxVadc_Adc_ChannelConfig adcChannelConfigInfo;
    uint32 ulTemp = ((uint32)1u<<param_ChNum);

    IfxVadc_Adc_initChannelConfig(&adcChannelConfigInfo,&g_VadcAutoScan2.adcGroup);

    adcChannelConfigInfo.channelId = (IfxVadc_ChannelId)(param_ChNum);
    adcChannelConfigInfo.resultRegister = (IfxVadc_ChannelResult)(param_ChNum);

    /*Initialize the channel*/
    IfxVadc_Adc_initChannel(&adc0Channel[param_ChNum],&adcChannelConfigInfo);

    /*add to scan*/
    IfxVadc_Adc_setScan(&g_VadcAutoScan2.adcGroup,ulTemp,ulTemp);

}

void DrvAdc_GetAdcRawGroup2(void){

    Ifx_VADC_RES conversionResult;


    conversionResult=IfxVadc_Adc_getResult(&adc0Channel[ADC_GROUP2_CH5]); //x_val
    stSensorAdc2Raw.UlSSense1_Raw=conversionResult.B.RESULT;
    conversionResult=IfxVadc_Adc_getResult(&adc0Channel[ADC_GROUP2_CH4]); //y_val
    stSensorAdc2Raw.UlSSense2_Raw=conversionResult.B.RESULT;

    IfxVadc_Adc_startScan(&g_VadcAutoScan.adcGroup);
}

void DrvAdc_GetAdcRawGroup0(void){

    Ifx_VADC_RES conversionResult;


    conversionResult=IfxVadc_Adc_getResult(&adc0Channel[ADC_GROUP0_CH3]); //x_val
    stSensorAdc0Raw.UlSSense1_Raw=conversionResult.B.RESULT;
    conversionResult=IfxVadc_Adc_getResult(&adc0Channel[ADC_GROUP0_CH2]); //y_val
    stSensorAdc0Raw.UlSSense2_Raw=conversionResult.B.RESULT;

    IfxVadc_Adc_startScan(&g_VadcAutoScan2.adcGroup);
}


