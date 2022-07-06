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
#define TASK0_EVENT       0x02
#define TASK2_EVENT       0x01

/***********************************************************************
 * ----------------------------- Task ID -------------------------------
 **********************************************************************/
/* Info about the tasks:
 * TASK0: USART
 * TASK1: USART
 */
#define TASK0_ID             1
#define TASK1_ID             2
#define TASK2_ID             3

/* Priorities of the tasks */
#define TASK0_PRIO           9
#define TASK1_PRIO           8
#define TASK2_PRIO           5

#define ALARM_TSK0           0
#define ALARM_TSK1           1


/**********************************************************************
 * ----------------------- GLOBAL DEFINITIONS -------------------------
 **********************************************************************/

// extern unsigned char hash_params[16];
// extern unsigned char go_params[2];
// extern unsigned char sensor_params[3];
// extern unsigned char alert_params[8];
// extern unsigned char payment_params[2];

// extern unsigned int go_counter = 0;
// extern unsigned int sensor_counter;
// extern unsigned int alert_counter;
// extern unsigned int payment_counter;

#define BUFSIZE 16                /* Static buffer size. Maximum amount of data */
extern unsigned char buffer[BUFSIZE], buffer2[BUFSIZE];    /* No malloc's in embedded code! */      // buffer is for receive, buffer2 is for transmit
extern unsigned char head = 0, tail = 0; /* head for pushing, tail for popping */
extern unsigned char head_2 = 0, tail_2 = 0; /* head for pushing, tail for popping */

extern enum GAME_STATE {IDLE, ACTIVE, END};
extern enum COMMAND {FEED, GO, END, WATER, PLAY, CHECK, SENSOR, ALERT, HASH, PAYMENT};
extern enum INPUT_STATE {START, PARSING, PARSED, FINISHED};
// COMMAND_START: {
// COMMAND_END: }
// GO: {GOXX}
// END: {END}
// FEED: {F}
// WATER: {W}
// PLAY: {P}
// CHECK: {C}
// SENSOR: {SXYZ}
// ALERT: {ASTR}
// HASH: {HCODE}
// PAYMENT: {MXX}

extern enum INPUT_STATE input_state;
extern enum COMMAND command;

enum IS_COMPLETED {false, true};
enum IS_EMPTY {false, true};

unsigned int buf_isempty(unsigned char bufno);
void buf_push(unsigned char v, unsigned char bufno);
unsigned char buf_pop(unsigned char bufno);

/**********************************************************************
 * ----------------------- FUNCTION PROTOTYPES ------------------------
 **********************************************************************/
 /* transmits data using serial communication */
void transmitData();
/* Invoked when receive interrupt occurs; meaning that data is received */
void dataReceived();

void transmitChar();


#endif

/* End of File : common.h */
