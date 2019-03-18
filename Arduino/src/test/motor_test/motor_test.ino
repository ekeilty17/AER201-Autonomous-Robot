#include "AerDCMotors/AerDCMotors.cpp"

#define RIGHT true
#define LEFT false
//Initiate DC Motors
// AerDCMotors(int pinL1, int pinL2, int pinR1, int pinR2)
AerDCMotors dc(9, 11, 5, 6);

void setup() {
  // Initialize DC Motors
  dc.init();
}

void loop() {
//  wheel_test(dc, 255);
//  delay(2000);
//  wheel_test(dc, 128);
  dc.forward(255);
  delay(5000);
  dc.stop();
}


void PWM_test_turn(AerDCMotors dc){
  
  dc.forward(255); 
  delay(500); 
  
  for (int i=200;i<230;i++){
    dc.forward(230);
    delay(50); 
  }
  
  delay(2000); 
  dc.stop();
  
}

void right_turn(AerDCMotors dc) {
  dc.left_wheel_forward(255);
  dc.right_wheel_backward(255);
  delay(1200);
  dc.stop();
  delay(1200);
  dc.forward(255); 
  delay(1200);
  dc.stop();
  delay(1200);
  dc.left_wheel_forward(255);
  dc.right_wheel_backward(255);
  delay(1200);
  dc.stop();
}

void wheel_test(AerDCMotors dc, int pwm_val) {
  
  delay(1000);
  dc.forward(pwm_val);
  delay(1000);
  dc.stop();
  
  delay(1000);
  dc.backward(pwm_val);
  delay(1000);
  dc.stop();

  delay(1000);
  dc.left_wheel_forward(pwm_val);
  delay(1000);
  dc.stop();

  delay(1000);
  dc.right_wheel_forward(pwm_val);
  delay(1000);
  dc.stop();

  delay(1000);
  dc.left_wheel_backward(pwm_val);
  delay(1000);
  dc.stop();

  delay(1000);
  dc.right_wheel_backward(pwm_val);
  delay(1000);
  dc.stop();
}

void turning_self_consistency_test(AerDCMotors dc, int isRight) {
  if (isRight) {
    dc.u_turn_right();
    dc.u_turn_right();
  } else {
    dc.u_turn_left();
    dc.u_turn_left();
  }
}
