#include "Arduino.h"    // needed in order to call pinMode(), digitalWrite(), and delay()

class AerSteppers {
  //Internal variables
  private:
    int _pin1;
    int _pin2;
    int _pin3;
    int _pin4;

    int _time_delay = 0;

  public:
    //Constructors
    AerSteppers() {
    }
    AerSteppers(int pin1, int pin2, int pin3, int pin4) {
      _pin1 = pin1;
      _pin2 = pin2;
      _pin3 = pin3;
      _pin4 = pin4;
    }
    void init() {
      //Assigning pins
      pinMode(_pin1, OUTPUT); 
      pinMode(_pin2, OUTPUT); 
      pinMode(_pin3, OUTPUT); 
      pinMode(_pin4, OUTPUT); 
    }
  
    //Methods
    void full_drive (){
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
    
    void drop_cone() {

      for(int i=0; i<64; i++) {
          full_drive();
          if ((i > 40) && (i < 50)) {
            delay(_time_delay);
          }
      }
      

      /*
      for(int i=0;i<64;i++){    // 64 Steps = 45º turn 
          full_drive();
      }
      */
      
      digitalWrite(_pin1,LOW);
      digitalWrite(_pin2,LOW);
      digitalWrite(_pin3,LOW);
      digitalWrite(_pin4,LOW);

      delay(1000);
    }
    
};
