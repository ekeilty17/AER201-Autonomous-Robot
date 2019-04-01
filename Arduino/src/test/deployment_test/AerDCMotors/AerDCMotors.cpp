#include "Arduino.h"    // needed in order to call delay()
#include "AerDCMotors.h"

//Constructors
AerDCMotors :: AerDCMotors(){
}
AerDCMotors :: AerDCMotors(int pinR2, int pinR1, int pinL2, int pinL1){
    _pinL1 = pinL1;
    _pinL2 = pinL2;
    _pinR1 = pinR1;
    _pinR2 = pinR2;
}
void AerDCMotors :: init() {
    //Assigning pins
    pinMode(_pinL1,OUTPUT);
    pinMode(_pinL2,OUTPUT);
    pinMode(_pinR2,OUTPUT);
    pinMode(_pinR2,OUTPUT);
    
}

//Moving Wheels
void AerDCMotors :: left_wheel_forward(int pwm_val) {
    analogWrite(_pinL1, pwm_val);
    digitalWrite(_pinL2, LOW);
}
void AerDCMotors :: right_wheel_forward(int pwm_val) {
    analogWrite(_pinR1, pwm_val);
    digitalWrite(_pinR2, LOW);
}
void AerDCMotors :: left_wheel_backward(int pwm_val) {
    digitalWrite(_pinL1, LOW);
    analogWrite(_pinL2, pwm_val);
}
void AerDCMotors :: right_wheel_backward(int pwm_val) {
    digitalWrite(_pinR1, LOW);
    analogWrite(_pinR2, pwm_val);
}
void AerDCMotors :: left_wheel_stop() {
    digitalWrite(_pinL1,HIGH);
    digitalWrite(_pinL2,HIGH);
    /*
    analogWrite(_pinL1, 0);
    analogWrite(_pinL2, 0);
     */
    /*
     digitalWrite(_pinL1,HIGH);
     digitalWrite(_pinL2,HIGH);
     */
}
void AerDCMotors :: right_wheel_stop() {
    digitalWrite(_pinR1,HIGH);
    digitalWrite(_pinR2,HIGH);
    /*
    analogWrite(_pinR1, 0);
    analogWrite(_pinR2, 0);
     */
    /*
     digitalWrite(_pinR1,HIGH);
     digitalWrite(_pinR2,HIGH);
     */
}

//Moving Robot (simple)
void AerDCMotors :: forward(int pwm_val) {
    /*
     this -> left_wheel_forward(pwm_val);
     this -> right_wheel_forward(pwm_val);
     */
    
    analogWrite(_pinL1, pwm_val);
    digitalWrite(_pinL2, LOW);
    analogWrite(_pinR1, pwm_val);
    digitalWrite(_pinR2, LOW);
}
void AerDCMotors :: backward(int pwm_val) {
    /*
     this -> left_wheel_backward(pwm_val);
     this -> right_wheel_backward(pwm_val);
     */
    
    digitalWrite(_pinL1, LOW);
    analogWrite(_pinL2, pwm_val);
    digitalWrite(_pinR1, LOW);
    analogWrite(_pinR2, pwm_val);
}
void AerDCMotors :: stop() {
    digitalWrite(_pinL1,LOW);
    digitalWrite(_pinL2,LOW);
    digitalWrite(_pinR1,LOW);
    digitalWrite(_pinR2,LOW);
    /*
    this -> left_wheel_stop();
    this -> right_wheel_stop();
     */
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
void AerDCMotors :: swing_left(int pwm_val) {
    this -> left_wheel_stop();
    this -> right_wheel_forward(pwm_val);
}
void AerDCMotors :: swing_right(int pwm_val) {
    this -> left_wheel_forward(pwm_val);
    this -> right_wheel_stop();
}
void AerDCMotors :: uturn_right(int pwm_val_turn, int pwm_val_L, int pwm_val_R) {
    this -> swing_right(pwm_val_turn);
    delay(1500);
    this -> stop();
    delay(500);
    this -> left_wheel_forward(pwm_val_L);
    this -> right_wheel_forward(pwm_val_R);
    delay(3000);
    this -> stop();
    delay(500);
    this -> swing_right(pwm_val_turn);
    delay(1500);
    this -> stop();
    delay(500);
}
void AerDCMotors :: uturn_left(int pwm_val_turn, int pwm_val_L, int pwm_val_R) {
    this -> pivot_left(pwm_val_turn);
    delay(0000);
    this -> stop();
    delay(000);
    this -> left_wheel_forward(pwm_val_L);
    this -> right_wheel_forward(pwm_val_R);
    delay(0000);
    this -> stop();
    delay(000);
    this -> pivot_left(pwm_val_turn);
    delay(0000);
    this -> stop();
    delay(000);
}
