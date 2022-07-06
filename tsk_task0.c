#include "common.h"
#define MAX_PARAM_LEN 19

extern unsigned char alert_params[9];
extern enum GAME_STATE game_state;
extern void easy();
extern void compute_hash(unsigned char *inp, unsigned char *out);   // MAYBE add this function in another way
/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/

/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/

void make_cat_live()
{
}

void get_hash()
{
/*  unsigned char *out;
  compute_hash(alert_params, out);
  str_to_send[0] = '{';
  str_to_send[1] = 'H';

  for(int i = 2; i<18; i++)  str_to_send[i] = out[i-2];
  str_to_send[18] = '}';   
  transmitChar(19);
  */
}

//// TRANSMISSION TASK ////
TASK(TASK0)
{
 SetRelAlarm(ALARM_TSK0, 100, 200);
//  while (1)
//  {
//    if (game_state == END)
//      break;
//    WaitEvent(ALARM_EVENT);
//    ClearEvent(ALARM_EVENT);
//    //check();
//   // make_cat_live();
//    if(command == ALERT)    // If the ALERT command has received, then calculate hash ASAP
//    {
//      get_hash();
//    }
//  }
    while (1)
    {
//        WaitEvent(ALARM_EVENT);
//        ClearEvent(ALARM_EVENT);
        // if (game_state == ACTIVE) {
        //     check();
        // }
        while (buf_isempty(2) == 0)
        {
          transmitChar();
        }
        
        SetEvent(TASK1_ID, ALARM_EVENT);
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
//        easy();

        
    }

  TerminateTask();
}

/* End of File : tsk_task0.c */