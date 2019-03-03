#include "Arduino.h"    // needed in order to call pinMode(), digitalWrite(), and delay()
#include "AerSteppers.h"

//Constructors
AerSteppers :: AerSteppers() {
}
AerSteppers :: AerSteppers(int pin1, int pin2, int pin3, int pin4) {
    _pin1 = pin1;
    _pin2 = pin2;
    _pin3 = pin3;
    _pin4 = pin4;
}
void AerSteppers :: init() {
    //Assigning pins
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
    pinMode(_pin3, OUTPUT);
    pinMode(_pin4, OUTPUT);
}

//Setters
void AerSteppers :: setTime_delay(int time_delay) {
    _time_delay = time_delay;
}

//Methods
void AerSteppers :: full_drive () {
    digitalWrite(_pin1,LOW);
    digitalWrite(_pin2,LOW);
    digitalWrite(_pin3,HIGH);
    digitalWrite(_pin4,HIGH);
    delay(3);
    digitalWrite(_pin1,LOW);
    digitalWrite(_pin2,HIGH);
    digitalWrite(_pin3,HIGH);
    digitalWrite(_pin4,LOW);
    delay(3);
    digitalWrite(_pin1,HIGH);
    digitalWrite(_pin2,HIGH);
    digitalWrite(_pin3,LOW);
    digitalWrite(_pin4,LOW);
    delay(3);
    digitalWrite(_pin1,HIGH);
    digitalWrite(_pin2,LOW);
    digitalWrite(_pin3,LOW);
    digitalWrite(_pin4,HIGH);
    delay(3);
    digitalWrite(_pin1,LOW);
    digitalWrite(_pin2,LOW);
    digitalWrite(_pin3,HIGH);
    digitalWrite(_pin4,HIGH);
    delay(3);
}
  
void AerSteppers :: drop_cone() {
    
    for(int i=0; i<64; i++) {
      full_drive();
      // slows down during period where cone drops
      if ((i > 40) && (i < 50)) {
        delay(_time_delay);
      }
    }

    digitalWrite(_pin1,LOW);
    digitalWrite(_pin2,LOW);
    digitalWrite(_pin3,LOW);
    digitalWrite(_pin4,LOW);

    delay(1000);
}
