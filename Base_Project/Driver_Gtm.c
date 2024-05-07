/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Driver_Gtm.h"
#include "IfxCpu.h"


/***********************************************************************/
/*Define*/ 
/***********************************************************************/
#define ISR_ASSIGN(no,cpu) ((no<<8)+cpu)
#define ISR_PRIORITY(no_cpu) (no_cpu>>8)
#define ISR_PROVIDER(no_cpu) (no_cpu%8)

#define ISR_PRIORITY_TIMER 50
#define ISR_PROVIDER_TIMER IfxSrc_Tos_cpu0
#define INTERRUPT_TIMER ISR_ASSIGN(ISR_PRIORITY_TIMER,ISR_PROVIDER_TIMER)
/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/



/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/
static void Driver_Tom0_Init(void);


/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
App_GtmTomTimer g_GtmTomTimer; /**< \brief Demo information */
uint32 PWMCnt;
/***********************************************************************/
/*Function*/ 
/***********************************************************************/
IFX_INTERRUPT(GTM_Tom0IntHandler, 0, 50u); //ISR_Timer isr에 해당 , ISR_PRIORITY_TIMER

void GTM_Tom0IntHandler(void)
{
    IfxCpu_enableInterrupts();

    IfxGtm_Tom_Timer_acknowledgeTimerIrq(&g_GtmTomTimer.drivers.timerOneMs);
    g_GtmTomTimer.isrCounter.slotOneMs++;
}


static void Driver_Tom0_Init(void) //GtmTomPwmHl_initTimer() 와 동일
{
    {   /* GTM TOM configuration */
        IfxGtm_Tom_Timer_Config timerConfig;
        IfxGtm_Tom_PwmHl_Config pwmHlConfig; //추가
        IfxGtm_Tom_Timer_initConfig(&timerConfig, &MODULE_GTM);
        IfxGtm_Tom_PwmHl_initConfig(&pwmHlConfig);

        timerConfig.base.frequency       = 2000; //2kHz=> 주기 0.5ms
        timerConfig.base.isrPriority     = ISR_PRIORITY(INTERRUPT_TIMER); //ISR_PRIORITY_TIMER
        timerConfig.base.isrProvider= ISR_PROVIDER(INTERRUPT_TIMER);//ISR_PROVIEDER_TIMER
        timerConfig.base.minResolution   = (1.0 / timerConfig.base.frequency) / 1000;
        timerConfig.base.trigger.enabled = FALSE;
        timerConfig.clock                = IfxGtm_Tom_Ch_ClkSrc_cmuFxclk0;
        // [100Mhz / (2^4)]hz= 6,250,000 hz clock을 Tom 모듈로 공급
        //1초에 6250 ,000 count
        //3125 000      0.5s
        //3125 count하는데 0.5ms 마다 CM0도착하여 Timer Reset 인터럽트 날려
        timerConfig.base.countDir=IfxStdIf_Timer_CountDir_upAndDown;//timer counting mode : up and down

        //0.5ms(주기)마다 timer reset되고 인터럽트 날려
        timerConfig.tom                  = IfxGtm_Tom_0;
        timerConfig.timerChannel         = IfxGtm_Tom_Ch_0;
        //Triggerout에서 PWM이 나오는 pin을 추가

        timerConfig.triggerOut                      = &IfxGtm_TOM0_0_TOUT77_P15_6_OUT;//어디서? 모터 D11

        timerConfig.base.trigger.enabled            = TRUE;
        timerConfig.base.trigger.outputEnabled      = TRUE;   
        //triggerPoint=> CM1 설정 (CM1~CM0: low , CM0(리셋), CN0~CM1: High)
        timerConfig.base.trigger.triggerPoint       = 500u;   /*Default 0 duty*/
        // 0~ 500:high 501~3125: low
        timerConfig.base.trigger.risingEdgeAtPeriod = TRUE;
        
        IfxGtm_Tom_Timer_init(&g_GtmTomTimer.drivers.timerOneMs, &timerConfig);
 
        //추가

        IfxGtm_Tom_ToutMapP ccx[2],coutx[2];
        coutx[0]=&IfxGtm_TOM0_3_TOUT105_P10_3_OUT;
        ccx[0]=&IfxGtm_TOM0_4_TOUT22_P33_0_OUT;
        coutx[1]=&IfxGtm_TOM0_2_TOUT107_P10_5_OUT;
        ccx[1]=&IfxGtm_TOM0_5_TOUT23_P33_1_OUT;

        pwmHlConfig.timer=&g_GtmTomTimer.drivers.timerOneMs;
        pwmHlConfig.tom=timerConfig.tom;
        pwmHlConfig.base.deadtime=2e-6;
        pwmHlConfig.base.minPulse=1e-6;
        pwmHlConfig.base.channelCount=2;
        pwmHlConfig.base.emergencyEnabled=FALSE;
        pwmHlConfig.base.outputMode=IfxPort_OutputMode_none;
        pwmHlConfig.base.outputDriver=IfxPort_PadDriver_cmosAutomotiveSpeed1;
        pwmHlConfig.base.ccxActiveState=Ifx_ActiveState_high;
        pwmHlConfig.base.coutxActiveState=Ifx_ActiveState_low;
        pwmHlConfig.ccx=ccx;
        pwmHlConfig.coutx=coutx;


        IfxGtm_Tom_PwmHl_init(&g_GtmTomTimer.drivers.pwm,&pwmHlConfig);
        IfxGtm_Tom_Timer_run(&g_GtmTomTimer.drivers.timerOneMs);
    }
}


void Driver_Gtm_Init(void)
{
    /* disable interrupts */
    boolean  interruptState = IfxCpu_disableInterrupts();

    /** - GTM clocks */
    Ifx_GTM *gtm = &MODULE_GTM;
    g_GtmTomTimer.info.gtmFreq = IfxGtm_Cmu_getModuleFrequency(gtm);//cmu clokc =fgtm=100Mhz
    //cmu
    IfxGtm_enable(gtm);

    /* Set the ''global clock'' frequencies */
    IfxGtm_Cmu_setGclkFrequency(&MODULE_GTM, g_GtmTomTimer.info.gtmFreq);//100Mhz
    g_GtmTomTimer.info.gtmGclkFreq = IfxGtm_Cmu_getGclkFrequency(gtm);//Gclk=global clock

    //cmu clock 설정
    IfxGtm_Cmu_setClkFrequency(&MODULE_GTM,IfxGtm_Cmu_Clk_0,g_GtmTomTimer.info.gtmGclkFreq);
    g_GtmTomTimer.info.gtmCmuClk0Freq=IfxGtm_Cmu_getClkFrequency(gtm, IfxGtm_Cmu_Clk_0, TRUE); //tom0
    /** - Initialise the GTM part */

    //tom 초기화
    Driver_Tom0_Init(); //Tom 부분 설정(중요)
    //추가
    g_GtmTomTimer.info.timerValue=IfxGtm_Tom_Timer_getPeriod(&g_GtmTomTimer.drivers.timerOneMs);
    g_GtmTomTimer.tOn[0]=0;
    g_GtmTomTimer.tOn[1]=0;
    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);

    IfxGtm_Cmu_enableClocks(gtm, IFXGTM_CMU_CLKEN_FXCLK | IFXGTM_CMU_CLKEN_CLK0);
}

void Driver_Gtm_Tom0Test(float32_t fDuty)
{
    Ifx_TimerValue triggerPoint = 0u;
    //static float32_t fDuty = 0.5f;
    //1ms => 6250
    //10ms period=> 62500
    //2번채널은 16나누고 3번채널은 256나눔 ==> 6250 : 한 주기 0.5ms => 6250 /2 =3125
    //1sec 390,625 0.5sec 195312.5
    //1msec 390.625  0.5ms 195.3125

//    triggerPoint = (uint32_t)(fDuty * (100000000.0f/16.0f/2000.0f)); //31,25*2=6250에 fDuty에 따라
//    IfxGtm_Tom_Timer_disableUpdate(&g_GtmTomTimer.drivers.timerOneMs);
//    //cm1 설정 (triggerPoint)-절반값인 31250 이므로 duty 50%
//    IfxGtm_Tom_Timer_setTrigger(&g_GtmTomTimer.drivers.timerOneMs, triggerPoint);
//    //triggerPoint=0이면 첨부터 쭈욱 LOW
//    IfxGtm_Tom_Timer_applyUpdate(&g_GtmTomTimer.drivers.timerOneMs);
    g_GtmTomTimer.tOn[0]=fDuty;
    g_GtmTomTimer.tOn[1]=fDuty;
}
void GtmTomPwmHl_run(void){
    PWMCnt++;
    IfxGtm_Tom_PwmHl *pwmHl=&g_GtmTomTimer.drivers.pwm;
    IfxGtm_Tom_Timer *timer=&g_GtmTomTimer.drivers.timerOneMs;
    g_GtmTomTimer.drivers.timerOneMs.base.countDir=IfxStdIf_Timer_CountDir_upAndDown;
    IfxGtm_Tom_Timer_getPeriod(timer);
    Ifx_TimerValue DUTY[2];

    DUTY[0]=(uint32)(g_GtmTomTimer.tOn[0]*IfxGtm_Tom_Timer_getPeriod(timer));
    DUTY[1]=(uint32)(g_GtmTomTimer.tOn[1]*IfxGtm_Tom_Timer_getPeriod(timer));

    IfxGtm_Tom_PwmHl_setMode(pwmHl,Ifx_Pwm_Mode_centerAligned);
    IfxGtm_Tom_Timer_disableUpdate(timer);
    IfxGtm_Tom_PwmHl_setOnTime(pwmHl, DUTY); //triggerpoint 부분과 동일
    IfxGtm_Tom_Timer_applyUpdate(timer);

}
