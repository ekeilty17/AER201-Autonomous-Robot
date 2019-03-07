/**
 * @file
 * @author Nkemjika Okeke
 *
 * Created February 1, 2019 5:40PM
 *
 * @defgroup RTC_I2C
 * @{
 */
#ifndef I2C_H
#define I2C_H

/********************************* Includes **********************************/
#include <xc.h>
#include "configure.h"

/********************************** Macros ***********************************/
// These mean different things depending on the context, see "Understanding the
// I2C bus" by Texas Instruments for more details
#define ACK  0 /**< Acknowledge     */
#define NACK 1 /**< Not acknowledge */


/************************ Public Function Prototypes *************************/
/**
 * @brief Initializes the MSSP module for I2C master mode. 
 * @param clockFreq - The frequency at which data to be transferred via the
 *        I2C bus
 */
void I2C_Master_Init(const unsigned long clockFreq);

/**
 * @brief Initiates Start condition on SDA and SCL pins
 */
void I2C_Master_Start(void);

/**
 * @brief Initiates Repeated Start condition on SDA and SCL pins
 */
void I2C_Master_RepeatedStart(void);

/**
 * @brief Initiates Stop condition on SDA and SCL pins
 */
void I2C_Master_Stop(void);

/** @brief Writes a byte to RTC */
void I2C_Master_Write(unsigned byteToWrite);

/**
 * @brief Reads a byte from RTC
 * @param acknowBit The acknowledge bit
 *        -acknowBit = 0 --> acknowledge bit sent, keep reading
 *        -acknowBit = 1 --> no acknowledge bit, stop reading
 * @return data received 
 */
unsigned char I2C_Master_Read(unsigned char ackBit);

/**
 * @}
 */

#endif /* I2C_H */