#ifndef _COMMON_H
#define COMMON_H

#include "device.h"

/***********************************************************************
 * ------------------------ Timer settings -----------------------------
 **********************************************************************/
#define _10MHZ	63320
#define _16MHZ	61768
#define _20MHZ	60768
#define _32MHZ	57768
#define _40MHZ 	55768

/***********************************************************************
 * ----------------------------- Events --------------------------------
 **********************************************************************/
#define ALARM_EVENT       0x80
#define LCD_EVENT         0x01

/***********************************************************************
 * ----------------------------- Task ID -------------------------------
 **********************************************************************/
/* Info about the tasks:
 * TASK0: USART
 * TASK1: USART
 */
#define TASK0_ID             1
#define TASK1_ID             2
#define TASK0_ID_LCD         3
#define LCD_ID               5

/* Priorities of the tasks */
#define TASK0_PRIO           10
#define TASK1_PRIO           9
#define TASK0_LCD_PRIO      8
#define LCD_PRIO            7


#define ALARM_TSK0           0
#define ALARM_TSK1           1
#define ALARM_TSK0_LCD       2       /* Alarm ID (index) in tascdesc.c */
#define LCD_ALARM_ID         3       /* Alarm ID (index) in tascdesc.c */

/**********************************************************************
 * ----------------------- GLOBAL DEFINITIONS -------------------------
 **********************************************************************/

/**********************************************************************
 * ----------------------- FUNCTION PROTOTYPES ------------------------
 **********************************************************************/
 /* transmits data using serial communication */
void transmitData();
/* Invoked when receive interrupt occurs; meaning that data is received */
void dataReceived();

void transmitCharAndHello(char chr);


#endif

/* End of File : common.h */
