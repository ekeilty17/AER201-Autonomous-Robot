/**
 * @file
 * @author Nkemjika Okeke
 *
 * Created on February 1, 2019, 4:55 PM
 *
 * @ingroup RTC_I2C
 */

/********************************* Includes **********************************/
#include "RTC_I2C.h"
#include "configure.h"

/***************************** Private Functions *****************************/
/**
 * @brief Used to check if MSSP is currently processing something. If so, stays
 *        in while loop. 
 */
static inline void I2C_Master_Wait(){
    // Wait while:
    //   1. SSPSTAT is still working - 4 LSBs not zero 
    //          if bit2 = 1, transmit in progress
    //   2. Hardware has not cleared a sequence 
    //          bit6:4 are not zero 
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)){
        continue;
    }
}

/***************************** Public Functions ******************************/
void I2C_Master_Init(const unsigned long clockFreq){
    SSPCON1bits.SSPEN = 0;  // Disable MSSP so it can be reset
    
    // Force data and clock pins as inputs
    TRISCbits.TRISC3 = 1; // (SCL) clock pin
    TRISCbits.TRISC4 = 1;  // (SDA) data pin
    
  
    // Configuring PIC as master in I2C 
    // SSPADD = Baud Rate Generator reload value
    // Used Eqn on pg.172 of datasheet, solved for SSPADD
    SSPADD = (_XTAL_FREQ / (4 * clockFreq)) - 1;
    SSPADD = (_XTAL_FREQ / (4 * clockFreq)) - 1;
    
    SSPSTAT = 0b10000000; // Disable slew rate control bc freq = 100kHz

    SSPCON1 = 0b00101000;  // Enable serial port so SDA & SCL are configured
                          // Set 4 LSBs so MSSP in master mode 

    SSPCON2 = 0b00000000;  // Set I2C in idle 
}

void I2C_Master_Start(void){    
    I2C_Master_Wait(); // Wait until MSSP done with other stuff 
    SSPCON2bits.SEN = 1; // Initiate Start condition on SDA/SCL pins 
}

void I2C_Master_RepeatedStart(void){
    I2C_Master_Wait(); // Wait until MSSP done with other stuff 
    SSPCON2bits.RSEN = 1; // Initiate Repeated Start condition on SDA/SCL pins 
}

void I2C_Master_Stop(void){
    I2C_Master_Wait(); // Wait until MSSP done with other stuff 
    SSPCON2bits.PEN = 1; // Initiate Stop condition on SDA/SCL pins 
}

void I2C_Master_Write(unsigned byteToWrite){
    I2C_Master_Wait(); // Wait until MSSP done with other stuff 

    SSPBUF = byteToWrite; //Write to the SSPBUF register to transmit data 
}

unsigned char I2C_Master_Read(unsigned char ackBit){
    I2C_Master_Wait(); // Wait until MSSP done with other stuff 
    SSPCON2bits.RCEN = 1; // Enable receive mode for I2C module

    I2C_Master_Wait(); // Wait until MSSP done with other stuff

        // Read data received from SSPBUF
    unsigned char receivedByte = SSPBUF;

    I2C_Master_Wait(); // Wait until MSSP done with other stuff
    SSPCON2bits.ACKDT = ackBit; // Acknowledge data bit
    SSPCON2bits.ACKEN = 1; // Start acknowledge bit transmission sequence



    return receivedByte;
}