/*
 * File:   newmain.c
 * Author: enesizgi
 *
 * Created on May 4, 2022, 3:42 PM
 */


#include <xc.h>

void sevenSeg(char J, char D){
    switch(J){  
        // All dps are reset (i.e., bit7 -> 0)
        case '0': // Also case O
            PORTJ = 0xfc;  // abcdef    -> 1111 1100
            break;
        case '1':
            PORTJ = 0xbc; // bc         -> 0110 0000
            break;
        case '2':
            PORTJ = 0xda;
            break;
        case '3':
            PORTJ = 0xf2;
            break;
        case '4': 
            PORTJ = 0x66;  
            break;
        case '5':
            PORTJ = 0xb6; 
            break;
        case '6':
            PORTJ = 0xbe;
            break;
        case '7':
            PORTJ = 0xe0;
            break;
        case '8': 
            PORTJ = 0xfe; 
            break;
        case '9':
            PORTJ = 0xf6; 
            break;
        case 'L':
            PORTJ = 0x1b;
            break;
        case 'E':
            PORTJ = 0x9e;
            break;
        case 'n': 
            PORTJ = 0x2a; 
            break;
        case 'd':
            PORTJ = 0x7a; 
            break;
    }
    switch(D){
        case 'D0':
            PORTH = 0x01;       // RH0 = 1, others = 0
            break;
        case 'D1':
            PORTH = 0x02;       // RH1 = 1, others = 0
            break;
        case 'D2':
            PORTH = 0x04;       // RH2 = 1, others = 0
            break;
        case 'D3':
            PORTH = 0x08;       // RH3 = 1, others = 0
            break;
    }
}
void main(void) {    
    return;
}
