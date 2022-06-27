#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
char hello[6] = "hello";
unsigned char hello_ind = 0;
char hello_chr = 0;
char rcvd_chr = 0;

/**********************************************************************
 * ----------------------- GLOBAL FUNCTIONS ---------------------------
 **********************************************************************/

void transmitCharAndHello(char chr)
{
    hello_chr = chr;
    TXSTA1bits.TXEN = 1; //enable transmission.
}

void transmitData()
{
    if(hello_ind <= 4)
    {
        TXREG1 = hello[hello_ind];
        hello_ind++;
    }
    else if(hello_ind == 5)
    {
        TXREG1 = hello_chr;
        hello_ind++;
    }
    else
    {
        hello_ind = 0;
        TXSTA1bits.TXEN = 0;// disable transmission
    }
}


/* Invoked when receive interrupt occurs; meaning that data is received */
void dataReceived()
{
    rcvd_chr = RCREG1;
}

/* End of File : common.c */
