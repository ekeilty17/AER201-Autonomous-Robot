 /**
 * @file
 * @author Nkemjika Okeke
 * 
 * Date: February 23, 2019 2:46 PM
 * 
 * @brief Entire AutoCone12 Program - Arduino Nano Side
 * 
 * Preconditions:
 * @pre The Co-processor is not driving lines on the UART bus (e.g. the JP_TX
 *      and JP_RX jumpers are removed)
 * @pre PIC-Arduino link switches are enabled (ON) for D1 of the Arduino (the RX
 *      pin). However, make sure that neither of D0 and D1 are enabled (ON) 
 *      while programming the Arduino Nano
 */
// Includes
#include <SoftwareSerial.h>
#include "AerServo/AerServo.h"
#include "AerSteppers/AerSteppers.h"

void setup() {
  
}

void loop() {
  while (1);
}
