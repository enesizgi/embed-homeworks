#include "common.h"

#define MAX_PARAM_LEN 19        // MAX length is HASH funciton's command, it is 19 bytes long
/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
unsigned char transmit_size = 0;
unsigned char transmit_index = 0;
char char_to_smltr = 0;
char rcvd_chr = 0;

unsigned char head, tail, head_2, tail_2;
unsigned char buffer[BUFSIZE], buffer2[BUFSIZE];

/**********************************************************************
 * ----------------------- GLOBAL FUNCTIONS ---------------------------
 **********************************************************************/

unsigned int buf_isempty(unsigned char bufno) {
    if(bufno == 1)
        return (head == tail);
    return (head_2 == tail_2);      
}

void buf_push(unsigned char v, unsigned char bufno)
{ /* Place new data in buffer */
    INTCONbits.GIE = 0;
    if(bufno == 1)
    {
        buffer[head++] = v;
            if (head == BUFSIZE)
                head = 0;
    }   
    else
    {
        buffer2[head_2++] = v;
            if (head_2 == BUFSIZE)
                head_2 = 0;

    }
//////////////////////////////////////////
    if (head == tail)
    { /* Overflow!!! */
        // error();
    }
    INTCONbits.GIE = 1;
}

unsigned char buf_pop(unsigned char bufno)
{ /* Retrieve data from buffer */
    unsigned char v;
    INTCONbits.GIE = 0;
    if(bufno == 1)
    {
        if (buf_isempty(bufno))
        { /* Underflow!! */
    //       error();
            return 0;
        }
        else
        {
            v = buffer[tail++];
            if (tail == BUFSIZE)
                tail = 0;
            return v;
        }
    }
    else
    {
        if (buf_isempty(bufno))
        { /* Underflow!! */
    //       error();
            return 0;
        }
        else
        {
            v = buffer2[tail++];
            if (tail_2 == BUFSIZE)
                tail_2 = 0;
            return v;
        }

    }
    INTCONbits.GIE = 1;
}

void transmitChar()
{
    // Sets the transmit strings length and enables the TXEN bit
//    transmit_index = ind;
	INTCON = 0;			// clear interrupt register completely
	PIE1bits.TX1IE = 1;	// enable USART transmit interrupt
	PIE1bits.RC1IE = 1;	// enable USART receive interrupt
	PIR1 = 0;			// clear all peripheral flags
	
	INTCONbits.PEIE = 1;// enable peripheral interrupts
	INTCONbits.GIE = 1;	// globally enable interrupts
    TXREG1 = buf_pop(2);
    TXSTA1bits.TXEN = 1; //enable transmission.
}

void transmitData()     // TODO: replace hello to str_to_send
{
    TXSTA1bits.TXEN = 0;
    // if(transmit_index-- > 0)
    // {
    // TXREG1 = buf_pop(2);
    //     TXSTA1bits.TXEN = 1;
    // }
     //enable transmission.
}


/* Invoked when receive interrupt occurs; meaning that data is received */
void dataReceived()
{
    rcvd_chr = RCREG1;
    buf_push(rcvd_chr, 1);
}

/* End of File : common.c */
