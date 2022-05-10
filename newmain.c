/*
 * File:   20212_the1.c
 * Author: Afsar Saranli
 */

#include <xc.h>
#include <stdint.h>
// CONFIG
#pragma config OSC = HSPLL
#pragma config WDT = OFF

void tmr_isr();
void __interrupt(high_priority) highPriorityISR(void)
{
    if (INTCONbits.TMR0IF)
        tmr_isr();
}
void __interrupt(low_priority) lowPriorityISR(void) {}

// ************* Utility functions ****************
uint8_t health;
uint8_t level;
uint8_t isGameStarted;
uint8_t isGameFinished;
uint8_t isRC0Pressed;
uint8_t isRG0Pressed;
uint8_t isRG1Pressed;
uint8_t isRG2Pressed;
uint8_t isRG3Pressed;
uint8_t isRG4Pressed;
uint8_t whichRG;
uint8_t tmr1flag = 0;
uint8_t ltmrval;
uint8_t htmrval;

void init_vars()
{
    health = 9;
    level = 1;
    isGameStarted = 0;
    isGameFinished = 0;
    isRC0Pressed = 0;
    isRG0Pressed = -1;
    isRG1Pressed = -1;
    isRG2Pressed = -1;
    isRG3Pressed = -1;
    isRG4Pressed = -1;
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
// in one of three states: RUN, DONE.
typedef enum
{
    TMR_IDLE,
    TMR_RUN,
    TMR_DONE
} tmr_state_t;
tmr_state_t tmr_state = TMR_IDLE; // Current timer state
uint8_t tmr_startreq = 0;         // Flag to request the timer to start
uint8_t tmr_ticks_left;           // Number of "ticks" until "done"


void tmr_isr()
{
    INTCONbits.TMR0IF = 0; // Reset flag
    if (--tmr_ticks_left == 0)
        tmr_state = TMR_DONE;
    
}
void tmr_init()
{
    // In order to achieve a 500ms delay, we will use Timer0 in 8-bit mode.
    // This setup assumes a 40MHz 18F8722, which corresponds to a 10MHz
    // instruction cycle
    T0CON = 0x47; // internal clock with 1:256 prescaler and 8-bit
    TMR0 = 0x00;  // Initialize TMR0 to 0, without a PRELOAD
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
    uint8_t noteval, lastbit, intermbit, num, val, i;
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
// The "input task" monitors RC0 and RG0-4 and sets associated flags

void input_task()
{
    ///////////////////////////////////////////////////RC0 TASK///////////////////////////////////////////////////

    // DONT FORGET TO SET THE VARIABLES -1 AGAIN IN GAME TASK  // Setted to 0 MAYBE -1 ?
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
            game_state = G_INIT;
        }
    }
    ///////////////////////////////////////////////////RG TASK///////////////////////////////////////////////////
    if (isRG0Pressed == 0)
    {
        if (PORTGbits.RG0 == 0)
        {
            isRG0Pressed = 1;
            whichRG = 0;
        }
    }
    else if (PORTGbits.RG0 == 1)
    {
        isRG0Pressed = 0;
    }

    if (isRG1Pressed == 0)
    {
        if (PORTGbits.RG1 == 0)
        {
            isRG1Pressed = 1;
            whichRG = 1;
        }
    }
    else if (PORTGbits.RG1 == 1)
    {
        isRG1Pressed = 0;
    }

    if (isRG2Pressed == 0)
    {
        if (PORTGbits.RG2 == 0)
        {
            isRG2Pressed = 1;
            whichRG = 2;
        }
    }
    else if (PORTGbits.RG2 == 1)
    {
        isRG2Pressed = 0;
    }

    if (isRG3Pressed == 0)
    {
        if (PORTGbits.RG3 == 0)
        {
            isRG3Pressed = 1;
            whichRG = 3;
        }
    }
    else if (PORTGbits.RG3 == 1)
    {
        isRG3Pressed = 0;
    }

    if (isRG4Pressed == 0)
    {
        if (PORTGbits.RG4 == 0)
        {
            isRG4Pressed = 1;
            whichRG = 4;
        }
    }
    else if (PORTGbits.RG4 == 1)
    {
        isRG4Pressed = 0;
    }
}

// ************* 7 segment display task and functions ****************
// This is the "display task", which is responsible from maintaining and
// updating outputs on PORTB, PORTC and PORTD. This task handles all
// blinking functionality when configured by using the timer task.


void sevenSeg_controller()
{
    switch (game_state)
    {
    case INIT:
        sevenSeg(health, 0);
        sevenSeg(game_level, 3);
        break;
    case LEVEL1:
        sevenSeg(health, 0);
        sevenSeg(game_level, 3);
        break;

    case LEVEL2_INIT:
        sevenSeg(health, 0);
        sevenSeg(game_level, 3);
        break;
    case LEVEL2:
        sevenSeg(health, 0);
        sevenSeg(game_level, 3);
        break;

    case LEVEL3_INIT:
        sevenSeg(health, 0);
        sevenSeg(game_level, 3);
        break;
    case LEVEL3:
        sevenSeg(health, 0);
        sevenSeg(game_level, 3);
        break;

    case END:  // MAYBE rc0 a kadar bunun yanmasi lazim
        sevenSeg(11, 0);    // E
        sevenSeg(12, 1);    // n
        sevenSeg(13,2);     // d
        break;
    case LOSE:
        sevenSeg(10, 0);    // L
        sevenSeg(0, 1);     // O
        sevenSeg(5,2);      // S
        sevenSeg(11, 0);    // E
        break;
    }
}

void sevenSeg(int J, char D)
{
    switch(J){  
        
        // All dps are reset (i.e., bit7 -> 0)
        case 0: // Also case O
            PORTJ = 0x3f;  // abcdef    -> 1111 1100
            break;
        case 1:
            PORTJ = 0x3d; // bc         -> 0110 0000
            break;
        case 2:
            PORTJ = 0x5b;
            break;
        case 3:
            PORTJ = 0x4f;
            break;
        case 4: 
            PORTJ = 0x66;  
            break;
        case 5:
            PORTJ = 0x6d; 
            break;
        case 6:
            PORTJ = 0x7d;
            break;
        case 7:
            PORTJ = 0x07;
            break;
        case 8: 
            PORTJ = 0x7f; 
            break;
        case 9:
            PORTJ = 0x6f; 
            break;
        case 10:    // L
            PORTJ = 0xd8;
            break;
        case 11:    // E
            PORTJ = 0x79;
            break;
        case 12:    // n 
            PORTJ = 0x54; 
            break;
        case 13:    // d
            PORTJ = 0x5e; 
            break;
    }
    switch(D){
        case 0:
            PORTH = 0x01;       // RH0 = 1, others = 0
            break;
        case 1:
            PORTH = 0x02;       // RH1 = 1, others = 0
            break;
        case 2:
            PORTH = 0x04;       // RH2 = 1, others = 0
            break;
        case 3:
            PORTH = 0x08;       // RH3 = 1, others = 0
            break;
    }
}

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
    END,
    LOSE
} game_state_t;
game_state_t game_state;

uint8_t level_subcount = 0;
uint8_t L1 = 5, L2 = 10, L3 = 15;

void shape_shifter()
{
    PORTF = PORTE;
    PORTE = PORTD;
    PORTD = PORTC;
    PORTC = PORTB;
    PORTB = PORTA;
    PORTA = 0x00;
}

void reset_task()
{
    isGameFinished = 1;
    isGameStarted = 0;
    tmr1flag = 0;
}

void health_decreaser()
{
    health--;
    if (health == 0)
    {
        game_state = LOSE;
    }
}

void game_task()
{
    // switch case RF == RG
    // tmr_state = TMR_DONE;
    // Check if the player presses more than one buttons or does not press at all.
    // MAYBE don't sure about not pressing a button, is it to be punished here or by means of counter ???
    // Now it does NOT checks for not pressing, if you want to check it go to proper commit.
    uint8_t count = 0;
    if (isRG0Pressed)
        count++;
    if (isRG1Pressed)
        count++;
    if (isRG2Pressed)
        count++;
    if (isRG3Pressed)
        count++;
    if (isRG4Pressed)
        count++;

    if (count > 1)
        whichRG = -1;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    switch (whichRG)
    {
    case -1:
        health_decreaser();
        break;
    case 0:
        if (PORTFbits.RF0 == 1)
            tmr_state = TMR_DONE; // MAYBE shift fast or just delete RF
        else
            health_decreaser();
        break;
    case 1:
        if (PORTFbits.RF1 == 1)
            tmr_state = TMR_DONE;
        else
            health_decreaser();
        break;
    case 2:
        if (PORTFbits.RF2 == 1)
            tmr_state = TMR_DONE;
        else
            health_decreaser();
        break;
    case 3:
        if (PORTFbits.RF3 == 1)
            tmr_state = TMR_DONE;
        else
            health_decreaser();
        break;
    case 4:
        if (PORTFbits.RF4 == 1)
            tmr_state = TMR_DONE;
        else
            health_decreaser();
        break;
    }
    switch (game_state)
    {
    case G_INIT:
        init_sevseg();
        tmr_start(77); // TMR0 counts 77 times so that 500 ms
        game_state = LEVEL1;
        // shape_shifter();   // Shift RA->RB, RB-RC, ... , RE->RF
        randomgen(); // generate note
        ++level_subcount;
        game_level = 1;
        break;
    case LEVEL1:
        // START state

        if (tmr_state == TMR_DONE) // 500 ms passed
        {
            if (level_subcount < L1)
            {
                shape_shifter(); // Shift RA->RB, RB-RC, ... , RE->RF
                randomgen();     // generate note
            }
            if (level_subcount >= L1)
            {
                shape_shifter(); // Shift RA->RB, RB-RC, ... , RE->RF
                // if(level_subcount == L1) PORTA = 0x00;
            }
            ++level_subcount;
            if (level_subcount == 6 + L1) // 5 is the A B C D E F PORST count  i.e., level_subcount == 11
            {
                game_state = LEVEL2_INIT;
            }
            tmr_start(77); // TMR0 counts 77 times so that 500 ms
        }
        break;
    case LEVEL2_INIT:
        level_subcount = 0;
        tmr_start(61); // TMR0 counts 77 times so that 500 ms
        game_state = LEVEL2;
        // shape_shifter();   // Shift RA->RB, RB-RC, ... , RE->RF
        randomgen(); // generate note
        ++level_subcount;
        game_level = 2;
        break;
    case LEVEL2:
        if (tmr_state == TMR_DONE) // 400 ms passed
        {
            if (level_subcount < L2)
            {
                shape_shifter(); // Shift RA->RB, RB-RC, ... , RE->RF
                randomgen();     // generate note
            }
            if (level_subcount >= L2)
            {
                shape_shifter(); // Shift RA->RB, RB-RC, ... , RE->RF
                // if(level_subcount == L1) PORTA = 0x00;
            }
            ++level_subcount;
            if (level_subcount == 6 + L2) // 5 is the A B C D E F PORST count  i.e., level_subcount == 11
            {
                game_state = LEVEL2;
            }
            tmr_start(61); // TMR0 counts 61 times so that 400 ms
        }
        break;
    case LEVEL3_INIT:
        level_subcount = 0;
        tmr_start(46); // TMR0 counts 46 times so that 300 ms
        game_state = LEVEL3;
        // shape_shifter();   // Shift RA->RB, RB-RC, ... , RE->RF
        randomgen(); // generate note
        ++level_subcount;
        game_level = 3;
        break;
    case LEVEL3:
        if (tmr_state == TMR_DONE) // 300 ms passed
        {
            if (level_subcount < L3)
            {
                shape_shifter(); // Shift RA->RB, RB-RC, ... , RE->RF
                randomgen();     // generate note
            }
            if (level_subcount >= L3)
            {
                shape_shifter(); // Shift RA->RB, RB-RC, ... , RE->RF
                // if(level_subcount == L1) PORTA = 0x00;
            }
            ++level_subcount;
            if (level_subcount == 6 + L3) // 5 is the A B C D E F PORST count  i.e., level_subcount == 11
            {
                game_state = END; // Oyun biter.  TO BE DONE
            }
            tmr_start(46); // TMR0 counts 77 times so that 500 ms
        }
        break;
    case LOSE:
        reset_task();
        //make seven segment display lose
        break;
    case END:
        reset_task();
        // make 7seg display End
        break;
    }
}

// Current game choices and the countdown
uint8_t game_level = 1;
void main(void)
{
    init_vars();  // DONE
    init_ports(); // DONE
    tmr_init();   // DONE
    init_irq();   // DONE
    while (1)
    {
        // TODO: 7seg time-based things
        // TODO: 7seg task
        input_task();
        sevenSeg_controller();
        if ((isGameStarted == 0) || (isGameFinished == 1))
        {
            continue;
        }
        // game_task();
    }
}