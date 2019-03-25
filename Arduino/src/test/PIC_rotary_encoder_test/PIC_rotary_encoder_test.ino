#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

//Read-only (Arduino-PIC Comm)
const byte rxPin = 1;
const byte txPin = 0;
const int rs = 4, en = 5, d4 = 6, d5 = 7, d6 = 8, d7 = 9;

//Global variables for primary initializing 
volatile unsigned char curr_pos = 0;

//Arduino-PIC Comm
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

//Display Arduino stuff
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // Set up the LCD's number of columns and rows
  lcd.begin(16, 4);
  
  // Configure pin modes for tx and rx
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  
  // Open software serial port with baud rate = 9600
  mySerial.begin(9600);
}

void loop() {

  while(1) {
    if (mySerial.available() > 0) {
       
       curr_pos = curr_pos ^ mySerial.read();

       if (curr_pos == 0) {
        mySerial.write('A'); 
       } else {
        mySerial.write('B'); 
       }
    }
    delay(1000);
  }
  
}
