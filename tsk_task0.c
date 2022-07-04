#include "common.h"
#include "LCD.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern char rcvd_chr;

/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/

char announcement[30] = "Start your homework EARLY!:)\n";

TASK(TASK0) 
{
    char a = 'A';
    ClearLCDScreen();
    LcdPrintString(&rcvd_chr, 0, 0);
    LcdPrintString(&a, 0, 1);
	SetRelAlarm(ALARM_TSK0, 100, 5000);
	while(1) {
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        transmitCharAndHello('!'); //sends hello!
	}
	TerminateTask();
}


/* End of File : tsk_task0.c */