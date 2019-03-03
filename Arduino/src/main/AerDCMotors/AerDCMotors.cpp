#include "Arduino.h"    // needed in order to call delay()
#include "AerDCMotors.h"

//Constructors
AerDCMotors :: AerDCMotors(){
}
AerDCMotors :: AerDCMotors(int pinL1, int pinL2, int pinR1, int pinR2){
    _pinL1 = pinL1;
    _pinL2 = pinL2;
    _pinR1 = pinR1;
    _pinR2 = pinR2;
}
void AerDCMotors :: init() {
    //Assigning pins
    pinMode(_pinL1,OUTPUT);
    pinMode(_pinL2,OUTPUT);
    pinMode(_pinR1,OUTPUT);
    pinMode(_pinR2,OUTPUT);
}

//Moving Wheels
void AerDCMotors :: left_wheel_forward() {
    digitalWrite(_pinL1,HIGH);
    digitalWrite(_pinL2,LOW);
}
void AerDCMotors :: right_wheel_forward() {
    digitalWrite(_pinR1,HIGH);
    digitalWrite(_pinR2,LOW);
}
void AerDCMotors :: left_wheel_backward() {
    digitalWrite(_pinL1,LOW);
    digitalWrite(_pinL2,HIGH);
}
void AerDCMotors :: right_wheel_backward() {
    digitalWrite(_pinR1,LOW);
    digitalWrite(_pinR2,HIGH);
}
void AerDCMotors :: left_wheel_slow() {
    
}
void AerDCMotors :: right_wheel_slow() {
    
}
void AerDCMotors :: left_wheel_stop() {
    digitalWrite(_pinL1,LOW);
    digitalWrite(_pinL2,LOW);
    /*
     digitalWrite(_pinL1,HIGH);
     digitalWrite(_pinL2,HIGH);
     */
}
void AerDCMotors :: right_wheel_stop() {
    digitalWrite(_pinR1,LOW);
    digitalWrite(_pinR2,LOW);
    /*
     digitalWrite(_pinR1,HIGH);
     digitalWrite(_pinR2,HIGH);
     */
}

//Moving Robot (simple)
void AerDCMotors :: forward() {
    this -> left_wheel_forward();
    this-> right_wheel_forward();
}
void AerDCMotors :: backward() {
    this -> left_wheel_backward();
    this -> right_wheel_backward();
}
void AerDCMotors :: slow() {
    this -> left_wheel_slow();
    this -> right_wheel_slow();
}
void AerDCMotors :: stop() {
    this -> left_wheel_stop();
    this -> right_wheel_stop();
}

//Moving Robot (complex)
void AerDCMotors :: pivot_right() {
    this -> left_wheel_forward();
    this-> right_wheel_backward();
}
void AerDCMotors :: pivot_left() {
    this -> left_wheel_backward();
    this-> right_wheel_forward();
}
void AerDCMotors :: u_turn_right() {
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
void AerDCMotors :: u_turn_left() {
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
