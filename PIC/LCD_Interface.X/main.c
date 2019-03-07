/**
 * @file
 * @author Nkemjika Okeke
 * 
 * Created on January 31, 2019, 10:31 PM
 * 
 * @defgroup LCD_Interface
 * @brief Standby mode cycle, set the real-time clock and read back time 
 *        each second, then exit cycle when "A" key pressed 
 * 
 * Preconditions:
 * @pre JP6, and JP7 in the RTC module shorted
 * @pre Check if RTC chip properly inserted 
 */


#include <xc.h>
#include <stdio.h>
#include <pic18f4620.h>
#include "RTC_I2C.h"
#include "LCD_F.h"
#include "configure.h"
#include "timer.h"

/*******Rotary Encoder Definitions*****/
#define Left_Wheel_Enc PORTBbits.RB0
#define Right_Wheel_Enc PORTBbits.RB2

/***Detection Definitions***/
#define Hole_Light LATAbits.LATA0
#define Crack_Light LATAbits.LATA1

/**Keypad Disable Definitions*/
#define Keypad_Disable LATAbits.LATA2

/****Distance Global Variables****/
float distance_travelled = 0; 

/******Op Data Variables*****/
int data_count = 0;      //Use this to keep track of what data count we're on
int total_cones = 0;
int total_cracks = 0;
int total_holes = 0;
float crack_dist[50];
float hole_dist[50]; 

const char reset[7] = {
    0x00, // Second
    0x46, // Minute
    0x11, // 24h Hour
    0x01, // Weekday
    0x25, // Day
    0x02, // Month
    0x19  // Year
};

void reset_time(void);

volatile int key_pressed = 0;

void main(void) {
    //Setting up LCD
       //RD2 = Register Select 
       //RD3 = Enable
       //RD4-RD7 = Data bytes 
    LATD = 0x00;
    TRISD = 0x00;
    initialize_LCD();

    //Setting up RTC
    I2C_Master_Init(100000);  //Set I2C Master with 100 kHz clock based on datasheet
    unsigned char time[7]; // Create a byte array to hold time read from RTC
//    reset_time();          //Reset RTC time
    
    //Reset cursor pre-printing on LCD
    cursor_reset();
    
    /*****Setting up Arduino-PIC communication - Start***/
    unsigned char data; // Holds the data to be sent/received
    /*****Setting up Arduino-PIC communication - End***/
    
    //Setting up lights for Detection System
    TRISAbits.RA0 = 0;     //Hole Light output
    TRISAbits.RA1 = 0;     //Crack Light output 
    Hole_Light = 0;
    Crack_Light = 0; 
    
    //Setting up interrupts
    INTCONbits.GIE = 1; // Enable interrupts 
    INTCONbits.PEIE = 1; //Enable peripherals 
    
    //Setting up Keypad
    INTCON3bits.INT1IE = 1;  // Enable RB1 interrupt (keypad pin)
    
    //Setting up Rotary encoder 
    TRISAbits.RA2 = 0;      //KPD pin output
    
   
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
            clear();
            printf("%02x/%02x/%02x", time[4],time[5],time[6]); // DD/MM/YY
            line_2(); 
            printf("%02x:%02x:%02x", time[2],time[1],time[0]); // HH:MM:SS
            __delay_ms(1000); //update printf output every second
            if (key_pressed){
                key_pressed = 0;
                    break; 
            }
        } 
    clear();      //Clear LCD screen, exited standby-mode 
    
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
    I2C_Master_Start();
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write('A');       // Write 'A' to  Arduino to start 
    I2C_Master_Stop();
    
    //Disable Keypad pins and Enable Rotary encoder sequence 
    Keypad_Disable = 1;   //Disable keypad pins 
    INTCONbits.RBIE = 1;  //Enable on-change pins 

    while(1){
        /****Send Arduino Nano total distance travelled data */
        I2C_Master_Start();
        I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
        I2C_Master_Write(distance_travelled);  // Load total distance travelled data to register 
        I2C_Master_Stop();
        
        /****Read from Arduino Nano*****/
        I2C_Master_Start();
        I2C_Master_Write(0b00010001); // 7-bit Arduino slave address + Read
        data = I2C_Master_Read(NACK); // Read 
        I2C_Master_Stop();
        
        //Check if Arduino Nano has signaled PIC to turn on Detection System
        if (data == 'H'){            //  H = Turn on light for hole
            Hole_Light = 1; 
            __delay_ms(1000);
            Hole_Light = 0;
        }
        if (data == 'C'){            //  C = Turn on light for crack
            Crack_Light = 1;
            __delay_ms(1000);
            Crack_Light = 1; 
        }
        
        //Check if Arduino Nano has signaled PIC to stop
        if (data == 'A'){            // A = Stop
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
    
    //Receive operation data from Arduino 
    while(1){
        /****Read from Arduino Nano*****/
        I2C_Master_Start();
        I2C_Master_Write(0b00010001); // 7-bit Arduino slave address + Read
        data = I2C_Master_Read(NACK); // Read 
        I2C_Master_Stop();
        if (data_count == 0){
            total_cones = data; 
            data_count++;
        }
        else if (data_count == 1){
            total_cracks = data; 
            data_count++;
        }
        else if (data_count == 2){
            total_holes = data; 
            data_count++;
        }
        else if (data_count >= 3 && data_count < (total_cracks+3)){
            crack_dist[data_count-3] = data; 
            data_count++; 
        }
        else if (data_count >= (3+total_cracks) && data_count < (total_cracks+total_holes+3)){
            hole_dist[data_count-(3+total_cracks)] = data; 
            data_count++; 
        }
        else if (data_count >= (total_cracks+total_holes+3)){
            break; 
        }
    }
    
    //Enable Keypad pins 
    Keypad_Disable = 0; 
    
    //Termination Message 
    while(1){ 
    clear();
    printf("Operation");
    line_2();
    printf("Complete");
    __delay_ms(2000); 
            if (key_pressed){
               key_pressed = 0;  //turn flag off 
                break;
            }
    }
    
    //Operation Data Loop
    while(1){
        clear(); 
           printf("Time: %02d:%02d", op_time[1],op_time[0]);  //op_time[1] = min, op_time[0] = seconds 
           line_2();
           printf("Cones: %d", total_cones); 
           if (key_pressed){
               key_pressed = 0;
               break;
           }
           __delay_ms(2000);
           clear(); 
           printf("Cracks: %d", total_cracks);
           line_2();
           printf("Holes: %d", total_holes); 
           if (key_pressed){
              key_pressed = 0;
               break;
           }
           __delay_ms(2000);
           for (i=0;i<total_cracks; i+=2){
               clear();
               printf("Crack %d: %fcm",i,crack_dist[i]);
               line_2();
               printf("Crack %d: %fcm",i+1,crack_dist[i+1]);  
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
           for (i=0;i<total_holes; i+=2){
               clear();
               printf("Hole %d: %fcm", i, hole_dist[i]);
               line_2();; 
               printf("Hole %d: %fcm", i+1, hole_dist[i+1]);  
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
    //Check if right wheel rotary encoder interrupt enabled and flag raised 
    if(INT2IE && INT2IF){
        distance_travelled = distance_travelled + 1.06103295395; 
        INT2IF = 0; //Clear interrupt flag 
    }
    //Check if left wheel rotary encoder interrupt enabled and flag raised 
    if(INT0IE && INT0IF){
       distance_travelled = distance_travelled + 1.06103295395; 
       INT0IF = 0; //Clear interrupt flag 
    }
    INTCONbits.GIE = 1; // Enable interrupts 
}

