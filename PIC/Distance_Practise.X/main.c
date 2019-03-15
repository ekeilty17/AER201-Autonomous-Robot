/**
 * @file
 * @author Michael Ding
 * @author Tyler Gamvrelis
 *
 * Created on July 18, 2016, 12:11 PM
 *
 * @defgroup CharacterLCD_1
 * @brief Prints "Hello world! :)" to the character LCD
 * 
 * Precondition:
 * @pre Character LCD is in a PIC socket
 */

#include <xc.h>
#include <stdio.h>
#include <pic18f4620.h> // This gives us access to standard print formatting functions
#include "configBits.h"

#define RS         LATDbits.LATD2 // Register select
#define E          LATDbits.LATD3 // Enable
#define LCD_DELAY  25 // Delay used to comply to LCD timing requirements

/****Distance Global Variables****/
float distance_travelled = 0; 
int counter = 0; 

/****ISR Variables****/
unsigned char input;

/**
 * @brief Low-level function to send bytes to the display
 * @param data The byte to be sent as a pair of nibbles (1/2 byte)
 */
void lcdNibble(char data){
    // Send the 4 most-significant bits
    char temp = data & 0xF0;
    LATD = LATD & 0x0F; // Clear LATD[7:4]
    LATD = temp | LATD; // Write data[7:4] to LATD[7:4]

    // These next 4 lines pulse the "enable" line of the character LCD, which 
    // causes its memory registers to latch the data we just wrote onto
    // LATD[7:4]
    E = 0;
    __delay_us(LCD_DELAY);
    E = 1;    
    __delay_us(100);
    
    // Send the 4 least-significant bits
    data = data << 4;
    temp = data & 0xF0;
    LATD = LATD & 0x0F;
    LATD = temp | LATD;
    E = 0;
    __delay_us(LCD_DELAY);
    E = 1;
    __delay_us(100);
}

/**
 * @brief Sends a command to a display control register
 * @param data The command byte for the Hitachi controller
 */
void lcdInst(char data) {
    RS = 0;
    lcdNibble(data);
}

/** @brief Performs the initial setup of the LCD */
void initLCD(void) {
    __delay_ms(15);
    // For reference, LCD instructions can be found in the controller datasheet
    // included with the sample code. Alternatively, it can be found here:
    // https://www.sparkfun.com/datasheets/LCD/HD44780.pdf (valid Dec 2018)
    lcdInst(0b00110011);
    lcdInst(0b00110010);
    lcdInst(0b00101000);
    lcdInst(0b00001111);
    lcdInst(0b00000110);
    lcdInst(0b00000001);
    __delay_ms(15);
}

/**
 * @brief Sends a character to the display for printing
 * @details The familiar C function printf internally calls a function named
 *          "putch" (put character) whenever a character is to be written to
 *          the screen. Here we have chosen to implement putch so that it
 *          sends the character to the LCD, but you can choose to implement it
 *          however you'd like (e.g. send the character over UART, etc.)
 * @param data The character (byte) to be displayed
 */
void putch(char data){
    RS = 1;
    lcdNibble(data);
    __delay_us(100);
}



void main(void) {    
    //Setting up interrupts
    INTCONbits.GIE = 1; // Enable interrupts 
    INTCONbits.PEIE = 1; //Enable peripherals
         
    
    // Set the data direction for all pins on port D to output
    TRISD = 0x00;
    
    //Disable Keypad 
    TRISAbits.RA2 = 0;
    LATAbits.LATA2 = 1; 
    
    //Setting up Rotary encoder 
//    INT0IE = 0;  // Disable RB0 interrupt (left wheel interrupt pin)
//    INT2IE = 1;  // Enable RB2 interrupt (right wheel interrupt pin)
//    TRISBbits.RB0 = 1; //Left wheel input
//    TRISBbits.RB2 = 1; //Right wheel input 
    TRISBbits.RB4 = 1;   //RB4 triggers on-change interrupt right now. 
    TRISBbits.RB5 = 0;   //Set all other pins to outputs so they don't trigger interrupts
    TRISBbits.RB6 = 0; 
    TRISBbits.RB7 = 0; 
    INTCONbits.RBIE = 1;  //Enable on-change interrupts
    
    initLCD();
//    printf("Hello!"); // Calls putch internally
//    while(1);
    while(1){
       lcdInst(0x01); 
       __delay_ms(5);
       printf("Count: %d",counter);
       __delay_ms(1000); 
    }
    
}

void __interrupt() interruptHandler(void){
//Check if right wheel rotary encoder interrupt enabled and flag raised 
    if(RBIE && RBIF){
        input = PORTB; 
        counter++; 
        RBIF = 0; //Clear interrupt flag 
    }

}