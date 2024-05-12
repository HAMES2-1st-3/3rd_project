/*
 * Driver_ASCLIN3.h
 *
 *  Created on: 2024. 5. 12.
 *      Author: jio
 */

#ifndef DEVICEDRIVER_DRIVER_ASCLIN3_H_
#define DEVICEDRIVER_DRIVER_ASCLIN3_H_


/***********************************************************************/
/*Include*/
/***********************************************************************/
#include "Ifx_Types.h"
#include "IfxAsclin_Asc.h"
#include "IfxPort.h"
#include <Interrupt_Priority.h>
/***********************************************************************/
/*Typedef*/
/***********************************************************************/

/***********************************************************************/
/*Define*/
/***********************************************************************/

/***********************************************************************/
/*External Variable*/
/***********************************************************************/

/***********************************************************************/
/*Global Function Prototype*/
/***********************************************************************/
void asclin3_tx_ISR(void);
void asclin3_rx_ISR(void);
void asclin3_err_ISR(void);
void init_asclin3(void);
void write_asclin3_uart(const uint8 chr);
uint8 read_asclin3_uart(void);




#endif /* DEVICEDRIVER_DRIVER_ASCLIN3_H_ */
