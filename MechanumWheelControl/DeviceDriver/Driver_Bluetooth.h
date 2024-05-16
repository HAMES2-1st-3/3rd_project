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

/* summary : initialize bluetooth with ASCLIN2
 * argu     >   none
 * return   >   none
 */
void init_bluetooth(void);

/* summary : send joystick value
 * argu     >   x_mv:   move joystick x-axivalues 
                y_mv:   move joystick y-axivalues
                x_rt:   rotate joystick x-axivalues 
                y_rt:   rotate joystick y-axivalues 
 * return   >   none
 */
void send_bluetooth_joystick_values(uint32 x_mv, uint32 y_mv, uint32 x_rt, uint32 y_rt);


/* summary : receive and save joystick values in static value in the source code
 * argu     >   none
 * return   >   none
 */
void receive_bluetooth_joystick_values(void);

/* summary : get the joystick values saved in source code(abstraction)
 * argu     >   none
 * return   >   none
 */
JoystickValues get_bluetooth_joystick_values(void);


/* summary : config bluetooth function at AT command mode
 * argu     >   none
 * return   >   none
 */
void init_bluetooth_master(void);
void init_bluetooth_slave(void);

#endif /* DEVICEDRIVER_DRIVER_BLUETOOTH_H_ */
