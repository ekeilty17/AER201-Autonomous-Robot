#include "Arduino.h"    // needed in order to call delay()
#include "AerServo.h"

// Constructors
AerServo :: AerServo() {
}
AerServo :: AerServo(int pin){
  _pin = pin;
  _curr_pos = 0.0;
}
AerServo :: AerServo(int pin, float curr_pos){
  _pin = pin;
  _curr_pos = curr_pos;
}
void AerServo :: init() {
  this -> attach(_pin);
}

//Setter
void AerServo :: setPin(int pin) {
  _pin = pin;
}
void AerServo :: setCurr_pos(float curr_pos) {
  _curr_pos = curr_pos;
}
void AerServo :: setV(float v) {
    _v = v;
}
void AerServo :: setVelocity(float t) {
    // This actually takes in the time (in ms) it takes for the servo to go end-to-end
    _v = (_width * 1000) / t;    // in cm/s
}

//Basic Motion
void AerServo :: stop() {
    this -> write(90);
}
void AerServo :: move_right(int t) {
  this -> write(180);
  delay(t);
  this -> stop();
}
void AerServo :: move_left(int t) {
  this -> write(0);
  delay(t);
  this -> stop();
}
void AerServo :: move(int t) {
    if (t > 0) {
        this -> move_right(t);
        
    } else {
        this -> move_left(t);
    }
}

//Relative Displacement
float AerServo :: displace_right(float dx) {
  int t = (int) (dx * 1000) / _v;
  this -> move_right(t);
  _curr_pos += dx;
  return _curr_pos;
}
float AerServo :: displace_left(float dx) {
  int t = (int) (dx * 1000) / _v;
  this -> move_left(t);
  _curr_pos -= dx;
  return _curr_pos;
}
float AerServo :: displace(float dx) {
    if (dx > 0) {
        return this -> displace_right(dx);
    } else {
        return this -> displace_left(-dx);
    }
}

// Moving to specific positions
float AerServo :: move_to(float x) {
  this -> displace(x - _curr_pos);
  _curr_pos = x;
  return _curr_pos;
}
float AerServo :: to_middle() {
  this -> move_to(8.5);
  return 8.5;
}
float AerServo :: hard_left() {
  this -> move_to(0 - 2);   //over shoot to ensure it's flush
  _curr_pos = 0;
  return 0;
}
float AerServo :: hard_right() {
  this -> move_to(17 + 2);  //over shoot to ensure it's flush
  _curr_pos = 17;
  return 17;
}
float AerServo :: reset(){
  if (_curr_pos < 8.5) {
    this -> hard_left();
  } else {
    this -> hard_right();
  }
  this -> to_middle();
  _curr_pos = 8.5;
  return 8.5;
}
