#include "AerDCMotors/AerDCMotors.cpp"

/*
 * PWM balance for wheel
 * : = 237, R = 255
 * L = 228, R = 240
 * L = 215, R = 230
 */

#define RIGHT true
#define LEFT false

#define PWM_L 228
#define PWM_R 240

//Initiate DC Motors
// AerDCMotors(int pinL1, int pinL2, int pinR1, int pinR2)
AerDCMotors dc(5, 6, 9, 11);

int line_L = 4;
int line_R = 8;
int line_interr = 3;

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


void smooth_start(void) {
  dc.forward(255);
  delay(500);
  for(int i=255; i>=175; i--) {
    dc.forward(i);
    delay(50);
  }
}

void line_follow_ISR(void) {
  line_follow();
  return;
}
void line_follow() {
  if (digitalRead(line_R) == LOW and digitalRead(line_L) == LOW) {
    // Sensing now lines, go straight  
    dc.left_wheel_forward(PWM_L);
    dc.right_wheel_forward(PWM_R);
  } else if (digitalRead(line_R) == HIGH and digitalRead(line_L) == LOW) {
    // Sensor over right lane
    dc.left_wheel_forward(150);
    dc.right_wheel_stop();
  } else if (digitalRead(line_R) == LOW and digitalRead(line_L) == HIGH){
    // Sensor over left lane
    dc.left_wheel_stop();
    dc.right_wheel_forward(150);
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

/*
void line_follow() {
  Serial.print(sensors);
  Serial.print('\t');
  switch(sensors) {
      case 0:   // both sensors on either side of line
        Serial.print("both on");
        Serial.print('\n');
        Serial.print('\n');
        dc.forward(255);
        break;
      case 1:   // right sensor reads line
        Serial.print("right off");
        Serial.print('\n');
        Serial.print('\n');
        dc.left_wheel_forward(255);
        dc.right_wheel_stop();
        break;
      case 10:  // left sensor reads line
        Serial.print("left off");
        Serial.print('\n');
        Serial.print('\n');
        dc.left_wheel_stop();
        dc.right_wheel_forward(255);
        break;
      case 11:  // both sensors read lines
        //dc.stop();
        Serial.print("both off");
        Serial.print('\n');
        Serial.print('\n');
        dc.stop();
        break;
      default:
        break;
    }
}
*/

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
