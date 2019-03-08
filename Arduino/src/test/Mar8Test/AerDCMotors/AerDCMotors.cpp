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
AerDCMotors :: AerDCMotors(int pinL1, int pinL2, int pwmL, int pwmR){
    _pinL1 = pinL1;
    _pinL2 = pinL2;
    _pwmL = pwmL;
    _pwmR = pwmR;
}

#define _pinR1 A4
#define _pinR2 A5

void AerDCMotors :: init() {
    //Assigning pins
    pinMode(_pinL1,OUTPUT);
    pinMode(_pinL2,OUTPUT);
    pinMode(_pinR1,OUTPUT);
    pinMode(_pinR2,OUTPUT);
    pinMode(_pwmL,OUTPUT);
    pinMode(_pwmR,OUTPUT);
    
}

//Moving Wheels
void AerDCMotors :: left_wheel_forward(int pwm_val) {
    // set direction
    digitalWrite(_pinL1,HIGH);
    digitalWrite(_pinL2,LOW);
    // set speed
    analogWrite(_pwmL,pwm_val);
    analogWrite(_pwmR,pwm_val);
}
void AerDCMotors :: right_wheel_forward(int pwm_val) {
    // set direction
    digitalWrite(_pinR1,HIGH);
    digitalWrite(_pinR2,LOW);
    // set speed
    analogWrite(_pwmL,pwm_val);
    analogWrite(_pwmR,pwm_val);
}
void AerDCMotors :: left_wheel_backward(int pwm_val) {
    // set direction
    digitalWrite(_pinL1,LOW);
    digitalWrite(_pinL2,HIGH);
    // set speed
    analogWrite(_pwmL,pwm_val);
    analogWrite(_pwmR,pwm_val);
}
void AerDCMotors :: right_wheel_backward(int pwm_val) {
    // set direction
    digitalWrite(_pinR1,LOW);
    digitalWrite(_pinR2,HIGH);
    // set speed
    analogWrite(_pwmL,pwm_val);
    analogWrite(_pwmR,pwm_val);
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
void AerDCMotors :: forward(int pwm_val) {
    this -> left_wheel_forward(pwm_val);
    this -> right_wheel_forward(pwm_val);
}
void AerDCMotors :: backward(int pwm_val) {
    this -> left_wheel_backward(pwm_val);
    this -> right_wheel_backward(pwm_val);
}
void AerDCMotors :: stop() {
    this -> left_wheel_stop();
    this -> right_wheel_stop();
}

//Moving Robot (complex)
void AerDCMotors :: pivot_left(int pwm_val) {
    this -> left_wheel_backward(pwm_val);
    this -> right_wheel_forward(pwm_val);
}
void AerDCMotors :: pivot_right(int pwm_val) {
    this -> left_wheel_forward(pwm_val);
    this -> right_wheel_backward(pwm_val);
}
void AerDCMotors :: u_turn_right() {
    this -> pivot_right(255);
    delay(0000);
    this -> stop();
    delay(000);
    this -> forward(255);
    delay(0000);
    this -> stop();
    delay(000);
    this -> pivot_right(255);
    delay(0000);
    this -> stop();
    delay(000);
}
void AerDCMotors :: u_turn_left() {
    this -> pivot_left(255);
    delay(0000);
    this -> stop();
    delay(000);
    this -> forward(255);
    delay(0000);
    this -> stop();
    delay(000);
    this -> pivot_left(255);
    delay(0000);
    this -> stop();
    delay(000);
}
