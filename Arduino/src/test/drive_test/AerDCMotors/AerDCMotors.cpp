#include "Arduino.h"    // needed in order to call delay()
#include "AerDCMotors.h"

//Constructors
AerDCMotors :: AerDCMotors(){
}
AerDCMotors :: AerDCMotors(int pinR2, int pinR1, int pinL2, int pinL1){
    _pinL1 = pinL2;
    _pinL2 = pinL1;
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
void AerDCMotors :: left_wheel_forward() {
    digitalWrite(_pinL1, HIGH);
    digitalWrite(_pinL2, LOW);
}
void AerDCMotors :: right_wheel_forward() {
    digitalWrite(_pinR1, HIGH);
    digitalWrite(_pinR2, LOW);
}
void AerDCMotors :: left_wheel_backward() {
    digitalWrite(_pinL1, LOW);
    digitalWrite(_pinL2, HIGH);
}
void AerDCMotors :: right_wheel_backward() {
    digitalWrite(_pinR1, LOW);
    digitalWrite(_pinR2, HIGH);
}
void AerDCMotors :: left_wheel_stop() {
    digitalWrite(_pinL1,HIGH);
    digitalWrite(_pinL2,HIGH);
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
void AerDCMotors :: forward() {
     this -> left_wheel_forward();
     this -> right_wheel_forward();
    
}
void AerDCMotors :: backward() {
     this -> left_wheel_backward();
     this -> right_wheel_backward();
}
void AerDCMotors :: stop() {
    this -> left_wheel_stop();
    this -> right_wheel_stop();
}

//Moving Robot (complex)
void AerDCMotors :: pivot_left() {
    this -> left_wheel_backward();
    this -> right_wheel_forward();
}
void AerDCMotors :: pivot_right() {
    this -> left_wheel_forward();
    this -> right_wheel_backward();
}
void AerDCMotors :: swing_left() {
    this -> left_wheel_stop();
    this -> right_wheel_forward();
}
void AerDCMotors :: swing_right() {
    this -> left_wheel_forward();
    this -> right_wheel_stop();
}
void AerDCMotors :: uturn_right(int turn_delay, int forward_delay, bool pivot) {
    
    if (pivot) {
        this -> pivot_right();
    } else {
        this -> swing_right();
    }
    delay(turn_delay);
    
    this -> stop();
    delay(500);
    this -> forward();
    delay(forward_delay);
    this -> stop();
    delay(500);
    
    if (pivot) {
        this -> pivot_right();
    } else {
        this -> swing_right();
    }
    delay(turn_delay);
    
    this -> stop();
    delay(500);
    this -> swing_right();
    /*
    delay(turn_delay_1);
    this -> stop(pwm_val_L, pwm_val_R, false);
    delay(500);
     */
}
