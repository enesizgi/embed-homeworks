#include "common.h"
#include "LCD.h"

/**********************************************************************
 * ----------------------- Local Variables ----------------------------
 **********************************************************************/

extern const char string_pool[2][16];
extern char rcvd_chr;

/**********************************************************************
 * ------------------------------ TASK0_LCD -------------------------------
 * 
 * Writes various strings to LCD 
 * 
 **********************************************************************/
TASK(TASK0_LCD)
{
    char a = 'A';

    SetRelAlarm(ALARM_TSK0_LCD, 100, 350);

    WaitEvent(ALARM_EVENT);
    ClearEvent(ALARM_EVENT);

    // ClearLCDScreen();
    // LcdPrintString(&rcvd_chr, 0, 0);
    // LcdPrintString(&a, 0, 1);
    TerminateTask();
}

/* End of File : tsk_task0_LCD.c */