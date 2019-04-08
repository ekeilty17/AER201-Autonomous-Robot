#include "AerDCMotors/AerDCMotors.cpp"

#define RIGHT true
#define LEFT false

#define PWM_L     170
#define PWM_R     170
#define PWM_ADJ   150

#define CONE_CLEAR      4000
#define PIVOT           3100
#define FORWARD_PIVOT   6000

//Initiate DC Motors
// AerDCMotors(int pinL1, int pinL2, int pinR1, int pinR2)
AerDCMotors dc(5, 6, 9, 11);

int line_L = 18;      // A4
int line_R = 19;      // A5
int line_interr = 2;

void setup() {
  // Initialize DC Motors
  dc.init();

  // Initialize IR input sensor pins
  pinMode(line_L,INPUT);
  pinMode(line_R,INPUT);
  pinMode(line_interr, INPUT);
  //attachInterrupt(digitalPinToInterrupt(line_interr),line_follow_ISR,CHANGE);
  
}

void loop() {

  dc.forward();
  delay(CONE_CLEAR);
  dc.stop();
  
  dc.uturn_right(PIVOT, FORWARD_PIVOT, true);
  
  dc.forward();
  delay(1000);
  dc.stop();
  
  while(1);
}


void line_follow_ISR(void) {
  line_follow();
  return;
}
void line_follow() {
  Serial.print("Entered ISR");
  if (digitalRead(line_R) == LOW and digitalRead(line_L) == LOW) {
    // Sensing now lines, go straight  
    dc.left_wheel_forward();
    dc.right_wheel_forward();
  } else if (digitalRead(line_R) == HIGH and digitalRead(line_L) == LOW) {
    // Sensor over right lane
    dc.left_wheel_forward();
    dc.right_wheel_stop();
  } else if (digitalRead(line_R) == LOW and digitalRead(line_L) == HIGH){
    // Sensor over left lane
    dc.left_wheel_stop();
    dc.right_wheel_forward();
  } else {
    // Sensing both lanes
    dc.left_wheel_forward();
    dc.right_wheel_forward();
  }
}

/*
void gradual(int pwm_start, int pwm_end, int isRight) {
  if (isRight) {
    if (pwm_start < pwm_end) {
      for (int i=pwm_start; i <= pwm_end; i++) {
        dc.right_wheel_forward(i);
      }
    } else {
      for (int i=pwm_start; i <= pwm_end; i--) {
        dc.right_wheel_forward(i);
      }
    }
  } else {
    if (pwm_start < pwm_end) {
      for (int i=pwm_start; i <= pwm_end; i++) {
        dc.left_wheel_forward(i);
      }
    } else {
      for (int i=pwm_start; i <= pwm_end; i--) {
        dc.left_wheel_forward(i);
      }
    }
  }
}

void sensor_test(int sensor) {
  if (sensor == line_R) {
    if (digitalRead(line_R) == HIGH) {
      dc.right_wheel_forward(255);
      delay(1000);
      dc.stop();
    } else {
      dc.right_wheel_backward(225);
      delay(1000);
      dc.stop();
    }
  }
  if (sensor == line_L) {
    if (digitalRead(line_L) == HIGH) {
      dc.left_wheel_forward(255);
      delay(1000);
      dc.stop();
    } else {
      dc.left_wheel_backward(225);
      delay(1000);
      dc.stop();
    }
  }
}
*/
