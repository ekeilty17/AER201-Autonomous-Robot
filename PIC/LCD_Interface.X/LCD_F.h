/**
 * @file
 * @author Nkemjika Okeke
 *
 * Created on February 1, 2019, 12:44 AM
 *
 * @defgroup LCDFunctions 
 * @brief Standby mode cycle, set the real-time clock and read back time 
 *        each second, then exit cycle when "A" key pressed 
 * @{
 */

#ifndef LCD_F_H
#define LCD_F_H

/************* Includes ************/
#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configure.h"

/************* Macros ************/
#define RS LATDbits.LATD2          
#define E  LATDbits.LATD3

/************************Public Function Prototypes**************************/

/** @brief Sends instructions into LCD controller
 * @param instructions to send into LCD controller
 */
void lcd_Instructions(char data);

void line_2(void);

void clear(void);

void cursor_reset(void);

void lcd_byte(char data); 


/** @brief Places LCD in 4-bit mode, so that nybbles of data can be sent in 
one at a time, and the LCD only starts reading once it has received 
2 nybbles (aka 8 bits).  */
void initialize_LCD(void);


/**
 * @brief Function called internal by printf to print a character on LCD screen 
 * @param char data to be printed
 */
void putch(char data);

/**
 * @}
 */

#endif	/* LCD_H */