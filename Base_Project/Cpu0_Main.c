/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include <App/AppScheduling.h>
#include <Device_driver/Driver_Adc.h>
#include <Device_driver/Driver_Asc.h>
#include <Device_driver/Driver_encoder.h>
#include <Device_driver/Driver_Gtm.h>
#include <Device_driver/Driver_Port.h>
#include <Device_driver/Driver_Stm.h>
#include <Device_driver/Driver_Watchdog.h>
#include <Device_driver/ASCLIN3_USB_UART.h>
#include "Ifx_Types.h"
#include "IfxCpu.h"
/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/

/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/


/***********************************************************************/
/*Variable*/ 
/***********************************************************************/

/***********************************************************************/
/*Function*/ 
/***********************************************************************/
int core0_main(void)
{   
    /*Driver Init*/
    Driver_Watchdog_Init();
    Driver_Port_Init();
    Driver_Stm_Init();
    //Driver_Adc_Init();
//    Driver_Asc_Init();
    Driver_Gtm_Init();
    Driver_ASCLIN3_init();
    /*Global Interrupt Enable*/
    IfxCpu_enableInterrupts();
    encoder_init();
    while(1)
    {
        AppScheduling();
    }
    return (1);
}
