#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern char rcvd_chr;
extern unsigned char buf_pop(unsigned char bufno);
extern void check();

unsigned int happy, hunger, thirst, money;

unsigned int go_counter = 0;
unsigned int payment_counter = 0;
unsigned int sensor_counter = 0;
unsigned int alert_counter = 0;

unsigned char go_params[2];
unsigned char sensor_params[3];
unsigned char alert_params[9];    // 8 + 1 (8 char 1 null character)
unsigned char payment_params[2];

enum INPUT_STATE input_state;
enum COMMAND command;
enum GAME_STATE game_state;
/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/
void easy()
{
    ////////////////Hungerness
    if (hunger <= 40 && money >= 80)
    {
        //  Feed  //
        buf_push('{', 2);
        buf_push('F', 2);
        buf_push('}', 2);
//        buf_push('a', 2);

        // transmitChar(3);
        money -= 80;
    }
    ////////////////Thirstyness
    else if (thirst <= 70 && money >= 30)
    {
        // Water  //
        buf_push('{', 2);
        buf_push('W', 2);
        buf_push('}', 2);
//        buf_push('a', 2);

        // transmitChar(3);
        money -= 30;
    }

    ////////////////Happiness
    else if (happy <= 40 && money >= 150)
    {
        //  Play  //
        buf_push('{', 2);
        buf_push('P', 2);
        buf_push('}', 2);
//        buf_push('a', 2);

        // transmitChar(3);
        money -= 80;
    }
}

void check()
{
  // Send check command here
  buf_push('{', 2);
  buf_push('C', 2);
  buf_push('}', 2);
//  buf_push('a', 2);
  // transmitChar(4);   // After this transmission, the simulator will send Sensor Response command
//  SetEvent(TASK2_ID, TASK2_EVENT);
}

void do_stuff()
{
  // Storing the command parameters
  switch (command)
  {
  case GO:    // Store money by shift and sum
    /* code */
    money = (((int)go_params[0]) << 8) + ((int)go_params[1]);
    break;
  case SENSOR:    // Store Hunger Happy Thirst
    hunger = sensor_params[0];
    happy = sensor_params[1];
    thirst = sensor_params[2];
   SetEvent(TASK0_ID, TASK0_EVENT);
    break;
  case PAYMENT:   // Store payment, i.e., add payment to money
    money += (((int)payment_params[0]) << 8) + ((int)payment_params[1]);
    break;
  default:
    break;
  }
}

void parse_parameter(unsigned char chr)
{
  // Parse the parameters which are shown as XX in the homework pdf
  switch (command)
  {
  case GO:
    go_params[go_counter++] = chr;
    if (go_counter == 2)
      go_counter = 0;
    break;
  case SENSOR:
    sensor_params[sensor_counter++] = chr;
    if (sensor_counter == 3)
      sensor_counter = 0;
    break;
  case ALERT:
    alert_params[alert_counter++] = chr;
    if (alert_counter == 8)
    {
      alert_params[alert_counter] = '\0';   // Make the last element null character in order to send HASH command
      alert_counter = 0;
    }
    break;
  case PAYMENT:
    payment_params[payment_counter++] = chr;
    if (payment_counter == 2)
      payment_counter = 0;
    break;
  default:
    break;
  }
}

void parse_command(unsigned char chr)
{
  // Identifies which command is received from the simulator, and then does some stuff.
  // All storing stuff is held in do_stuff() routine.
  switch (chr)
  {
  case 'O':   // Go command is received, store the money
    game_state = ACTIVE;
    command = GO;
    input_state = PARSING;
    break;
  case 'D':   // End command is received, terminate the program
    game_state = END;
    break;
  case 'S':   // Sensor response is received, store Hunger Happy Thirst
    command = SENSOR;
    input_state = PARSING;
    break;
  case 'A':   // Alert command is received, store the string to be hashed
    command = ALERT;
    input_state = PARSING;
    break;
  case 'M':   // Payment is received, store the payment
    command = PAYMENT;
    input_state = PARSING;
  default:
    break;
  }
}


//// RECEIVE TASK ////
TASK(TASK1)
{
  input_state = START;
  SetRelAlarm(ALARM_TSK1, 100, 500);
  while (1)
  {
    if (game_state == END)
      break;
    WaitEvent(ALARM_EVENT);
    ClearEvent(ALARM_EVENT);
    if (buf_isempty(1) == 0)
    {
      unsigned char chr = buf_pop(1);

      if (chr == '{')
        input_state = START;
      else if (chr == '}')
        input_state = PARSED;

      switch (input_state)
      {
      case START:
        parse_command(chr);
        break;
      case PARSING:
        parse_parameter(chr);
        break;
      case PARSED:
        do_stuff();
        input_state = FINISHED;
        break;
      default:
        break;
      }
    }
  }
  TerminateTask();
}

/* End of File : tsk_task1.c */