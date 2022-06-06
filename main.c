#include <xc.h>
#include <stdint.h>
// CONFIG Oscillator and TURN OFF Watchdog timer
#pragma config OSC = HSPLL
#pragma config WDT = OFF

#define true 1
#define false 0

#define left 0
#define right 1
#define up 2
#define down 3

void tmr_isr();
void lcd_task();

/*_* GLOBAL DECLERATIONS GO HERE */
typedef enum {TEM, CDM, TSM} game_state_t;
game_state_t game_state = TEM;

uint8_t nOfCustom;      // Number of custom characters, range [1-8], both excluded
uint8_t sevenSeg3WayCounter;    // counter for 7seg display
uint8_t cursorClm, cursorRow;
unsigned int result;

// Flags
uint8_t re0Pressed, re1Pressed, re2Pressed, re3Pressed, re4Pressed, re5Pressed;        // flags for input
uint8_t adif;

char predefined[] = {' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char currPredChar;
uint8_t currPredIndex;

void pred_next()
{
    currPredIndex = (currPredIndex+1)%37;
    currPredChar = predefined[currPredIndex];
}

void pred_prev()
{
    currPredIndex = (currPredIndex-1)%37;
    currPredChar = predefined[currPredIndex];
}

/*_* Interrupt Service Routines */
void __interrupt(high_priority) highPriorityISR(void)
{
    if (INTCONbits.TMR0IF)
    {
        tmr_isr();
    }
    if(PIR1bits.ADIF)
    {
        PIR1bits.ADIF = 0x00;
        adif = true;
    }
}

void tmr_isr()
{
    // We increase all sevenSegWayCounter variables and take modulo operation for all of them.
    // Using modulo operation caused some problems on our board, so we used simple if else.

    if (sevenSeg3WayCounter == 2)
        sevenSeg3WayCounter = 0;
    else
        sevenSeg3WayCounter++;

    INTCONbits.TMR0IF = 0; // Reset flag
}

void sevenSeg(uint8_t J, uint8_t D)
{
    // This function changes the 7 segment display to the desired value.
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

/*_* Initialize variables */
void init_vars()
{
    re0Pressed = re1Pressed = re2Pressed = re3Pressed = re4Pressed = re5Pressed = false;
    adif = false;
    game_state = TEM;
    nOfCustom = 0;
    sevenSeg3WayCounter = 0;
    cursorClm = cursorRow = 0;
    currPredIndex = 0;
    result = 0;
    currPredChar = predefined[currPredIndex];
}

void init_ports()
{
    /*_* INPUT TRISSES*/
    TRISE = 0x3f;       // 0011 1111 -> RE 0-5
    TRISH = 0X10;       // 0001 0000 -> RH4

    /*_* OUTPUT TRISSES*/
    // LCD BASED TRISSES
    TRISB = 0x00;
    TRISD = 0x00;

    // Other PORTs
    TRISA = 0X00;
    TRISC = 0X00;

    // 7-SEG BASED TRISSES
    // PORTH IS EDITED UPWARDS
    PORTJ = 0X00;
}

void init_adc()
{
    // Configure ADC
    ADCON0 = 0x31; // Channel 12; Turn on AD Converter
    ADCON1 = 0x00; // All analog pins
    ADCON2 = 0xAA; // Right Align | 12 Tad | Fosc/32
    ADRESH = 0x00;
    ADRESL = 0x00;
    PIR1bits.ADIF = 0x00;
    PIE1bits.ADIE = 0x01;

}

void init_irq()
{
    // INTCON is configured to use TMR0 interrupts
    INTCON = 0xa0;
}

void tmr_init()
{
    // In order to achieve a 500ms-400ms-300ms delay, we will use Timer0 in 8-bit mode.
    // This setup assumes a 40MHz 18F8722, which corresponds to a 10MHz
    // instruction cycle
    T0CON = 0xc7; // internal clock with 1:256 prescaler and 8-bit MAYBE 16-bit
    TMR0 = 0x00;  // Initialize TMR0 to 0, without a PRELOAD
}

void start_adc()
{
    ADCON0bits.GODONE = 0x01;
}

void sev_seg_task()
{
    if(sevenSeg3WayCounter == 0)
        sevenSeg(nOfCustom, 0);
    else if(sevenSeg3WayCounter == 1)
        sevenSeg(cursorClm, 2);
    else
        sevenSeg(cursorRow, 3);
}

void adc_finish()
{
    if(adif)
    {
        result = (ADRESH << 8) + ADRESL; // Get the result;
        adif = false;
        init_adc();     // MAYBE we do not enable GIE again and again, because we are not disabling GIE
        start_adc();
    }
}

void input_task()
{
    if(PORTEbits.RE0)
    {
        re0Pressed = true;
    }
    if(PORTEbits.RE1)
    {
        re1Pressed = true;
    }
    if(PORTEbits.RE2)
    {
        re2Pressed = true;
    }
    if(PORTEbits.RE3)
    {
        re3Pressed = true;
    }
    if(PORTEbits.RE4)
    {
        re4Pressed = true;
    }
    if(PORTEbits.RE5)
    {
        re5Pressed = true;
    }
    
}

void inv_cursor(uint8_t dir)
{
    switch (dir)
    {
    case left:
        if(cursorClm != 0) cursorClm--;
        break;
    case right:
        if(cursorClm != 3) cursorClm++;
        break;
    case up:
        if(cursorRow != 0) cursorRow--;
        break;
    case down:
        if(cursorRow != 7) cursorRow++;
        break;
    
    default:
        break;
    }
}

void game_task()
{
    switch (game_state)
    {
    case TEM:
        if(re0Pressed)
        {
            // TODO custom character array and logic
            re0Pressed = false;
        }

        if(re1Pressed)      // predefined -> backwars
        {
            pred_prev();
            re1Pressed = false;
        }

        if(re2Pressed)      // predefined -> forwards
        {
            pred_next();
            re2Pressed = false;
        }

        if(re3Pressed)
        {
            re3Pressed = false;
        }

        if(re4Pressed)
        {
            re4Pressed = false;
            game_state = CDM;
        }

        if(re5Pressed)
        {
            re5Pressed = false;
            game_state = TSM;
        }
        break;

    case CDM:
        if(re0Pressed)      // cursor -> right
        {
            inv_cursor(right);
            re0Pressed = false;
        }

        if(re1Pressed)      // cursor -> down
        {
            inv_cursor(down);
            re1Pressed = false;
        }

        if(re2Pressed)      // cursor -> up
        {
            inv_cursor(up);
            re2Pressed = false;
        }

        if(re3Pressed)      // cursor -> left
        {
            inv_cursor(left);
            re3Pressed = false;
        }

        if(re4Pressed)
        {
            re4Pressed = false;
            // TODO toggle led
        }
        if(re5Pressed)
        {
            re5Pressed = false;
            game_state = TEM;
        }
        break;

    case TSM:
        break;
    
    default:
        break;
    }
}

int main()
{
    init_vars();
    init_ports();
    init_adc();
    init_irq();
    tmr_init();
    start_adc();

    while(1)
    {
        adc_finish();
        sev_seg_task();
        input_task();
        game_task();
        lcd_task();
    }
    return 0;
}