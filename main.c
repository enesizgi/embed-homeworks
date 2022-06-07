#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "Includes.h"
// CONFIG Oscillator and TURN OFF Watchdog timer
#pragma config OSC = HSPLL
#pragma config WDT = OFF

#define pressed 2
#define true 1
#define false 0
#define _XTAL_FREQ 40000000

#define left 0
#define right 1
#define up 2
#define down 3

// MACROS FOR LCD

#define lcd_up 0x80
#define lcd_down 0xc0
#define cst 1
#define prd 0

#define empty 0

void tmr_isr();
void lcd_task();

/*_* GLOBAL DECLERATIONS GO HERE */
typedef enum {TEM, CDM, TSM} game_state_t;
game_state_t game_state = TEM;

uint8_t nOfCustom;      // Number of custom characters, range [1-8], both excluded
uint8_t sevenSeg3WayCounter;    // counter for 7seg display
uint8_t cursorClm, cursorRow;
uint8_t count_to_8;

unsigned int result, upCursor;

// Flags
uint8_t re0Pressed, re1Pressed, re2Pressed, re3Pressed, re4Pressed, re5Pressed;        // flags for input
uint8_t adif, adif2;

char predefined[] = {' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char currPredChar;
uint8_t currPredIndex;

char currCustChar;
uint8_t currCustIndex;


uint8_t lcd_buf_up[16][8];
uint8_t lcd_buf_down[16][8];
uint8_t custom_chars[8][8];

uint8_t charmap[8] = {
        0b00000,
        0b00000,
        0b01010,
        0b11111,
        0b11111,
        0b01110,
        0b00100,
        0b00000,
};

uint8_t charmap2[8] = {
        0b10000,
        0b01000,
        0b00110,
        0b00111,
        0b00111,
        0b01110,
        0b01100,
        0b01111,
};

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

void cust_next()
{
    currCustIndex = (currCustIndex+1)%8;
    currCustChar = custom_chars[currCustIndex];
}

void cust_prev()
{
    currCustIndex = (currCustIndex-1)%8;
    currCustChar = custom_chars[currCustIndex];
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
    upCursor = 0;

    count_to_8 = 0;
    currPredChar = predefined[currPredIndex];
    for (int i = 0; i < 16;i++) {
        for (int j = 0; j < 8; j++) {
            lcd_buf_up[i][j] = 0;
            lcd_buf_down[i][j] = 0;
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            custom_chars[i][j] = empty;
        }
    }
}

void Pulse(void){
    PORTBbits.RB5 = 1;
    __delay_us(30);
    PORTBbits.RB5 = 0;
    __delay_us(30);
}

void SendBusContents(uint8_t data){
    PORTD = PORTD & 0x0F;           // Clear bus
    PORTD = PORTD | (data&0xF0);     // Put high 4 bits
    Pulse();
    PORTD = PORTD & 0x0F;           // Clear bus
    PORTD = PORTD | ((data<<4)&0xF0);// Put low 4 bits
    Pulse();
}

void InitLCDv2(void) {
    // Initializing by Instruction
    __delay_ms(20);
    PORTD = 0x30;
    Pulse();

    __delay_ms(6);
    PORTD = 0x30;
    Pulse();

    __delay_us(300);
    PORTD = 0x30;
    Pulse();

    __delay_ms(2);
    PORTD = 0x20;
    Pulse();
    PORTBbits.RB2 = 0;
    SendBusContents(0x2C);
    SendBusContents(0x0E);
    SendBusContents(0x01);
    __delay_ms(30);
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
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    LATD = 0x00;
    TRISJ = 0x00;
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
        adif2 = true;
        upCursor = result/64;        // MAYBE division may be erronous but I think it is OK @Ali
        move_cursor(lcd_up + upCursor);        // 1024/16 = 64, so if we divide our result by 64, it will be the index to the cell in the upper side of LCD
    }
}

void input_task()
{
    // TODO : We should implement button release detection
    if(PORTEbits.RE0)
    {
        re0Pressed = pressed;
    }
    else if (re0Pressed == pressed) {
        re0Pressed = true;
    }

    if(PORTEbits.RE1)
    {
        re1Pressed = pressed;
    }
    else if (re1Pressed == pressed) {
        re1Pressed = true;
    }

    if(PORTEbits.RE2)
    {
        re2Pressed = pressed;
    }
    else if (re2Pressed == pressed) {
        re2Pressed = true;
    }

    if(PORTEbits.RE3)
    {
        re3Pressed = pressed;
    }
    else if (re3Pressed == pressed) {
        re3Pressed = true;
    }

    if(PORTEbits.RE4)
    {
        re4Pressed = pressed;
    }
    else if (re4Pressed == pressed) {
        re4Pressed = true;
    }

    if(PORTEbits.RE5)
    {
        re5Pressed = pressed;
    }
    else if (re5Pressed == pressed) {
        re5Pressed = true;
    }
    
}

void inv_cursor(uint8_t direction)
{
    switch (direction)
    {
    case left:
        if(cursorClm != 0 && re3Pressed == true) cursorClm--;
        break;
    case right:
        if(cursorClm != 3 && re0Pressed == true) cursorClm++;
        break;
    case up:
        if(cursorRow != 0 && re2Pressed == true) cursorRow--;
        break;
    case down:
        if(cursorRow != 7 && re1Pressed == true) cursorRow++;
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
        if(re0Pressed == true)      // custom -> forward
        {
            // TODO custom character array and logic
            cst_next();
            re0Pressed = false;
        }

        if(re1Pressed == true)      // predefined -> backwars
        {
            pred_prev();
            re1Pressed = false;
            write_lcd(lcd_up + upCursor, prd, currPredChar);
        }

        if(re2Pressed == true)      // predefined -> forwards
        {
            pred_next();
            re2Pressed = false;
            write_lcd(lcd_up + upCursor, prd, currPredChar);
        }

        if(re3Pressed == true)      // custom -> backwards
        {
            cst_prev();
            re3Pressed = false;
        }

        if(re4Pressed == true)
        {
            re4Pressed = false;
            game_state = CDM;
        }

        if(re5Pressed == true)
        {
            re5Pressed = false;
            game_state = TSM;
        }
        break;

    case CDM:
        if(re0Pressed == true)      // cursor -> right
        {
            inv_cursor(right);
            re0Pressed = false;
        }

        if(re1Pressed == true)      // cursor -> down
        {
            inv_cursor(down);
            re1Pressed = false;
        }

        if(re2Pressed == true)      // cursor -> up
        {
            inv_cursor(up);
            re2Pressed = false;
        }

        if(re3Pressed == true)      // cursor -> left
        {
            inv_cursor(left);
            re3Pressed = false;
        }

        if(re4Pressed == true)
        {
            re4Pressed = false;
            // TODO toggle led
            if (cursorClm == 0) {
                LATA ^= 1 << cursorRow;
            }
            else if (cursorClm == 1) {
                LATB ^= 1 << cursorRow;
            }
            else if (cursorClm == 2) {
                LATC ^= 1 << cursorRow;
            }
            else if (cursorClm == 3) {
                LATD ^= 1 << cursorRow;
            }
        }
        if(re5Pressed == true)
        {
            re5Pressed = false;
            game_state = TEM;
            uint8_t temp[8]; // TODO: @Enes - Replace this with custom_chars array
            temp[nOfCustom] = LA0 << 4 | LB0 << 3 | LC0 << 2 | LD0 << 1;
            temp[nOfCustom+1] = LA1 << 4 | LB1 << 3 | LC1 << 2 | LD1 << 1;
            temp[nOfCustom+2] = LA2 << 4 | LB2 << 3 | LC2 << 2 | LD2 << 1;
            temp[nOfCustom+3] = LA3 << 4 | LB3 << 3 | LC3 << 2 | LD3 << 1;
            temp[nOfCustom+4] = LA4 << 4 | LB4 << 3 | LC4 << 2 | LD4 << 1;
            temp[nOfCustom+5] = LA5 << 4 | LB5 << 3 | LC5 << 2 | LD5 << 1;
            temp[nOfCustom+6] = LA6 << 4 | LB6 << 3 | LC6 << 2 | LD6 << 1;
            temp[nOfCustom+7] = LA7 << 4 | LB7 << 3 | LC7 << 2 | LD7 << 1;
//            custom_chars[nOfCustom] = temp;
            PORTBbits.RB2 = 0;

            SendBusContents(0x40);
//    count_to_8 = (count_to_8+8) % 64;        // MAYBE DISABLE OVERWRITE

            // Start sending charmap
            for(int i=0; i<8; i++){
                PORTBbits.RB2 = 1; // Send Data
                SendBusContents(temp[i]);
            }
            SendBusContents(0x02);
            nOfCustom++;
            LATA = 0x00;
            LATB = 0x00;
            LATC = 0x00;
            LATD = 0x00;
            cursorClm = 0;
            cursorRow = 0;
        }
        break;

    case TSM:
        break;
    
    default:
        break;
    }
}

void generate_custom_char(uint8_t chars[])      // TODO: Re-organize this function
{
    PORTBbits.RB2 = 0;

    SendBusContents(0x40);
//    count_to_8 = (count_to_8+8) % 64;        // MAYBE DISABLE OVERWRITE

    // Start sending charmap
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            PORTBbits.RB2 = 1; // Send Data
            if(custom_chars[i][j] != empty)
                SendBusContents(custom_chars[i][j]);
        }

    }

    // All custom characters generated again
    // Return home
    SendBusContents(0x02);


}

// Moves the cursor to the desired address
void move_cursor(uint8_t address)
{
    PORTBbits.RB2 = 0;
    SendBusContents(address);
}


void write_lcd(uint8_t address, uint8_t mode, uint8_t character)        // To use in cst mode, send 0-1-2-... intead of 0-8-16-...
{
    switch(mode)
    {
        case cst:
            move_cursor(address);
            // Write buf to LCD DDRAM
            PORTBbits.RB2 = 1;
            SendBusContents(character);
            // Move cursor again the current cell
            move_cursor(address);
            break;
        case prd:
            move_cursor(address);
            // Write buf to LCD DDRAM
            PORTBbits.RB2 = 1;
            SendBusContents(character);
            // Move cursor again the current cell
            move_cursor(address);
            break;
    }

}

void lcd_task() 
{
    if (adif2) {
//         4bytes for ADC Res + 1 byte for custom char + 1 byte null;
        char buf[6];
        sprintf(buf, "%04u", result);
        buf[4]=0; // Address of custom char
        buf[5]=0; // Null terminator

        // Set DDRAM address to 0 (line 1 cell 1) -> 0x80
//        PORTBbits.RB2 = 0;
//        write_lcd(lcd_up+7, cst, 0);
//        write_lcd(lcd_up+8, cst, 1);

        adif2 = false;
    }

}

int main()
{
    init_vars();
    init_ports();
    init_adc();
    InitLCDv2();
    init_irq();
    tmr_init();
    start_adc();

//    INTCONbits.GIE = 1;
// Define custom char LCD
    // Set CGRAM address 0 -> 0x40
    PORTBbits.RB2 = 0;

    SendBusContents(0x40);
//    count_to_8 = (count_to_8+8) % 64;        // MAYBE DISABLE OVERWRITE

    // Start sending charmap
    for(int i=0; i<8; i++){
        PORTBbits.RB2 = 1; // Send Data
        SendBusContents(charmap[i]);
    }

//    PORTBbits.RB2 = 0;

//    SendBusContents(0x41);
//    count_to_8 = (count_to_8+8) % 64;        // MAYBE DISABLE OVERWRITE

    // Start sending charmap
    for(int i=0; i<8; i++){
        PORTBbits.RB2 = 1; // Send Data
        SendBusContents(charmap2[i]);
    }
//
//    generate_custom_char(charmap);
//    generate_custom_char(charmap2);

    SendBusContents(0x02);

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