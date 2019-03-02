#include "Arduino.h"    // needed in order to call delay()

class AerDCMotors{
  private:
    int _pinL1;
    int _pinL2;
    int _pinR1;
    int _pinR2;
  
  public:
    //Constructors
    AerDCMotors(){
    }
    AerDCMotors(int pinL1, int pinL2, int pinR1, int pinR2){
      _pinL1 = pinL1;
      _pinL2 = pinL2;
      _pinR1 = pinR1;
      _pinR2 = pinR2;
    }
    void init() {
      //Assigning pins
      pinMode(_pinL1,OUTPUT); 
      pinMode(_pinL2,OUTPUT); 
      pinMode(_pinR1,OUTPUT); 
      pinMode(_pinR2,OUTPUT);
    }
  
    //Moving Wheels
    void left_wheel_forward() {
      digitalWrite(_pinL1,HIGH);
      digitalWrite(_pinL2,LOW);
    }
    void right_wheel_forward() {
      digitalWrite(_pinR1,HIGH);
      digitalWrite(_pinR2,LOW);
    }
    void left_wheel_backward() {
      digitalWrite(_pinL1,LOW);
      digitalWrite(_pinL2,HIGH);
    }
    void right_wheel_backward() {
      digitalWrite(_pinR1,LOW);
      digitalWrite(_pinR2,HIGH);
    }
    void left_wheel_stop() {
      digitalWrite(_pinL1,LOW);
      digitalWrite(_pinL2,LOW);
      /*
      digitalWrite(_pinL1,HIGH);
      digitalWrite(_pinL2,HIGH);
      */
    }
    void right_wheel_stop() {
      digitalWrite(_pinR1,LOW);
      digitalWrite(_pinR2,LOW);
      /*
      digitalWrite(_pinR1,HIGH);
      digitalWrite(_pinR2,HIGH);
      */
    }

    //Moving Robot (simple)
    void forward() {
      this -> left_wheel_forward();
      this-> right_wheel_forward();
    }
    void backward() {
      this -> left_wheel_backward();
      this -> right_wheel_backward();
    }
    void stop() {
      this -> left_wheel_stop();
      this -> right_wheel_stop();
    }

    //Moving Robot (complex)
    void pivot_right() {
      this -> left_wheel_forward();
      this-> right_wheel_backward();
    }
    void pivot_left() {
      this -> left_wheel_backward();
      this-> right_wheel_forward();
    }
    void u_turn_right() {
      this -> pivot_right();
      delay(0000);
      this -> stop();
      delay(000);
      this -> forward();
      delay(0000);
      this -> stop();
      delay(000);
      this -> pivot_right();
      delay(0000);
      this -> stop();
      delay(000);
    }
    void u_turn_left() {
      this -> pivot_left();
      delay(0000);
      this -> stop();
      delay(000);
      this -> forward();
      delay(0000);
      this -> stop();
      delay(000);
      this -> pivot_left();
      delay(0000);
      this -> stop();
      delay(000);
    }
};
