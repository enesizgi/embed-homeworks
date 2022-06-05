#include <xc.h>
#include <stdint.h>
// CONFIG Oscillator and TURN OFF Watchdog timer
#pragma config OSC = HSPLL
#pragma config WDT = OFF

/*_* GLOBAL DECLERATIONS GOES HERE */
typedef enum {TEM, CDM, TSM} game_state_t;
game_state_t game_state = TEM;

uint8_t nOfCustom;      // Number of custom characters
bool re0Pressed, re1Pressed, re2Pressed, re3Pressed, re4Pressed, re5Pressed;        // flags for input

/*_* Initialize variables */
void init_vars()
{
    re0Pressed = re1Pressed = re2Pressed = re3Pressed = re4Pressed = re5Pressed = false;
    game_state = TEM;
    nOfCustom = 0;
}

void init_ports()
{
    /*_* INPUT TRISSES*/
    TRISE = 0x3f;       // 0011 1111 -> RE 0-5
    TRISH = 0X10;       // 0001 0000 -> RH4

    /*_* OUTPUT TRISSES*/
    // LCD BASED TRISSES
    // TRISB = TODO
    // TRISD = TODO 
    TRISA = 0X00;
    TRISC = 0X00;

    // 7-SEG BASED TRISSES
    // PORTH IS EDITED UPWARDS
    PORTJ = 0X00;

}

void sev_seg_task()
{

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

void game_task()
{
    switch (game_state)
    {
    case TEM:
        break;

    case CDM:
        break;

    case TSM:
        break;
    
    default:
        break;
    }
}

void lcd_task()
{
    
}

int main()
{

    init_vars();
    init_ports();

    while(1)
    {
        sev_seg_task();
        input_task();
        game_task();
        lcd_task();

    }
    return 0;
}