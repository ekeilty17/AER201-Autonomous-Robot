#include "AerDCMotors/AerDCMotors.cpp"

#define RIGHT true
#define LEFT false

int volatile interrupt = false;

//Initiate DC Motors
// AerDCMotors(int pinL1, int pinL2, int pinR1, int pinR2)
AerDCMotors dc(9, 11, 5, 6);

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
  
  dc.forward(255);
  
}

void loop() {
  
  while(1) {
    if (interrupt) {
      line_follow();
      interrupt = false;
    }
  }
}


void line_follow_ISR(void) {
  
  Serial.print("change detected");
  Serial.print('\t');
  Serial.print('\t');
  
  interrupt = true;
  return;

}

void line_follow() {

  int pwm_val = 200 ;
  
  if (digitalRead(line_R)) {  // detected a line
    Serial.print("right off");
    Serial.print('\n');
    Serial.print('\n');
    //dc.right_wheel_stop();
    //gradual(255, pwm_val, RIGHT);
    dc.right_wheel_forward(pwm_val);
  } else {
    Serial.print("right on");
    Serial.print('\n');
    Serial.print('\n');
    dc.right_wheel_forward(255);
  }

  if (digitalRead(line_L)) {  // detected a line
    Serial.print("left off");
    Serial.print('\n');
    Serial.print('\n');
    dc.left_wheel_stop();
    //gradual(255, pwm_val, LEFT);
    //dc.left_wheel_forward(pwm_val);
  } else {
    Serial.print("left on");
    Serial.print('\n');
    Serial.print('\n');
    dc.left_wheel_forward(255);
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
