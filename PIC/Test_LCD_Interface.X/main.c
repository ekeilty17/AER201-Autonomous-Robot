/**
 * @file
 * @author Nkemjika Okeke
 * 
 * Created on March 22 3:04 PM
 * 
 */

#include <xc.h>
#include <stdio.h>
#include "configure.h"
#include "RTC_I2C.h"
#include "lcd.h"
#include <pic18f4620.h>
#include "timer.h"
#include <stdlib.h>


/***Detection Definitions***/
#define Hole_Light LATAbits.LATA0
#define Crack_Light LATAbits.LATA1

/**Keypad Disable Definitions*/
#define Keypad_Disable LATAbits.LATA2

/****Distance Global Variables****/
float distance_travelled = 0; 
int counter = 0; 

/****ISR Variables****/
volatile unsigned char input;

/******Op Data Variables*****/
volatile int data_count = 0; //Use this to keep track of what data count we're on
volatile int data_count2 = 0; 
volatile char total_cones[3];
volatile int  total_cones_deployed = 0; 
volatile char total_cracks[3];
volatile int total_cracks_int = 0;
volatile char total_holes[3];
volatile int total_holes_int = 0; 
volatile char crack_dist[50][6];
volatile int crack_dist_n = -1;
volatile char hole_dist[50][6]; 
volatile int hole_dist_n = -1;
volatile char dist_1;
volatile char dist_2;
volatile char indicator = ')'; 


/****Keypad Interface Variables****/
const char keys[] = "123A456B789C*0#D";
char first_digit = ')';
char second_digit = ')';
int total_cones_input = 0;
unsigned char keypress;
int start_sequence = 100; 

/****Arduino Status Variables*****/
volatile unsigned char status = 'N'; 

const char reset[7] = {
    0x20, // Second
    0x38, // Minute
    0x00, // 24h Hour
    0x01, // Weekday
    0x24, // Day
    0x03, // Month
    0x19  // Year
};

void reset_time(void);
void Light_Hole(void); 
void Light_Crack(void);

volatile int key_pressed = 0;

void main(void) {
    //Setting up LCD
       //RD2 = Register Select 
       //RD3 = Enable
       //RD4-RD7 = Data bytes 
    LATD = 0x00;
    TRISD = 0x00;
    initLCD();

    //Setting up RTC
    I2C_Master_Init(100000);  //Set I2C Master with 100 kHz clock based on datasheet
    unsigned char time[7]; // Create a byte array to hold time read from RTC
//    reset_time();          //Reset RTC time
    
    //Reset cursor pre-printing on LCD
    lcd_home();
    
    /*****Setting up Arduino-PIC communication - Start***/
    
    // Configure the baud rate generator for 9600 bits per second
    long baudRate = 9600;
    SPBRG = (unsigned char)((_XTAL_FREQ / (64 * baudRate)) - 1);
    
    // Configure transmit control register
    TXSTAbits.TX9 = 0; // Use 8-bit transmission (8 data bits, no parity bit)
    TXSTAbits.SYNC = 0; // Asynchronous communication
    TXSTAbits.TXEN = 1; // Enable transmitter
    __delay_ms(5); // Enabling the transmitter requires a few CPU cycles for stability
    
    // Configure receive control register
    RCSTAbits.RX9 = 0; // Use 8-bit reception (8 data bits, no parity bit)
    RCSTAbits.CREN = 1; // Enable receiver
    
    // Enforce correct pin configuration for relevant TRISCx
    TRISCbits.TRISC6 = 0; // TX = output
    TRISCbits.TRISC7 = 1; // RX = input
   
    // Enable serial peripheral
    RCSTAbits.SPEN = 1;

    // Set enable bit to get interrupts
    PIE1bits.RCIE = 1;  
    
    // Set receive as high priority to override rotary encoders
    IPR1bits.RCIP = 1;
    
    /*****Setting up Arduino-PIC communication - End***/
    
    //Setting up lights for Detection System
    TRISAbits.RA0 = 0; 
    TRISAbits.RA1 = 0; 
    Hole_Light = 0;
    Crack_Light = 0; 
    
    //Setting up interrupts
    INTCONbits.GIE = 1; // Enable interrupts 
    INTCONbits.PEIE = 1; //Enable peripherals 
    
    //Setting up Keypad
    INTCON3bits.INT1IE = 1;  // Enable RB1 interrupt (keypad pin)
    
    TRISAbits.RA2 = 0;      //KPD pin output
    Keypad_Disable = 0; 
   
    //For Operation Data Printing
    int i; 
    int break_while; 
    
    while (1){
        //RTC Loop
        while(1){
            // Reset RTC memory pointer
            I2C_Master_Start(); // Start condition
            I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
            I2C_Master_Write(0x00); // Set memory pointer to seconds
            I2C_Master_Stop(); // Stop condition

            // Read current time
            I2C_Master_Start(); // Start condition
            I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
            for(unsigned char i = 0; i < 6; i++){
                time[i] = I2C_Master_Read(ACK); // Read with ACK=0 to continue reading
            }

            //time[i] are all "random" chars, timing is really done in hex values
            //So must print and manipulate the hex values of the chars 

            time[6] = I2C_Master_Read(NACK); // Read with NACK=1 to stop reading 
            I2C_Master_Stop(); // Stop condition

            // Print time on LCD
            lcd_home();
            printf("%02x/%02x/%02x", time[4],time[5],time[6]); // DD/MM/YY
            lcd_set_ddram_addr(LCD_LINE2_ADDR); 
            printf("%02x:%02x:%02x", time[2],time[1],time[0]); // HH:MM:SS
            __delay_ms(1000); //update printf output every second
            if (key_pressed){   //Start sequence 
                key_pressed = 0;
                    break; 
            }
        } 
    lcd_clear();      //Clear LCD screen, exited standby-mode 
   
    //Getting total # of cones data 
    printf("Total Cones:"); 
    while(1){
        if(key_pressed){
            key_pressed = 0; // Clear the flag

            // Write key press data to bottom line of LCD
            keypress = (PORTB & 0xF0) >> 4;

            if (keys[keypress] == 'A'){      //Enter # of cones, then press A to start
                    break;
            }
            if (first_digit == ')'){
                first_digit = keys[keypress]; 
            }
            if (first_digit != ')'){
                second_digit = keys[keypress];
            }
            printf("%c",keys[keypress]);
        }
    }
    total_cones_input = char_to_int_2dig(first_digit, second_digit);
    __delay_ms(1); 
    
        
    //Send Total Cones Data to Arduino
    
    // Wait until the previous TXREG data has completed its transfer 
    // into the TSR and the TSR has finished transmitting all bits 
    // before loading the TXREG register
    while(!TXIF | !TRMT){
        continue;
    }
    // Load data into the transmit register, TXREG
    TXREG = total_cones_input;

    __delay_ms(1000);
        
    lcd_clear();
    
    // Read current time for Operation Start 
    // Reset RTC memory pointer
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    I2C_Master_Stop(); 

    I2C_Master_Start();
    I2C_Master_Write(0b11010001); 
    unsigned char begin_sec = I2C_Master_Read(ACK); 
    unsigned char begin_min = I2C_Master_Read(NACK); 
    unsigned char * temp = &begin_sec; 
    I2C_Master_Stop(); 
    
    /****Signal Arduino Nano to Start*/
    // Wait until the previous TXREG data has completed its transfer 
    // into the TSR and the TSR has finished transmitting all bits 
    // before loading the TXREG register
    while(!TXIF | !TRMT){
        continue;
    }
    // Load data into the transmit register, TXREG, 'A' means start/stop
    TXREG = start_sequence;
    __delay_ms(1000);   //Wait for processing 
    
    //Disable Keypad pins and Enable Rotary encoder sequence 
    //Setting up Rotary encoder 
    TRISBbits.RB5 = 1;   //Set all other pins to outputs so they don't trigger interrupts
    Keypad_Disable = 1;   //Disable keypad pins 
    INTCONbits.RBIE = 1;  //Enable on-change pins 
    
    lcd_clear(); 

    while(1){
        
        /****Send Arduino Nano total distance travelled data */
        // Wait until the previous TXREG data has completed its transfer 
        // into the TSR and the TSR has finished transmitting all bits 
        // before loading the TXREG register
        while(!TXIF | !TRMT){
            continue;
        }
        // Load total distance travelled data to register
        TXREG = counter;
        
        //Check if Arduino Nano has signaled PIC to stop
        if (status == 'A'){
            break; 
        }
        
    }

    // Read current time for Operation Stop
    // Reset RTC memory pointer
    I2C_Master_Start(); 
    I2C_Master_Write(0b11010000);
    I2C_Master_Write(0x00);
    I2C_Master_Stop(); 

    I2C_Master_Start();
    I2C_Master_Write(0b11010001); 
    unsigned char end_sec = I2C_Master_Read(ACK); 
    unsigned char end_min = I2C_Master_Read(NACK); 
    I2C_Master_Stop();  
    
    int op_time[2];   //op_time[1] = min, op_time[0] = seconds 
    time_elapsed(begin_min, begin_sec, end_min, end_sec, op_time);
    
    
    //Enable Keypad pins 
    Keypad_Disable = 0; 
    
    //Termination Message 
    while(1){ 
    lcd_clear();
    printf("Operation");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Complete");
    __delay_ms(2000); 
            if (key_pressed){
               key_pressed = 0;  //turn flag off 
                break;
            }
    }
    
    //Operation Data Loop
    while(1){
        lcd_clear(); 
//           printf("Time: %02d:%02d", op_time[1],op_time[0]);  //op_time[1] = min, op_time[0] = seconds
           printf("Time: N/A");  //op_time[1] = min, op_time[0] = seconds
           lcd_set_ddram_addr(LCD_LINE2_ADDR);
           printf("Cones: "); 
           printf(total_cones); 
           if (key_pressed){
               key_pressed = 0;
               break;
           }
           __delay_ms(2000);
           lcd_clear();
           printf("Cracks: "); 
           printf(total_cracks);
           lcd_set_ddram_addr(LCD_LINE2_ADDR);
           printf("Holes: ");
           printf(total_holes);
           if (key_pressed){
              key_pressed = 0;
               break;
           }
           __delay_ms(2000);
           for (i=0; i<=crack_dist_n; i++){
               lcd_clear();
               printf("Crack %02d: ",i);
               printf(crack_dist[i]);
               //lcd_set_ddram_addr(LCD_LINE2_ADDR);
               if (key_pressed){
                key_pressed = 0;
                break_while = 1;
                   break;
               }
               __delay_ms(2000);
           }
           if (break_while){
               break_while = 0; 
               break;
           }
           for (i=0;i<=hole_dist_n; i++){
               lcd_clear();
               printf("Hole %02d: ", i); 
               printf(hole_dist[i]);
               
               if (key_pressed){
               key_pressed = 0;
               break_while = 1;
                  break;
               }
               __delay_ms(2000);
           }
           if (break_while){
               break_while = 0; 
               break;
           }
        }

    }
}

/** @brief Writes the reset array to the RTC memory */
void reset_time(void){
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); //7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    
    // Write array
    for(char i=0; i < 7; i++){
        I2C_Master_Write(reset[i]);
    }
    
    I2C_Master_Stop(); //Stop condition
}

/**
 * @brief Function called when interrupt generated, sends you to ISRs 
 */
void __interrupt() interruptHandler(void){
    INTCONbits.GIE = 0; // Disable interrupts 
    
    // Check if there's an RB1
    // INT1IE = Check if interrupt enabled, INT1IF = Check if flag raised 
    if(INT1IE && INT1IF){
        key_pressed = 1;
        INT1IF = 0; // Clear interrupt flag 
    }
    
    //Check if rotary encoder interrupt enabled and flag raised 
    if(RBIE && RBIF){
        input = PORTB; 
        counter++; 
        RBIF = 0; //Clear rotary encoder interrupt flag 
    }
    
    //Check if arduino nano sent the PIC data 
    if(PIE1bits.RCIE && PIR1bits.RCIF){
        if(RCSTAbits.OERR){ // check for Error 
            RCSTAbits.CREN = 0; //If error -> Reset 
            RCSTAbits.CREN = 1; //If error -> Reset 
        }
        else if (RCREG == 'A'){
            status = 'A'; 
        }
//          Check if Arduino Nano has signaled PIC to turn on Detection System
        else if (RCREG == 'H'){            //  H = Turn on light for hole
            Light_Hole(); 
        }
        else if (RCREG == 'C'){            //  C = Turn on light for crack
            Light_Crack();  
        }
        else if (RCREG == 'R') {
            //Rotary Encoder Data
            indicator = 'R';
        }
        else if (RCREG == '*') {
            //Number of holes
            indicator = '*';
        }
        else if (RCREG == '^') {
           //Number cones deployed 
            indicator = '^';
        }
        else if (RCREG == '_') {
           //Number of cracks 
           indicator = '_';
        }
        else if (RCREG == 'c') {
           //Crack distance 
           indicator = 'c';
        }
        else if (RCREG == 'h') {
           //Holes distance 
           indicator = 'h'; 
        }
        else{
            if (indicator == 'R') {
                // This should happen somewhere else
                int RE;
                RE_left = RE >> 8;
                RE_right = RE & 0x0000ffff;
                if (data_count == 0) {
                    TXREG = RE_left;
                    data_count++;
                } else {
                    TXREG = RE_right;
                    data_count = 0;
                }
            }
            else if (indicator == '^'){
                total_cones[data_count] = RCREG; 
                if (data_count == 2) {
                    data_count = -1; 
                }
                data_count++;
                /****Tell Arduino done processing****/
                while(!TXIF | !TRMT){
                    continue;
                }
                // Load total distance travelled data to register
                TXREG = 'D';
            }
            else if (indicator == '_'){
                total_cracks[data_count] = RCREG; 
                if (data_count == 2) {
                    data_count = -1; 
                }
                data_count++;
                /****Tell Arduino done processing****/
                while(!TXIF | !TRMT){
                    continue;
                }
                // Load total distance travelled data to register
                TXREG = 'D';
            }
            else if (indicator == '*'){
                total_holes[data_count] = RCREG; 
                if (data_count == 2) {
                    data_count = -1; 
                }
                data_count++;
                /****Tell Arduino done processing****/
                while(!TXIF | !TRMT){
                    continue;
                }
                // Load total distance travelled data to register
                TXREG = 'D';
            }
            else if (indicator == 'c'){
                
                if (data_count == 0) {
                    crack_dist_n++;
                }
                
                crack_dist[crack_dist_n][data_count] = RCREG;
                data_count++;
                data_count %= 6;
                
                /****Tell Arduino done processing****/
                while(!TXIF | !TRMT){
                    continue;
                }
                // Load total distance travelled data to register
                TXREG = 'D';
            }
            else if (indicator == 'h'){
                if (data_count2 == 0) {
                    hole_dist_n++;
                }
                
                hole_dist[hole_dist_n][data_count2] = RCREG;
                data_count2++;
                data_count2 %= 6;
                
                /****Tell Arduino done processing****/
                while(!TXIF | !TRMT){
                    continue;
                }
                // Load total distance travelled data to register
                TXREG = 'D'; 
            }
        }
//            else if (data_count == total_cracks_int+total_holes_int+6){
//                //cracks and holes distance calculations here 
//                data_count = 0; 
//            }
        PIR1bits.RCIF = 0; //Clear interrupt flag  
    }
    INTCONbits.GIE = 1; // Enable interrupts 
}

void Light_Hole(void){
    Hole_Light = 1; 
    __delay_ms(1000);
    Hole_Light = 0;
    return; 
}

void Light_Crack(void){
    Crack_Light = 1; 
    __delay_ms(1000);
    Crack_Light = 0;
    return; 
}