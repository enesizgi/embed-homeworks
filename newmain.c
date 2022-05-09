/*
 * File:   20212_the1.c
 * Author: Uluc Saranli
 *
 * This file is an example C solution for the CENG 336 THE1 within the
 * Spring 2022 semester. It illustrates modular design principles using
 * various tasks and their data elements within a Round-Robin style
 * architecture. Note that this implementation will not necessarily pass
 * the grading scripts as-is, but should be able to do so with minor
 * changes and handling of edge-cases.
 *
 * Created on April 8, 2022, 12:05 PM
 */

#include <xc.h>
#include <stdint.h>
void tmr_isr();
void __interrupt(high_priority) highPriorityISR(void)
{
    if (INTCONbits.TMR0IF)
        tmr_isr();
}
void __interrupt(low_priority) lowPriorityISR(void) {}

// ************* Utility functions ****************
int health;
int level;
int isGameStarted;
int isGameFinished;
int isRC0Pressed;
int tmr1flag = 0;
int ltmrval;
int htmrval;

void init_vars()
{
    health = 9;
    level = 1;
    isGameStarted = 0;
    isGameFinished = 0;
    isRC0Pressed = 0;
}
void init_ports()
{
    TRISA = 0x00; //
    TRISB = 0x00; //
    TRISC = 0x01; // TRIS RC0 will be changed during the game
    TRISD = 0x00; //
    TRISE = 0x00; //
    TRISF = 0x00;
    TRISG = 0x1f; // 0001 1111
    TRISH = 0x00;
    TRISJ = 0x00;

    PORTA = 0x00;
    PORTB = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    PORTF = 0x00;
    PORTH = 0x00;
    PORTJ = 0x00;
}

void init_irq()
{
    INTCON = 0x10;
    INTCONbits.TMR0IE = 1; // MAYBE
    INTCONbits.GIE = 1;
    RCONbits.IPEN = 0;
}

// ************* Timer task and functions ****************
// The "timer" task is responsible from maintaining  counter which,
// once started, counts up to a maximum count, and repeats this "ticks"
// number of times before declaring itself to be "done". The timer can be
// in one of three states: IDLE, RUN, DONE.
typedef enum
{
    TMR_IDLE,
    TMR_RUN,
    TMR_DONE
} tmr_state_t;
tmr_state_t tmr_state = TMR_IDLE; // Current timer state
uint8_t tmr_startreq = 0;         // Flag to request the timer to start
uint8_t tmr_ticks_left;           // Number of "ticks" until "done"

// Preload value for 16bit timer0 1:256 prescaler and 10MHz instruction clock
// to achieve 500ms
// i.e. preload = 65536 - 0.5*10000000/256
#define TIMER0_PRELOAD 46005
//#define TIMER0_PRELOAD 60653

void tmr_isr()
{
    INTCONbits.TMR0IF = 0;  // Reset flag
    if (--tmr_ticks_left == 0)
        tmr_state = TMR_DONE;
}
void tmr_init()
{
    // In order to achieve a 500ms delay, we will use Timer0 in 8-bit mode.
    // This setup assumes a 40MHz 18F8722, which corresponds to a 10MHz
    // instruction cycle
    T0CON = 0x47; // internal clock with 1:256 prescaler and 8-bit
    TMR0 = 0x00; // Initialize TMR0 to 0, without a PRELOAD
    T1CON = 0xc9; // Setting TMR1 to generate random notes
}
// This function resets and starts the timer with the given max counter
// and ticks. The total time waited is ticks*cntmax, after which the timer
// goes into the DONE state
void tmr_start(uint8_t ticks)
{
    tmr_ticks_left = ticks;
    tmr_state = TMR_RUN;
    TMR0 = 0x00;
    INTCONbits.T0IF = 0;
    T0CON |= 0x80; // Set TMR0ON
}
// This function aborts the current timer run and goes back to IDLE
void tmr_abort()
{
    T0CON &= 0x7f; // Unset TMR0ON
    tmr_startreq = 0;
    tmr_state = TMR_IDLE;
}


void randomgen()
{
    int noteval, lastbit, intermbit, num, val, i;
    PORTA = 0x00;

    if (tmr1flag == 0)
    {
        htmrval = TMR1H;
        ltmrval = TMR1L;
        noteval = 0x07 & ltmrval; // Reading Timer1 value
        tmr1flag = 1;
    }
    if (tmr1flag == 1)
    {
        noteval = noteval % 5;
        val = 0x01;
        for (i = 0; i < noteval; i++)
        {
            val = val << 1;
        }
        PORTA = val;
        if (level == 1)
        {
            lastbit = 0x01 & TMR1L;
            intermbit = 0x01 & TMR1H;
            htmrval = htmrval >> 8;
            ltmrval = ltmrval >> 8;
            lastbit = lastbit << 7;
            intermbit = intermbit << 7;
            ltmrval = ltmrval | lastbit;
            htmrval = htmrval | intermbit;
        }
        if (level == 2)
        {
            num = 3;
            while (num > 0)
            {
                lastbit = 0x01 & TMR1L;
                intermbit = 0x01 & TMR1H;
                htmrval = htmrval >> 8;
                ltmrval = ltmrval >> 8;
                lastbit = lastbit << 7;
                intermbit = intermbit << 7;
                ltmrval = ltmrval | lastbit;
                htmrval = htmrval | intermbit;
                num--;
            }
        }
        if (level == 3)
        {
            num = 5;
            while (num > 0)
            {
                lastbit = 0x01 & TMR1L;
                intermbit = 0x01 & TMR1H;
                htmrval = htmrval >> 8;
                ltmrval = ltmrval >> 8;
                lastbit = lastbit << 7;
                intermbit = intermbit << 7;
                ltmrval = ltmrval | lastbit;
                htmrval = htmrval | intermbit;
                num--;
            }
        }
    }
}
// ************* Input task and functions ****************
// The "input task" monitors RA4 and RE4 and increments associated counters
// whenever a high pulse is observed (i.e. HIGH followed by a LOW).
void input_task()
{
    if (!isGameStarted || isGameFinished)
    {
        if (isRC0Pressed)
        {
            if (PORTCbits.RC0 == 0)
            {
                isRC0Pressed = 0;
                isGameStarted = 1;
                isGameFinished = 0;
                TRISC = 0x00;
                PORTC = 0x00;
            }
        }
        else if (PORTCbits.RC0 == 1)
        {
            isRC0Pressed = 1;
        }
    }


}
uint8_t inp_config_cnt = 0; // Current count for CONFIGURE input(i.e. RA4)
uint8_t inp_port_cnt = 0;   // Current count for PORT SELECT input (i.e. RE4)
uint8_t inp_config_btn_st = 0, inp_port_btn_st = 0;

// ************* 7 segment display task and functions ****************
// This is the "display task", which is responsible from maintaining and
// updating outputs on PORTB, PORTC and PORTD. This task handles all
// blinking functionality when configured by using the timer task.
void init_sevseg()
{
    // DO: make the 7seg 9--1
}

// Current expected states of output ports
uint8_t dsp_portb = 0x01, dsp_portc = 0x01, dsp_portd = 0x00;
// Current blink configuration 0: no blink, 1: blink PORTB, 2: blink PORTC
uint8_t dsp_blink = 0;
// Display state for all ports. This is used to implement blinking
// 0: display turned on, 1: display turned off (blink)
uint8_t dsp_off = 0;
// This flag indicates that the actual port outputs should be updated on next
// iteration
uint8_t dsp_updatereq = 1;

// This function sets the current states for "level" (PORTB), action (PORTC)
// and count (PORTD). Level is 0,1,2 or 3, action is 0 (attack) or 1 (defend)
// and 0 <= count < 8 is the countdown start value. Port bit masks are
// computed accordingly here.

// ************* Game task and functions ****************
// This task handles the overall game logic and control remaining tasks
// through their utility functions and flags

// Game state definitions and the global state
typedef enum
{
    G_INIT,
    LEVEL1,
    LEVEL2_INIT,
    LEVEL2,
    LEVEL3_INIT,
    LEVEL3,
    END
} game_state_t;
game_state_t game_state = G_INIT;

uint8_t level_subcount = 0;
uint8_t L1 = 5, L2 = 10, L3 = 15;

void game_task()
{
    switch (game_state)
    {
    case G_INIT:
        // tmr_start(2);  // MAYBE
        init_sevseg();
        tmr_start(77); // TMR0 counts 77 times so that 500 ms
        game_state = LEVEL1;
        //shift_task();   // Shift RA->RB, RB-RC, ... , RE->RF
        randomgen();    // generate note    
        ++level_subcount;    
        break;
    case LEVEL1:
        // START state
        
        if (tmr_state == TMR_DONE) // 500 ms passed
        {
            if(level_subcount < L1)
            {
                shift_task();   // Shift RA->RB, RB-RC, ... , RE->RF
                randomgen();    // generate note
            }
            if(level_subcount >= L1)
            {
                shift_task();   // Shift RA->RB, RB-RC, ... , RE->RF
                //if(level_subcount == L1) PORTA = 0x00;
            }
            ++level_subcount;
            if(level_subcount == 6 + L1)  // 5 is the A B C D E F PORST count  i.e., level_subcount == 11
            {
                game_state = LEVEL2_INIT;
            }
            tmr_start(77);  // TMR0 counts 77 times so that 500 ms
        }
        break;
    case LEVEL2_INIT:
        level_subcount = 0;
        tmr_start(XXX); // TMR0 counts 77 times so that 500 ms
        game_state = LEVEL2;
        //shift_task();   // Shift RA->RB, RB-RC, ... , RE->RF
        randomgen();    // generate note    
        ++level_subcount;    
        break;
    case LEVEL2:
        if (tmr_state == TMR_DONE) // 500 ms passed
        {
            if(level_subcount < L2)
            {
                shift_task();   // Shift RA->RB, RB-RC, ... , RE->RF
                randomgen();    // generate note
            }
            if(level_subcount >= L2)
            {
                shift_task();   // Shift RA->RB, RB-RC, ... , RE->RF
                //if(level_subcount == L1) PORTA = 0x00;
            }
            ++level_subcount;
            if(level_subcount == 6 + L2)  // 5 is the A B C D E F PORST count  i.e., level_subcount == 11
            {
                game_state = LEVEL2;
            }
            tmr_start(XXX);  // TMR0 counts 77 times so that 500 ms
        }
        break;
    case LEVEL3_INIT:
        level_subcount = 0;
        tmr_start(XXX); // TMR0 counts 77 times so that 500 ms
        game_state = LEVEL3;
        //shift_task();   // Shift RA->RB, RB-RC, ... , RE->RF
        randomgen();    // generate note    
        ++level_subcount;    
        break;
    case LEVEL3:
        if (tmr_state == TMR_DONE) // 500 ms passed
        {
            if(level_subcount < L3)
            {
                shift_task();   // Shift RA->RB, RB-RC, ... , RE->RF
                randomgen();    // generate note
            }
            if(level_subcount >= L3)
            {
                shift_task();   // Shift RA->RB, RB-RC, ... , RE->RF
                //if(level_subcount == L1) PORTA = 0x00;
            }
            ++level_subcount;
            if(level_subcount == 6 + L3)  // 5 is the A B C D E F PORST count  i.e., level_subcount == 11
            {
                game_state = END;    // Oyun biter.  TO BE DONE
            }
            tmr_start(XXX);  // TMR0 counts 77 times so that 500 ms
        }
        break;
        case END:
            // make 7seg display End
            break;
    }
}

void shape_shifter()
{
    PORTF = PORTE;
    PORTE = PORTD;
    PORTD = PORTC;
    PORTC = PORTB;
    PORTB = PORTA;
    PORTA = 0;
}
// Current game choices and the countdown
uint8_t game_level = 1, game_action = 0, game_count = 0;

void main(void)
{
    init_vars();  // DONE
    init_ports(); // DONE
    tmr_init();   // DONE
    init_irq();   // DONE
    while (1)
    {
        // check here if rc0 pressed
        //        T0CONbits.TMR0ON = 0x01;
        input_task();
        if ((isGameStarted == 0) || (isGameFinished == 1))
        {
            continue;
        }
        //  timer_task();
        // sevenseg_task();
        // game_task();
    }
}