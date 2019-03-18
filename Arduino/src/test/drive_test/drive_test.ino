#include "AerDCMotors/AerDCMotors.cpp"

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
  attachInterrupt(digitalPinToInterrupt(line_interr),line_follow,CHANGE);
  
  Serial.begin(9600);
  Serial.print("start");
  Serial.print('\n');
  
  dc.forward(255);
  
}

void loop() {
  while(1);
}



unsigned long lastInterrupt = 0;
void line_follow(void) {

  int pwm_val = 230;

  if (millis() - lastInterrupt > 1) {
    
    Serial.print("change detected");
    Serial.print('\n');
    
    if (digitalRead(line_L) == HIGH) {
      //dc.left_wheel_forward(pwm_val);
      dc.left_wheel_stop();
      Serial.print("Left wheel stop");
      Serial.print('\n');
    } else {
      dc.left_wheel_forward(225);
      Serial.print("Left wheel start");
      Serial.print('\n');
    }
    
    if (digitalRead(line_R) == HIGH) {
      //dc.right_wheel_forward(pwm_val);
      dc.right_wheel_stop();
      Serial.print("Right wheel stop");
      Serial.print('\n');
    } else {
      dc.right_wheel_forward(225);
      Serial.print("Right wheel start");
      Serial.print('\n');
    }
  }

  lastInterrupt = millis();
  return;

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
