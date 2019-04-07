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
    analogWrite(_pinL1, 255);
    digitalWrite(_pinL2, LOW);
    delay(100);
    analogWrite(_pinL1, pwm_val);
    digitalWrite(_pinL2, LOW);
}
void AerDCMotors :: right_wheel_forward(int pwm_val) {
    analogWrite(_pinR1, 255);
    digitalWrite(_pinR2, LOW);
    delay(100);
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
void AerDCMotors :: forward(int pwm_val_L, int pwm_val_R, bool jump) {
    /*
     this -> left_wheel_forward(pwm_val_L);
     this -> right_wheel_forward(pwm_val_R);
     */
    // jump start
    if (jump) {
        analogWrite(_pinL1, 255);
        digitalWrite(_pinL2, LOW);
        analogWrite(_pinR1, 255);
        digitalWrite(_pinR2, LOW);
        delay(25);
    }
    // steady movement
    analogWrite(_pinL1, pwm_val_L);
    digitalWrite(_pinL2, LOW);
    analogWrite(_pinR1, pwm_val_R);
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
void AerDCMotors :: stop(int pwm_val_L, int pwm_val_R) {
    
    int pwm_low;
    if (pwm_val_L < pwm_val_R) {
        pwm_low = pwm_val_L;
    } else {
        pwm_low = pwm_val_R;
    }
    
    digitalWrite(_pinL1, LOW);
    analogWrite(_pinL2, pwm_low);
    digitalWrite(_pinR1, LOW);
    analogWrite(_pinR2, pwm_low);
    delay(10);
    digitalWrite(_pinL1,LOW);
    digitalWrite(_pinL2,LOW);
    digitalWrite(_pinR1,LOW);
    digitalWrite(_pinR2,LOW);
    /*
    this -> left_wheel_stop();
    this -> right_wheel_stop();
     */
}
void AerDCMotors :: gradual_stop(int pwm_val_L, int pwm_val_R) {
    
    int pwm_low;
    if (pwm_val_L < pwm_val_R) {
        pwm_low = pwm_val_L;
    } else {
        pwm_low = pwm_val_R;
    }
    
    analogWrite(_pinL1, pwm_low);
    digitalWrite(_pinL2, LOW);
    analogWrite(_pinR1, pwm_low);
    digitalWrite(_pinR2, LOW);
    
    for (int i=pwm_low; i>0; i--) {
        analogWrite(_pinL1, i);
        digitalWrite(_pinL2, LOW);
        analogWrite(_pinR1, i);
        digitalWrite(_pinR2, LOW);
        delay(5);
    }
    
    digitalWrite(_pinL1,LOW);
    digitalWrite(_pinL2,LOW);
    digitalWrite(_pinR1,LOW);
    digitalWrite(_pinR2,LOW);
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
void AerDCMotors :: uturn_right(int pwm_val_turn, int pwm_val_L, int pwm_val_R, int turn_delay_1, int turn_delay_2) {
    this -> swing_right(pwm_val_turn);
    delay(turn_delay_1);
    this -> stop(pwm_val_L, pwm_val_R);
    delay(500);
    this -> forward(pwm_val_L, pwm_val_R, true);
    delay(1500);
    this -> stop(pwm_val_L, pwm_val_R);
    delay(500);
    this -> swing_right(pwm_val_turn);
    delay(turn_delay_2);
    this -> stop(pwm_val_L, pwm_val_R);
    delay(500);
}
void AerDCMotors :: uturn_left(int pwm_val_turn, int pwm_val_L, int pwm_val_R, int turn_delay_1, int turn_delay_2) {
    this -> pivot_left(pwm_val_turn);
    delay(0000);
    this -> stop(pwm_val_L, pwm_val_R);
    delay(000);
    this -> forward(pwm_val_L, pwm_val_R, true);
    delay(0000);
    this -> stop(pwm_val_L, pwm_val_R);
    delay(000);
    this -> pivot_left(pwm_val_turn);
    delay(0000);
    this -> stop(pwm_val_L, pwm_val_R);
    delay(000);
}
