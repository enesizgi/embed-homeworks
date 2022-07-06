#include "common.h"
#define MAX_PARAM_LEN 19

extern unsigned char alert_params[9];
extern enum GAME_STATE game_state;
extern unsigned int happy, hunger, thirst, money;

extern void compute_hash(unsigned char *inp, unsigned char *out); // MAYBE add this function in another way



//// TRANSMISSION TASK ////
TASK(TASK2)
{
    // SetRelAlarm(ALARM_TSK1, 100, 50);
    // while (1)
    // {

    //     WaitEvent(TASK2_EVENT);
    //     ClearEvent(TASK2_EVENT);
    //     check();

    //     easy();

    //     SetEvent(TASK1_ID, ALARM_EVENT);
    // }

    TerminateTask();
}

/* End of File : tsk_task0.c */