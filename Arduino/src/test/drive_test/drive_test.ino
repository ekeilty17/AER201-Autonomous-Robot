#include "AerDCMotors/AerDCMotors.cpp"

/*
 * PWM balance for wheel
 * : = 237, R = 255
 * L = 228, R = 240
 * L = 215, R = 230
 */

#define RIGHT true
#define LEFT false

#define PWM_L     170
#define PWM_R     170
#define PWM_ADJ   150

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
  attachInterrupt(digitalPinToInterrupt(line_interr),line_follow_ISR,CHANGE);
  
  Serial.begin(9600);
  Serial.print("start");
  Serial.print('\n');
  
  dc.left_wheel_forward(PWM_L);
  dc.right_wheel_forward(PWM_R);
  
}

void loop() {

  while(1);
//  while(1) {
//    if (detectedLine) {
//      line_follow();
//      detectedLine = false;
//    }
//  }
}


void smooth_start(int pwm_val) {
  dc.forward(255, 255, true);
  delay(500);
  for(int i=255; i>=pwm_val; i--) {
    dc.forward(i, i, forward);
    delay(50);
  }
}

void line_follow_ISR(void) {
  line_follow();
  return;
}
void line_follow() {
  Serial.print("Entered ISR");
  if (digitalRead(line_R) == LOW and digitalRead(line_L) == LOW) {
    // Sensing now lines, go straight  
    dc.left_wheel_forward(PWM_L);
    dc.right_wheel_forward(PWM_R);
  } else if (digitalRead(line_R) == HIGH and digitalRead(line_L) == LOW) {
    // Sensor over right lane
    dc.left_wheel_forward(PWM_ADJ);
    dc.right_wheel_stop();
  } else if (digitalRead(line_R) == LOW and digitalRead(line_L) == HIGH){
    // Sensor over left lane
    dc.left_wheel_stop();
    dc.right_wheel_forward(PWM_ADJ);
  } else {
    // Sensing both lanes
    dc.stop();
  }
}

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
