/*
 * Driver_ToF.h
 *
 *  Created on: 2024. 5. 10.
 *      Author: jaeeun
 */

#ifndef DEVICEDRIVER_DRIVER_TOF_H_
#define DEVICEDRIVER_DRIVER_TOF_H_


/***********************************************************************/
/*Include*/
/***********************************************************************/
#include "IfxPort_reg.h"
#include "IfxPort_bf.h"
#include "IfxPort.h"
#include "IfxPort_PinMap.h"
#include "IfxAsclin_reg.h"
#include "IfxAsclin_bf.h"
#include "IfxAsclin_Asc.h"
#include "IfxSrc_reg.h"

/***********************************************************************/
/*Typedef*/
/***********************************************************************/

/***********************************************************************/
/*Define*/
/***********************************************************************/
#define ASC1_TX_BUFFER_SIZE          256                                     /* Define the TX buffer size in byte    */
#define ASC1_RX_BUFFER_SIZE          256
#define ASC1_BAUDRATE                921600

/***********************************************************************/
/*External Variable*/
/***********************************************************************/
/***********************************************************************/
/*Global Function Prototype*/
/***********************************************************************/


/* summary : initialize tof sensor, it communication using ASCLIN1(UART)
 * argu     >   none
 * return   >   none
 */
void init_tof(void);

/* summary : initialize tof sensor, it communication using ASCLIN1(UART)
 * argu     >   none
 * return   >   (sint32)    : give disttance in mm(millimeter)
 *                            negative value indicate error   
 */
sint32 get_tof_distance(void);


#endif /* DEVICEDRIVER_DRIVER_TOF_H_ */
