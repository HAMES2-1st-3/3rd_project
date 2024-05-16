/*
 * Driver_USB.h
 *
 *  Created on: 2024. 5. 12.
 *      Author: jio
 */

#ifndef DEVICEDRIVER_DRIVER_USB_H_
#define DEVICEDRIVER_DRIVER_USB_H_



/***********************************************************************/
/*Include*/
/***********************************************************************/
#include "Ifx_Types.h"

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

/* summary : initialize USB communication port(UART) with PC
 * argu     >   none
 * return   >   none
 */
void init_usb(void);

/* summary : send format string message to PC 
 * argu     >   format: format string  
 * return   >   none
 */
void send_usb_printf(pchar format,...);


#endif /* DEVICEDRIVER_DRIVER_USB_H_ */
