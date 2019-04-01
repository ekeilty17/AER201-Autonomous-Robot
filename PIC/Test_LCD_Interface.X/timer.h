/**
 * @file
 * @author Nkemjika Okeke
 *
 * Created on February 3, 2019, 12:44 AM
 *
 * @defgroup TimerFunctions
 * @brief Functions required to use RTC outputs as a timer 
 * @{
 */

#ifndef TIMER_H
#define TIMER_H

/************* Includes ************/
#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configure.h"

/**
 * @brief Converts char character to the integer value of the char's ASCII hex value  
 * @param char character of hex number
 */
int atohtoi(unsigned char ascii); 

int char_to_int_2dig(char first, char second); 

float char_to_float_4dig(char first, char second, char third, char fourth); 

int time_elapsed(unsigned char begin_min, unsigned char begin_sec,unsigned char end_min,unsigned char end_sec, int*array); 


#endif	/* TIMER_H */