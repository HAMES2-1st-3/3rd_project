#ifndef DRIVER_JOYSTICK
#define DRIVER_JOYSTICK

/***********************************************************************/
/*Include*/
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxVadc.h"
#include "IfxVadc_Adc.h"
#include "IfxPort.h"
#include "IfxPort_PinMap.h"

#define MidAdc_GetAdcGroup2SenseRaw() (&stSensorAdc2Raw)
#define MidAdc_GetAdcGroup0SenseRaw() (&stSensorAdc0Raw)


/***********************************************************************/
/*Typedef*/
/***********************************************************************/

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


typedef struct{ //센싱 변수
        uint32 UlSSense1_Raw;
        uint32 UlSSense2_Raw;
        uint32 UlSSense3_Raw;
}SensorAdcRaw;

extern SensorAdcRaw stSensorAdc0Raw; //전력 센싱 변수
extern SensorAdcRaw stSensorAdc2Raw; //전력 센싱 변수

/***********************************************************************/
/*Define*/
/***********************************************************************/

/***********************************************************************/
/*External Variable*/
/***********************************************************************/
//IFX_EXTERN App_VadcAutoScan g_VadcAutoScan;
//extern volatile uint32_t adcDataResult[5];

/***********************************************************************/
/*Global Function Prototype*/
/***********************************************************************/

extern void Init_Joystick(void);
extern void DrvAdc_GetAdcRawGroup0(void);
extern void DrvAdc_GetAdcRawGroup2(void);

#endif /* DRIVER_STM */
