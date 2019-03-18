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
  
  //this -> _t_cleft = (int) this -> _t_left * (3 / 10);
  //this -> _t_cright = (int) this -> _t_right * (3 / 10);
  
  this -> attach(_pin);
}

//Setter
void AerServo :: setPin(int pin) {
  _pin = pin;
}
void AerServo :: setCurr_pos(float curr_pos) {
  _curr_pos = curr_pos;
}

//Basic Motion
void AerServo :: move_right() {
  this -> write(0);
}
void AerServo :: move_left() {
  this -> write(180);
}
void AerServo :: stop() {
  this -> write(90);
}

//Relative Displacement
float AerServo :: displace_right(float dx) {
  this -> move_right();
  int t = (int) (dx * 1000) / _velocity_r;
  delay(t);
  this -> stop();
  _curr_pos -= dx;
  return _curr_pos;
}
float AerServo :: displace_left(float dx) {
  this -> move_left();
  int t = (int) (dx * 1000) / _velocity_l;
  delay(t);
  this -> stop();
  _curr_pos -= dx;
  return _curr_pos;
}

// Moving to specific positions
float AerServo :: move_to(float x) {
  if (x > _curr_pos) {
    this -> displace_right(x - _curr_pos);
  } else {
    this -> displace_left(_curr_pos - x);
  }
  _curr_pos = x;
  return _curr_pos;
}
float AerServo :: move_to_manual(float x) {
    return 10.0;
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
  if (_curr_pos < 9.5) {
    this -> hard_left();
  } else {
    this -> hard_right();
  }
  this -> to_middle();
  _curr_pos = 8.5;
  return 8.5;
}
