/*
 * Interrupt_Priority.h
 *
 *  Created on: 2024. 5. 11.
 *      Author: jaeeun
 *
 ***************** Define Sensor or Actuator's Interrupt Priority
 */

#ifndef DEVICEDRIVER_INTERRUPTPRIORITY_H_
#define DEVICEDRIVER_INTERRUPTPRIORITY_H_

/* ToF channel - ASCLIN1 */
#define ISR_PRIORITY_ASCLIN1_TX     0x21    /* Priority for interrupt ASCLIN1 Transmit  */
#define ISR_PRIORITY_ASCLIN1_RX     0x20    /* Priority for interrupt ASCLIN1 Receive   */
#define ISR_PRIORITY_ASCLIN1_ER     0x22    /* Priority for interrupt ASCLIN1 Errors    */

/* bluetooth channel - ASCLIN2 */
#define ISR_PRIORITY_ASCLIN2_TX     0x41    /* Priority for interrupt ASCLIN2 Transmit  */
#define ISR_PRIORITY_ASCLIN2_RX     0x40    /* Priority for interrupt ASCLIN2 Receive   */
#define ISR_PRIORITY_ASCLIN2_ER     0x42    /* Priority for interrupt ASCLIN2 Errors    */

/* USB - ASCLIN3 */
#define ISR_PRIORITY_ASCLIN3_TX     0x31    /* Priority for interrupt ASCLIN3 Transmit  */
#define ISR_PRIORITY_ASCLIN3_RX     0x30    /* Priority for interrupt ASCLIN3 Receive   */
#define ISR_PRIORITY_ASCLIN3_ER     0x32    /* Priority for interrupt ASCLIN3 Errors    */


#define ISR_PRIORITY_SCUERU0        0x10    /* Priority for interrupt encoderFL chA */
#define ISR_PRIORITY_SCUERU1        0x11    /* Priority for interrupt encoderFR chA */
#define ISR_PRIORITY_SCUERU2        0x12    /* Priority for interrupt encoderRL chA */
#define ISR_PRIORITY_SCUERU3        0x13    /* Priority for interrupt encoderRR chA */

#define ISR_PRIORITY_STMSR0         0x64    /* Priority for interrupt STM0 */
#define ISR_PRIORITY_STMSR1         0x65    /* Priority for interrupt STM1 */


#endif /* DEVICEDRIVER_INTERRUPTPRIORITY_H_ */

