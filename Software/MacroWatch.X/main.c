/*
 * File:   main.c
 * Author: Parker Dillmann
 *
 * Created on October 26, 2015, 4:07 PM
 */

#define _XTAL_FREQ 32768


// PIC16F527 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG
#pragma config FOSC = LP        // Oscillator Selection Interal
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

void TEST_LED_PATTERN(void);

char HOUR   = 12;
char MIN    = 12;
char SEC    = 0;
int INTCNT  = 0;

char Disp_timeout = 0;
char Change_time  = 0;

char BUTT_action = 0;

char STATE = 0;

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
    
    TEST_LED_PATTERN();
        
    while (1)
    {
        switch(STATE)
        {
            //OFF STATE
            case 0:            
                PORTB = 0x00;
                PORTC = 0x00;
                
                if(SW == 0)
                {
                    STATE = 1;  
                    BUTT_action = 1;
                }
                else
                {
                    STATE = 0;
                }
                break;
                
            //BUTTON PRESSED. RECORD TIME.
            case 1: 
                
                Disp_timeout = SEC; 
                Disp_timeout = Disp_timeout + 10;
                
                if(Disp_timeout >= 60)
                {
                    Disp_timeout = Disp_timeout - 60;
                }
                
                Change_time = SEC;
                Change_time = Change_time + 3;
                
                if(Change_time >= 60)
                {
                    Change_time = Change_time - 60;
                }
                
                STATE = 2; 

                break;
            
            //TURN ON DISPLAY. AND WAIT FOR TIMEOUTS    
            case 2:
               
                HOURLED_0 = HOUR & 0b00000001;
                HOURLED_1 = (HOUR >> 1) & 0b00000001;
                HOURLED_2 = (HOUR >> 2) & 0b00000001;
                HOURLED_3 = (HOUR >> 3) & 0b00000001;
                
                MINLED_0  = MIN & 0b00000001; 
                MINLED_1  = (MIN >> 1) & 0b00000001; 
                MINLED_2  = (MIN >> 2) & 0b00000001;
                MINLED_3  = (MIN >> 3) & 0b00000001; 
                MINLED_4  = (MIN >> 4) & 0b00000001; 
                MINLED_5  = (MIN >> 5) & 0b00000001; 
                
                SECOUT    = SEC & 0b00000001; 
                
                if(SW == 1)
                {
                    BUTT_action = 0;                    
                }
                
                if(Disp_timeout == SEC)
                {
                    STATE = 0;
                }
                
                if(Change_time == SEC && SW == 0 && BUTT_action == 1)
                {
                    STATE = 3;
                }
                
                break;
              
            //INCREASE TIME AND DISPLAY TIME.    
            case 3:
                GIE = 0;
                
                if(SW == 0)
                {
                    SEC = 0;
                    MIN = MIN + 1;
                    if(MIN == 60)
                    {
                        MIN = 0;
                        HOUR = HOUR + 1;
                        if(HOUR == 13)
                        {
                            HOUR = 1;   
                        }   
                    }
                }     
                else if(SW == 1)
                {
                    GIE = 1;
                    STATE = 2;
                }
                
                HOURLED_0 = HOUR & 0b00000001;
                HOURLED_1 = (HOUR >> 1) & 0b00000001;
                HOURLED_2 = (HOUR >> 2) & 0b00000001;
                HOURLED_3 = (HOUR >> 3) & 0b00000001;
                
                MINLED_0  = MIN & 0b00000001; 
                MINLED_1  = (MIN >> 1) & 0b00000001; 
                MINLED_2  = (MIN >> 2) & 0b00000001;
                MINLED_3  = (MIN >> 3) & 0b00000001; 
                MINLED_4  = (MIN >> 4) & 0b00000001; 
                MINLED_5  = (MIN >> 5) & 0b00000001; 
                
                __delay_ms(150);

                break;
                
            default:
                
                STATE = 0;
                
                break;   
        }

                
    }
    return;
}

void TEST_LED_PATTERN(void)
{
    char mask = 0x01;
    
    for(int i = 0; i < 8; i++)
    {
        PORTB = mask;
        PORTC = mask;
        mask = mask << 1;
        __delay_ms(500);
    }
    
    return;
}

void interrupt isr(void)
{
    INTCNT = INTCNT + 1;
    if(INTCNT == 32)
    {  
        INTCNT = 0; 
        SEC = SEC + 1;
        
        if (SEC == 60)
        {
            SEC = 0;
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
    }
    T0IF = 0;
    return;
}