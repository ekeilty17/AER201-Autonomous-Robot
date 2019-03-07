/**
 * @file
 * @author Nkemjika Okeke
 *
 * Created on February 1, 2016, 9:27 AM
 * @ingroup LCDFunctions 
 */

#include "LCD_F.h"

/**
 * @brief Regular function that sends a byte of data into LCD instructions
 * @param Byte of data to be sent
 * @note  Function sends in 2 pulses of nybbles 
 */
void lcd_byte(char data){
    // Sending 4 MSBs 
    char temp = data & 0xF0;  //Clear bottom LSBs of data
    LATD = LATD & 0x0F; // Clear LATD[7:4]
    LATD = temp | LATD; // Write data[7:4] to LATD[7:4]

    //Pulse E so LCD can latch LATD data
    E = 0;
    __delay_us(25);
    E = 1;    
    __delay_us(100);
    
    // Send 4 LSBs
    data = data << 4;  
    temp = data & 0xF0;     //shift data up and clear bottom LSBs 
    LATD = LATD & 0x0F;     //Clear LATD[7:4]
    LATD = temp | LATD;     //Write data[7:4] to LATD
    
    //Pulse E so LCD can latch LATD data
    E = 0;
    __delay_us(25);
    E = 1;
    __delay_us(100);
}

/** @brief Sends instructions into LCD controller
 * @param instructions to send into LCD controller
 */
void lcd_Instructions(char data) {
    RS = 0;
    lcd_byte(data);
}

/**
 * @brief Puts the printf result on 2nd line
 * @param address of new line 
 */
void line_2 (void){
    lcd_Instructions(0x80 | 64);
}

/**
 * @brief Clears LCD
 */
void clear(void){
    lcd_Instructions(0x01);
	__delay_ms(5);
}

/**
 * @brief Puts lcd cursor back at start
 */
void cursor_reset(void){
    lcd_Instructions(0x80);
}

/** @brief Places LCD in 4-bit mode, so that nybbles of data can be sent in 
one at a time, and the LCD only starts reading once it has received 
2 nybbles (aka 8 bits).  */
void initialize_LCD(void) {
    __delay_ms(15);
    // For reference, LCD instructions can be found in the controller datasheet
    // included with the sample code. Alternatively, it can be found here:
    // https://www.sparkfun.com/datasheets/LCD/HD44780.pdf (valid Dec 2018)
    lcd_Instructions(0b00110011);
    lcd_Instructions(0b00110010);
    lcd_Instructions(0b00101000);
    lcd_Instructions(0b00001100);
    lcd_Instructions(0b00000110);
    lcd_Instructions(0b00000001);
    __delay_ms(15);
}

/**
 * @brief Function called internal by printf to print a character on LCD screen 
 * @param char data to be printed
 */
void putch(char data){
    RS = 1;
    lcd_byte(data);
    __delay_us(100);
}
