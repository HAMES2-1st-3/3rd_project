/*
 * Driver_USB.c
 *
 *  Created on: 2024. 5. 12.
 *      Author: jio
 */




/***********************************************************************/
/*Include*/
/***********************************************************************/
#include <Driver_USB.h>
#include <Interrupt_Priority.h>

#include "_Utilities/Ifx_Assert.h"

#include "IfxAsclin_Asc.h"
#include "IfxPort.h"


#include <string.h>
#include <stdio.h>
#include <stdarg.h>
/***********************************************************************/
/*Define*/
/***********************************************************************/
#define USB_UART_MAX_PRINT_SIZE (255)

#define ASC3_TX_BUFFER_SIZE          256                                     /* Define the TX buffer size in byte    */
#define ASC3_RX_BUFFER_SIZE          256                                     /* Define the RX buffer size in byte    */
#define ASC3_BAUDRATE                115200                                  /* Define the UART baud rate            */

/***********************************************************************/
/*Typedef*/
/***********************************************************************/

/***********************************************************************/
/*Static Function Prototype*/
/***********************************************************************/

/***********************************************************************/
/*Variable*/
/***********************************************************************/
IfxAsclin_Asc g_asclin3;
uint8 g_asclin3_tx_buf[ASC3_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
uint8 g_asclin3_rx_buf[ASC3_RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

/***********************************************************************/
/*Function*/
/***********************************************************************/


void usb_printf(pchar format,...)
{
    char      message[USB_UART_MAX_PRINT_SIZE + 1];
    Ifx_SizeT count;
    va_list   args;
    va_start(args, format);
    vsprintf((char *)message, format, args);
    va_end(args);
    count = (Ifx_SizeT)strlen(message);
    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, count < STDIF_DPIPE_MAX_PRINT_SIZE);
    usb_print_string(message, USB_UART_MAX_PRINT_SIZE);
    //return
}



void usb_print_string(char *str, uint32 max_len) {
    uint32 count = 0;

    char ch = *str;
    while(ch != 0 && count < max_len) {
        if(ch == '\n') {
            IfxAsclin_Asc_blockingWrite(&g_asclin3, '\r');
            IfxAsclin_Asc_blockingWrite(&g_asclin3, '\n');
        } else {
            IfxAsclin_Asc_blockingWrite(&g_asclin3, ch);
        }
        str++;
        max_len++;
        ch = *str;
    }
}

void init_usb() {

    /* ======================= init_asclin3 ======================= */
    IfxAsclin_Asc_Config asc_conf;

    /* Set default configurations */
    IfxAsclin_Asc_initModuleConfig(&asc_conf, &MODULE_ASCLIN3); /* Initialize the structure with default values      */

    /* Set the desired baud rate */
    asc_conf.baudrate.baudrate = ASC3_BAUDRATE;                                   /* Set the baud rate in bit/s       */
    asc_conf.baudrate.oversampling = IfxAsclin_OversamplingFactor_16;            /* Set the oversampling factor      */

    /* Configure the sampling mode */
    asc_conf.bitTiming.medianFilter = IfxAsclin_SamplesPerBit_three;             /* Set the number of samples per bit*/
    asc_conf.bitTiming.samplePointPosition = IfxAsclin_SamplePointPosition_8;    /* Set the first sample position    */

    /* ISR priorities and interrupt target */
    asc_conf.interrupt.txPriority = ISR_PRIORITY_ASCLIN3_TX;  /* Set the interrupt priority for TX events             */
    asc_conf.interrupt.rxPriority = ISR_PRIORITY_ASCLIN3_RX;  /* Set the interrupt priority for RX events             */
    asc_conf.interrupt.erPriority = ISR_PRIORITY_ASCLIN3_ER;  /* Set the interrupt priority for Error events          */
    asc_conf.interrupt.typeOfService = IfxSrc_Tos_cpu0;

    /* Pin configuration */
    const IfxAsclin_Asc_Pins pins = {
          .cts        = NULL_PTR,                         /* CTS pin not used                                     */
          .ctsMode    = IfxPort_InputMode_pullUp,
          .rx         = &IfxAsclin3_RXD_P32_2_IN ,        /* Select the pin for RX connected to the USB port      */
          .rxMode     = IfxPort_InputMode_pullUp,         /* RX pin                                               */
          .rts        = NULL_PTR,                         /* RTS pin not used                                     */
          .rtsMode    = IfxPort_OutputMode_pushPull,
          .tx         = &IfxAsclin3_TX_P15_7_OUT,         /* Select the pin for TX connected to the USB port      */
          .txMode     = IfxPort_OutputMode_pushPull,      /* TX pin                                               */
          .pinDriver  = IfxPort_PadDriver_cmosAutomotiveSpeed1
    };
    asc_conf.pins = &pins;

    /* FIFO buffers configuration */
    asc_conf.txBuffer = g_asclin3_tx_buf;                      /* Set the transmission buffer                          */
    asc_conf.txBufferSize = ASC3_TX_BUFFER_SIZE;              /* Set the transmission buffer size                     */
    asc_conf.rxBuffer = g_asclin3_rx_buf;                      /* Set the receiving buffer                             */
    asc_conf.rxBufferSize = ASC3_RX_BUFFER_SIZE;              /* Set the receiving buffer size                        */

    /* Init ASCLIN module */
    IfxAsclin_Asc_initModule(&g_asclin3, &asc_conf);          /* Initialize the module with the given configuration   */

}



IFX_INTERRUPT(asclin3_tx_ISR, 0, ISR_PRIORITY_ASCLIN3_TX);
void asclin3_tx_ISR(void)
{
    IfxAsclin_Asc_isrTransmit(&g_asclin3);
    // 이게 없으면 첫 문자만 보내지고 그 이후에는 전송이 안됨.
}

IFX_INTERRUPT(asclin3_rx_ISR, 0, ISR_PRIORITY_ASCLIN3_RX);
void asclin3_rx_ISR(void)
{
    IfxAsclin_Asc_isrReceive(&g_asclin3);
}

IFX_INTERRUPT(asclin3_err_ISR, 0, ISR_PRIORITY_ASCLIN3_ER);
void asclin3_err_ISR(void)
{
    ;
}



