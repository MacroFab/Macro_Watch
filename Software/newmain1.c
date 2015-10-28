/*
 * File:   newmain1.c
 * Author: MacroFab_00
 *
 * Created on October 26, 2015, 4:07 PM
 */

#define _XTAL_FREQ 32000


// PIC16F527 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG
#pragma config FOSC = LP  // Oscillator Selection Interal
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT Disabled)
#pragma config CP = OFF         // Code Protection - User Program Memory (Code protection off)
#pragma config MCLRE = ON       // Master Clear Enable (MCLR pin functions as MCLR)
#pragma config IOSCFS = 8MHz    // Internal Oscillator Frequency Select (8 MHz INTOSC Speed)
#pragma config CPSW = OFF       // Code Protection - Self Writable Memory (Code protection off)
#pragma config BOREN = OFF      // Brown-out Reset Enable (BOR Enabled)
#pragma config DRTEN = OFF      // Device Reset Timer Enable (DRT Disabled)

#define HOURLED_0 RB4
#define HOURLED_1 RB5
#define HOURLED_2 RB6
#define HOURLED_3 RB7

#define MINLED_0  RC0
#define MINLED_1  RC1
#define MINLED_2  RC2
#define MINLED_3  RC3
#define MINLED_4  RC4
#define MINLED_5  RC5

#define SECOUT    RC6

#define SW        RC7

char HOUR = 1;
char MIN  = 0;
int INTCNT = 0;

void main(void) {
        
    CM1CON0 = 0x00; // <- this mother fucker right here.
    CM2CON0 = 0x00;
    OPTION = 0b11001000;
    ANSEL = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    TRISB = 0x00;
    TRISC = 0x80;

    T0IE = 1;
    GIE=1;    
    
   
    
    //char HOUR = 0b00000001;
    //char MIN  = 0b00000000;
        
    while (1)
    {
        PORTB = HOUR << 4;
        PORTC = MIN;

//        if(SW == 0)
//        {
//            PORTB = HOUR << 4;
//            PORTC = MIN;
//        }
//        else
//        {
//            PORTB = 0b00000000;
//            PORTC = 0b00000000;
//        }
        //SLEEP();
    }
    return;
}

void interrupt isr(void)
{
    SECOUT = 1;
    INTCNT = INTCNT + 1;
    if(INTCNT == 32)
    {  
        INTCNT = 0; 
        MIN = MIN + 1;

        if (MIN == 60)
        {
            MIN = 0;
            HOUR = HOUR + 1;
            if(HOUR == 13)
            {
                HOUR = 1;
            } 
        }
    }
    SECOUT = 0;
    T0IF = 0;
    return;
}