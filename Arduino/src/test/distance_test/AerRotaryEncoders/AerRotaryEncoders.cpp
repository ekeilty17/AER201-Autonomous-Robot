#include "Arduino.h"
#include <SoftwareSerial.h>
#include "AerRotaryEncoders.h"

AerRotaryEncoders :: AerRotaryEncoders() {
}
AerRotaryEncoders :: AerRotaryEncoders(int pinL, int pinR) {
  _pinL = pinL;
  _pinR = pinR;
}
void AerRotaryEncoders :: init() {
  distance_travelled = 0.0;
  pinMode(_pinL, INPUT);
  pinMode(_pinR, INPUT);
  attachInterrupt(digitalPinToInterrupt(_pinL), Distance_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_pinR), Distance_ISR, CHANGE);
  Serial.begin(9600); 
}

void AerRotaryEncoders :: Distance() {
  //Check if right wheel rotary encoder interrupt enabled and flag raised 
  if(digitalRead(_pinL)){
      distance_travelled += 1.06103295395;
  }
  //Check if left wheel rotary encoder interrupt enabled and flag raised 
  else if(digitalRead(_pinR)){
     distance_travelled += 1.06103295395;
  }
}

void Distance_ISR() {
    //...
}
