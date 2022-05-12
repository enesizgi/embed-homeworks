/*-*
 *-* File:   20212_the2.c
 *-* GROUP 32
 *-* Author: Ali KOMURCU              2380699
 *-* Author: Muhammed Enes IZGI       2310142
 *-* Author: Kerem Can BAKIR          2380152
 *-* Author: Emre Can KOPARAL         2380673

*-* u --> micro
*-* m --> mili

*-* TIMER0 Calculations:
    *-* Oscillator frequency is 40 MHz, so that instruction frequency is 10 MHz. (Fosc = 4*Fins)
    *-* That means Tins = 1/Fins --> Tins = 0.1 us
    *-* We used 1:256 prescaler.
    *-*
    *-* FOR LEVEL1
    *-* We need to count 500 ms so that 500000 us.
    *-* So that we need to count 500000 cycles for 500 ms.
    *-* From start to interrupt we count 2^8 * 256 * 0.1 = 6553.6 cycles.
    *-* 500000/6553.6 = 76.29 --> We need to use TMR0 76.29 (~ 77) times to achieve 500 ms. 
    *-* 
    *-* FOR LEVEL2
    *-* We need to count 400 ms so that 400000 us.
    *-* So that we need to count 400000 cycles for 400 ms.
    *-* From start to interrupt we count 2^8 * 256 * 0.1 = 6553.6 cycles.
    *-* 400000/6553.6 = 61.03 --> We need to use TMR0 61.03 (~ 61) times to achieve 400 ms. 
    *-* 
    *-* FOR LEVEL3
    *-* We need to count 300 ms so that 300000 us.
    *-* So that we need to count 300000 cycles for 300 ms.
    *-* From start to interrupt we count 2^8 * 256 * 0.1 = 6553.6 cycles.
    *-* 300000/6553.6 = 45.77 --> We need to use TMR0 45.77 (~ 46) times to achieve 300 ms.

 *-*/

#include <xc.h>
#include <stdint.h>
// CONFIG Oscillator and TURN OFF Watchdog timer
#pragma config OSC = HSPLL
#pragma config WDT = OFF

void tmr_isr();
uint8_t sevenSeg2WayCounter;
uint8_t sevenSeg3WayCounter;
uint8_t sevenSeg4WayCounter;
void __interrupt(high_priority) highPriorityISR(void)
{
    /*-*
    *-* We used high priority interrupts for TMR0.
    *-*/
    if (INTCONbits.TMR0IF)
    {
        tmr_isr();
    }
}
void __interrupt(low_priority) lowPriorityISR(void) {}

// ************* Utility functions ****************
uint8_t health;
uint8_t level;
uint8_t isGameStarted;      // Set when game starts, reset when game ends
uint8_t isGameFinished;     // Set when game ends, reset when game starts
int8_t isRG0Pressed;        // Set if RG0 pressed
int8_t isRG1Pressed;        // Set if RG1 pressed
int8_t isRG2Pressed;        // Set if RG2 pressed
int8_t isRG3Pressed;        // Set if RG3 pressed
int8_t isRG4Pressed;        // Set if RG4 pressed
int8_t isTruePressed;       // Set if proper RG button pressed for RF, o.w reset
int8_t starterDelay;        // Health begins to decrease after when the lights on the LEDs arrives RF, so we need wait to decrease health. This variable is used for this purpose on game_task
uint8_t whichRG;            // When a PORTG button is pressed, this flag is set to that buttons number. For example if RG0 is pressed -> whichRG = 0,..., if RG4 is pressed -> whichRG = 4
uint8_t tmr1flag;           // Used for reading TMR1 once
uint8_t ltmrval;            // Used for TMR1 read low
uint8_t htmrval;            // Used for TMR1 read high

// Game state definitions and the global state
typedef enum
{
    INIT_START,
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

uint8_t level_subcount;     // Used for shifting purposes in game_task
uint8_t L1 = 5, L2 = 10, L3 = 15;

// Current game choices and the countdown
uint8_t game_level;

void init_vars()
{
    // This function initializes variables and it is called when a new game starts
    health = 9;
    level = 1;
    isGameStarted = 0;
    isGameFinished = 0;
    isTruePressed = 0;
    isRG0Pressed = 2;       // This flag is tri-state flag 2 - 1 - 0
    isRG1Pressed = 2;       // This flag is tri-state flag 2 - 1 - 0
    isRG2Pressed = 2;       // This flag is tri-state flag 2 - 1 - 0
    isRG3Pressed = 2;       // This flag is tri-state flag 2 - 1 - 0
    isRG4Pressed = 2;       // This flag is tri-state flag 2 - 1 - 0
    sevenSeg2WayCounter = 0;
    sevenSeg3WayCounter = 0;
    sevenSeg4WayCounter = 0;
    whichRG = 5;            // Initialized to 5 which is not in range 0-4
    starterDelay = 0;
    level_subcount = 0;
    game_level = 1;
    game_state = G_INIT;
    tmr1flag = 0;
}



void init_ports()
{
    ADCON1 = 0x0f;      // Configure ADCON1 to use all pins as digital
    TRISA = 0x00;       // Output
    TRISB = 0x00;       // Output
    TRISC = 0x01;       // TRIS RC0 will be changed during the game
    TRISD = 0x00;       // Output
    TRISE = 0x00;       // Output
    TRISF = 0x00;       // Output
    TRISG = 0x1f;       // Inputs -> 0001 1111
    TRISH = 0x00;       // Output
    TRISJ = 0x00;       // Output

    // RESET PORTS
    PORTA = 0x00;
    PORTB = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    PORTF = 0x00;
    PORTH = 0x00;
    PORTJ = 0x00;
    
    T1CON = 0xc1; // Setting TMR1 to generate random notes
}

void init_irq()
{
    // INTCON is configured to use TMR0 interrupts
    INTCON = 0xa0;
}

// ************* Timer task and functions ****************
// The "timer" task is responsible from maintaining  counter which,
// once started, counts up to a maximum count, and repeats this "ticks"
// number of times before declaring itself to be "done". The timer can be
// in one of three states: RUN, DONE.
typedef enum
{
    TMR_RUN,
    TMR_DONE
} tmr_state_t;
tmr_state_t tmr_state = TMR_RUN; // Current timer state
uint8_t tmr_ticks_left;          // Number of "ticks" until "done"

void tmr_isr()
{
    if (sevenSeg2WayCounter == 1)
        sevenSeg2WayCounter = 0;
    else
        sevenSeg2WayCounter = 1;
    if (sevenSeg3WayCounter == 2)
        sevenSeg3WayCounter = 0;
    else
        sevenSeg3WayCounter++;
    if (sevenSeg4WayCounter == 3)
        sevenSeg4WayCounter = 0;
    else
        sevenSeg4WayCounter++;
    INTCONbits.TMR0IF = 0; // Reset flag
    if (--tmr_ticks_left == 0)
        tmr_state = TMR_DONE;
}
void tmr_init()
{
    // In order to achieve a 500ms-400ms-300ms delay, we will use Timer0 in 8-bit mode.
    // This setup assumes a 40MHz 18F8722, which corresponds to a 10MHz
    // instruction cycle
    T0CON = 0x47; // internal clock with 1:256 prescaler and 8-bit
    TMR0 = 0x00;  // Initialize TMR0 to 0, without a PRELOAD
    T1CON = 0xc1; // Setting TMR1 to generate random notes
}
// This function resets and starts the timer with the given max counter
// and ticks. The total time waited is ticks*cntmax, after which the timer
// goes into the DONE state
void tmr_start(uint8_t ticks)
{
    tmr_ticks_left = ticks;     
    tmr_state = TMR_RUN;        // Set timer state to RUN
    TMR0 = 0x00;                // Clear TMR0
    INTCONbits.TMR0IF = 0;      // Reset TMR0 Interrupt Flag
}

void randomgen()
{
    uint8_t noteval, lastbit, intermbit, num, val, i; // 

    if (tmr1flag == 0) // Reading TMR1 value 
    {
        ltmrval = TMR1L; // Saving last 8 bits of TMR1
        htmrval = TMR1H; // Saving first 8 bits of 
        noteval = 0x07 & ltmrval; // Reading last 3 bits of Timer1 value
        tmr1flag = 1; // To read TMR1 value once I use a flag 
        
    }
    if (tmr1flag == 1)
    {
        noteval = 0x07 & ltmrval;
        switch (noteval) { // Getting the remainder from division by 5
            case 5:
                noteval = 0;
                break;
            case 6:
                noteval = 1;
                break;
            case 7:
                noteval = 2;
                break;
            default:
                break;
        }
        val = 0x01;
        val = val << noteval; // Converting the remainder to choose correct RC led
        PORTA = 0x00;
        PORTA = val; // Lighting up the corresponding RC led
        if (level == 1) // Rotation for level 1
        {
            lastbit = 0x01 & ltmrval; //  Saving the value of the last bit of last 8 bit
            intermbit = 0x01 & htmrval; // Saving the value of the last bit of first 8 bit
            htmrval = htmrval >> 1; // Making shift but first bit is set to 0 automatically 
            ltmrval = ltmrval >> 1;
            lastbit = lastbit << 7; // Shifting the rightmost bit to the leftmost position
            intermbit = intermbit << 7;
            ltmrval = ltmrval | lastbit; // Using OR operation to merge leftmost bit and the remaining bits
            htmrval = htmrval | intermbit;
        }
        if (level == 2)
        {
            num = 3; // Rotating 3 times for level 2 
            while (num > 0)
            {
                lastbit = 0x01 & ltmrval;
                intermbit = 0x01 & htmrval;
                htmrval = htmrval >> 1;
                ltmrval = ltmrval >> 1;
                lastbit = lastbit << 7;
                intermbit = intermbit << 7;
                ltmrval = ltmrval | lastbit;
                htmrval = htmrval | intermbit;
                num--;
            }
        }
        if (level == 3)
        {
            num = 5; // Rotating 5 times for level 3
            while (num > 0)
            {
                lastbit = 0x01 & ltmrval;
                intermbit = 0x01 & htmrval;
                htmrval = htmrval >> 1;
                ltmrval = ltmrval >> 1;
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
    /*------------------ RC0 TASK ------------------*/
    // Below code checks if RC0 being pressed
    // We use game_state G_INIT at the beginning
    if (!isGameStarted || isGameFinished)
    {
        if (PORTCbits.RC0 == 1)
        {
            health = 9; // MAYBE Disable?
            game_state = G_INIT; // MAYBE Disable?
            init_vars();
            init_ports();
            isGameStarted = 1;
            isGameFinished = 0;
            T1CON = 0xc9;
            TRISC = 0x00;
            T1CONbits.T1OSCEN = 0;
            PORTC = 0x00;
            T0CON |= 0x80; // Set TMR0ON
        }
    }
    /*------------------ RC0 TASK ------------------*/
    // CHECK IF RG0 PRESSED
    if (isRG0Pressed == 0)
    {
        if (PORTGbits.RG0 == 0)
        {
            // Button was pressed and now it is released.
            // We are setting our flag variables.
            isRG0Pressed = 1;
            whichRG = 0;
        }
    }
    else if (PORTGbits.RG0 == 1) 
    {
        // Button is pressed but not released yet.
        isRG0Pressed = 0;
    }
    // CHECK IF RG0 PRESSED

    // CHECK IF RG1 PRESSED
    if (isRG1Pressed == 0)
    {
        if (PORTGbits.RG1 == 0)
        {
            // Button was pressed and now it is released.
            // We are setting our flag variables.
            isRG1Pressed = 1;
            whichRG = 1;
        }
    }
    else if (PORTGbits.RG1 == 1)
    {
        // Button is pressed but not released yet.
        isRG1Pressed = 0;
    }
    // CHECK IF RG1 PRESSED

    // CHECK IF RG2 PRESSED
    if (isRG2Pressed == 0)
    {
        if (PORTGbits.RG2 == 0)
        {
            // Button was pressed and now it is released.
            // We are setting our flag variables.
            isRG2Pressed = 1;
            whichRG = 2;
        }
    }
    else if (PORTGbits.RG2 == 1)
    {
        // Button is pressed but not released yet.
        isRG2Pressed = 0;
    }
    // CHECK IF RG2 PRESSED

    // CHECK IF RG3 PRESSED
    if (isRG3Pressed == 0)
    {
        if (PORTGbits.RG3 == 0)
        {
            // Button was pressed and now it is released.
            // We are setting our flag variables.
            isRG3Pressed = 1;
            whichRG = 3;
        }
    }
    else if (PORTGbits.RG3 == 1)
    {
        // Button is pressed but not released yet.
        isRG3Pressed = 0;
    }
    // CHECK IF RG3 PRESSED

    // CHECK IF RG4 PRESSED
    if (isRG4Pressed == 0)
    {
        if (PORTGbits.RG4 == 0)
        {
            // Button was pressed and now it is released.
            // We are setting our flag variables.
            isRG4Pressed = 1;
            whichRG = 4;
        }
    }
    else if (PORTGbits.RG4 == 1)
    {
        // Button is pressed but not released yet.
        isRG4Pressed = 0;
    }
    // CHECK IF RG4 PRESSED
}

// ************* 7 segment display task and functions ****************
// This is the "7 segment display task", which is responsible from maintaining and
// updating outputs on PORTJ, PORTH. This task handles all
// 7 segment-related purposes.
void sevenSeg(uint8_t J, uint8_t D);
void sevenSeg_controller()
{
    switch (game_state)
    {
    case G_INIT:
        if (sevenSeg2WayCounter == 0)
            sevenSeg(health, 0);
        else
            sevenSeg(game_level, 3);
        break;
    case LEVEL1:
        if (sevenSeg2WayCounter == 0)
            sevenSeg(health, 0);
        else
            sevenSeg(game_level, 3);
        break;
    case LEVEL2_INIT:
        if (sevenSeg2WayCounter == 0)
            sevenSeg(health, 0);
        else
            sevenSeg(game_level, 3);
        break;
    case LEVEL2:
        if (sevenSeg2WayCounter == 0)
            sevenSeg(health, 0);
        else
            sevenSeg(game_level, 3);
        break;

    case LEVEL3_INIT:
        if (sevenSeg2WayCounter == 0)
            sevenSeg(health, 0);
        else
            sevenSeg(game_level, 3);
        break;
    case LEVEL3:
        if (sevenSeg2WayCounter == 0)
            sevenSeg(health, 0);
        else
            sevenSeg(game_level, 3);
        break;
    case END:
        if (sevenSeg3WayCounter == 0)
            sevenSeg(11, 0); // E
        else if (sevenSeg3WayCounter == 1)
            sevenSeg(12, 1); // n
        else
            sevenSeg(13, 2); // d
        break;
    case LOSE:
        if (sevenSeg4WayCounter == 0)
            sevenSeg(10, 0); // L
        else if (sevenSeg4WayCounter == 1)
            sevenSeg(0, 1); // O
        else if (sevenSeg4WayCounter == 2)
            sevenSeg(5, 2); // S
        else
            sevenSeg(11, 3); // E
        break;
        default:
            break;
    }
}

void sevenSeg(uint8_t J, uint8_t D)
{
    switch (J)
    {

    // All dps are reset (i.e., bit7 -> 0)
    case 0:           // Also case O
        PORTJ = 0x3f; // abcdef    -> 1111 1100
        break;
    case 1:
        PORTJ = 0x06; // bc         -> 0110 0000
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
    case 10: // L
        PORTJ = 0x38;
        break;
    case 11: // E
        PORTJ = 0x79;
        break;
    case 12: // n
        PORTJ = 0x54;
        break;
    case 13: // d
        PORTJ = 0x5e;
        break;
    }
    switch (D)
    {
    case 0:
        PORTH = 0x01; // RH0 = 1, others = 0
        break;
    case 1:
        PORTH = 0x02; // RH1 = 1, others = 0
        break;
    case 2:
        PORTH = 0x04; // RH2 = 1, others = 0
        break;
    case 3:
        PORTH = 0x08; // RH3 = 1, others = 0
        break;
    }
}

// ************* Game task and functions ****************
// This task handles the overall game logic and control remaining tasks
// through their utility functions and flags

// This function basiaclly shifts PORTS A --> way to --> F
void shape_shifter()
{
    PORTF = PORTE;
    PORTE = PORTD;
    PORTD = PORTC;
    LATC = PORTB;
    PORTB = PORTA;
    
    // We clear the bits 4-7 of PORTA-..-F
    PORTA &= 0x1f;
    PORTB &= 0x1f;
    LATC &= 0x1f;
    PORTD &= 0x1f;
    PORTE &= 0x1f;
    PORTF &= 0x1f;
    
    // Reset PORTA, after shape_shifter a random note is generated on PORTA
    PORTA = 0x00;
}

void reset_task()
{
    // This functions basically resets the needed variables.
    // Configures the game logic control flags and initiliazes PORTS.
    isGameStarted = 0;
    isGameFinished = 1;
    PORTC = 0x00;
    init_ports();
}

void health_decreaser()
{
    // This function decreases health and if health is 0 --> game state goes to LOSE
    health--;
    if (health == 0)
    {
        game_state = LOSE;
    }
}

void game_task()
{
    // This count is for controlling if the player pressed
    uint8_t count = 0;
    if (isRG0Pressed == 1)
    {
        count++;
        isRG0Pressed = 2;
    }
    if (isRG1Pressed == 1)
    {
        count++;
        isRG1Pressed = 2;
    }
    if (isRG2Pressed == 1)
    {
        count++;
        isRG2Pressed = 2;
    }
    if (isRG3Pressed == 1)
    {
        count++;
        isRG3Pressed = 2;
    }
    if (isRG4Pressed == 1)
    {
        count++;
        isRG4Pressed = 2;
    }

    if (count > 1)
    {
        whichRG = -1;
        
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    switch (whichRG)
    {
    case -1:
        while(count--)
            health_decreaser();
        break;
    case 0:
        if (PORTFbits.RF0 == 1) {
            PORTF = 0X00;
            isTruePressed = 1;
        }
        else
            health_decreaser();
        break;
    case 1:
        if (PORTFbits.RF1 == 1) {
            PORTF = 0X00;
            isTruePressed = 1;
        }
            
        else
            health_decreaser();
        break;
    case 2:
        if (PORTFbits.RF2 == 1) {
            PORTF = 0X00;
            isTruePressed = 1;
        }
            
        else
            health_decreaser();
        break;
    case 3:
        if (PORTFbits.RF3 == 1) {
            PORTF = 0X00;
            isTruePressed = 1;
        }
            
        else
            health_decreaser();
        break;
    case 4:
        if (PORTFbits.RF4 == 1) {
            PORTF = 0X00;
            isTruePressed = 1;
        }
            
        else
            health_decreaser();
        break;
    default:
        break;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    whichRG = 5;
    switch (game_state)
    {
    case G_INIT:
        tmr_start(77); // TMR0 counts 77 times so that 500 ms
        game_state = LEVEL1;
        // shape_shifter();   // Shift RA->RB, RB-RC, ... , RE->RF
        randomgen(); // generate note
        ++level_subcount;
        game_level = 1;
        starterDelay = 0;
        break;
    case LEVEL1:
        // START state

        if (tmr_state == TMR_DONE) // 500 ms passed
        {
            starterDelay++;
            if (isTruePressed == 0 && starterDelay >= 6)
            {
                health_decreaser();
            }
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
            isTruePressed = 0;
        }
        break;
    case LEVEL2_INIT:
        level_subcount = 0;
        tmr_start(61); // TMR0 counts 61 times so that 500 ms
        game_state = LEVEL2;
        // shape_shifter();   // Shift RA->RB, RB-RC, ... , RE->RF
        randomgen(); // generate note
        ++level_subcount;
        game_level = 2;
        starterDelay = 0;
        break;
    case LEVEL2:
        if (tmr_state == TMR_DONE) // 400 ms passed
        {
            starterDelay++;
            if (isTruePressed == 0 && starterDelay >= 6)
            {
                health_decreaser();
            }
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
                game_state = LEVEL3_INIT;
            }
            tmr_start(61); // TMR0 counts 61 times so that 400 ms
            isTruePressed = 0;
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
        starterDelay = 0;
        break;
    case LEVEL3:
        if (tmr_state == TMR_DONE) // 300 ms passed
        {
            starterDelay++;
            if (isTruePressed == 0 && starterDelay >= 6)
            {
                health_decreaser();
            }
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
            tmr_start(46); // TMR0 counts 46 times so that 300 ms
            isTruePressed = 0;
        }
        break;
    case LOSE:
        reset_task();
        break;
    case END:
        reset_task();
        break;
    }
}

void main(void)
{
    init_vars();  
    init_ports(); 
    tmr_init();   
    init_irq();   
    game_state = INIT_START;
    while (1)
    {
        input_task();
        sevenSeg_controller();
        if ((isGameStarted == 0) || (isGameFinished == 1))
        {
            continue;
        }
        game_task();
    }
}