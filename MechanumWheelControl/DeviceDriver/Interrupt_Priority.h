/*
 * Interrupt_Priority.h
 *
 *  Created on: 2024. 5. 11.
 *      Author: jaeeun
 *
 ***************** Define Sensor or Actuator's Interrupt Priority
 */

#ifndef DEVICEDRIVER_INTERRUPT_PRIORITY_H_
#define DEVICEDRIVER_INTERRUPT_PRIORITY_H_

#define ISR_PRIORITY_ASCLIN1_TX     0x21                                        /* Priority for interrupt ASCLIN1 Transmit  */
#define ISR_PRIORITY_ASCLIN1_RX     0x20                                        /* Priority for interrupt ASCLIN1 Receive   */
#define ISR_PRIORITY_ASCLIN1_ER     0x22                                        /* Priority for interrupt ASCLIN1 Errors    */

#define ISR_PRIORITY_ASCLIN3_TX      0x31                                       /* Priority for interrupt ISR Transmit  */
#define ISR_PRIORITY_ASCLIN3_RX      0x30                                       /* Priority for interrupt ISR Receive   */
#define ISR_PRIORITY_ASCLIN3_ER      0x32


#endif /* DEVICEDRIVER_INTERRUPT_PRIORITY_H_ */
