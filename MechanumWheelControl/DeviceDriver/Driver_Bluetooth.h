/*
 * Driver_Bluetooth.h
 *
 * Created on: 2024. 5. 14.
 * Author: chanik
 */

#ifndef DEVICEDRIVER_DRIVER_BLUETOOTH_H_
#define DEVICEDRIVER_DRIVER_BLUETOOTH_H_

/***********************************************************************/
/*Include*/
/***********************************************************************/
#include "Ifx_Types.h"
#include "Driver_Joystick.h"

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
void init_bluetooth(void);

void send_bluetooth_joystick_values(uint32 x_mv, uint32 y_mv, uint32 x_rt, uint32 y_rt);
void receive_bluetooth_joystick_values(void);
JoystickValues get_bluetooth_joystick_values(void);


// config bluetooth function at AT command mode
void init_bluetooth_master(void);
void init_bluetooth_slave(void);

#endif /* DEVICEDRIVER_DRIVER_BLUETOOTH_H_ */
