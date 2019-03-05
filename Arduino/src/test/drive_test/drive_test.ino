#include "AerDCMotors/AerDCMotors.cpp"

#define FORWARD 1
#define STOP 0
#define BACKWARD -1

#define RIGHT true
#define LEFT false

//Initiate DC Motors
// AerDCMotors(int pinL1, int pinL2, int pinR1, int pinR2, int pwmL, int pwmR)
AerDCMotors dc(8, 7, 10, 9, 5, 6);

int sensor_L = 2;
int sensor_R = 12;

int c = 0; 
int d = 0;

void setup() {
  // Initialize DC Motors
  dc.init();

  // Initialize IR input sensor pins
  pinMode(sensor_L,INPUT);
  pinMode(sensor_R,INPUT);
  attachInterrupt(digitalPinToInterrupt(sensor_L),line_follow,CHANGE); 
  attachInterrupt(digitalPinToInterrupt(sensor_R),line_follow,CHANGE); 

  Serial.begin(9600);
}

void loop() {
  //Serial.print(digitalRead(4));
  //PWM_test_turn(dc);
  //line_follow(dc);

//  dc.forward(255);
//  delay(2000);
//  dc.stop();

  sensor_test(LEFT);
    
  while(1);
}


/*
 * Functionality
 */
void line_follow(void) {
  
  int pwm_val = 230;
  
  if (digitalRead(sensor_L) == HIGH) {
    for (int i=pwm_val;i<255;i++){
      dc.left_wheel_forward(i);
      delay(50); 
    }
  } else {
    //dc.left_wheel_forward(pwm_val);
    dc.left_wheel_stop();
  }

  if (digitalRead(sensor_R) == HIGH) {
    for (int i=pwm_val;i<255;i++){
      dc.right_wheel_forward(i);
      delay(50); 
    }
  } else {
    //dc.right_wheel_forward(pwm_val);
    dc.right_wheel_stop();
  }
  
}

void line_follow_old(void) {
  if (digitalRead(sensor_L) && c == 0) {
    dc.left_wheel_forward(255);
    c = 0;
  }else if (digitalRead(sensor_L) && c == 1){
    for (int i=220;i<255;i++){
      //analogWrite(5,i);
      dc.left_wheel_forward(i);
      delay(50); 
    }
    c = 0; 
  }else{
    for (int i=255;i<220;i--){
      //analogWrite(5,i);
      dc.left_wheel_forward(i);
      delay(50); 
    }
    c = 1;
  }
  
  if (digitalRead(sensor_R) && d == 0) {
    dc.right_wheel_forward(255);
    d = 0;
  }else if (digitalRead(sensor_R) && d == 1){
    for (int i=220;i<255;i++){
      //analogWrite(6,i);
      dc.right_wheel_forward(i);
      delay(50); 
    }
    d = 0; 
  }else{
    for (int i=255;i<220;i--){
      //analogWrite(6,i);
      dc.right_wheel_forward(i);
      delay(50); 
    }
    d = 1;
  }
}

/*
 * Interrupt Test
 */
void sensor_test(int isRight) {
  Serial.print(digitalRead(2));
  if (isRight) {
    if (digitalRead(sensor_R) == HIGH) {
      dc.right_wheel_forward(255);
      delay(1000);
      dc.stop();
    } else {
      dc.right_wheel_backward(255);
      delay(1000);
      dc.stop();
    }
  } else {
    if (digitalRead(sensor_L) == HIGH) {
      dc.left_wheel_forward(255);
      delay(1000);
      dc.stop();
    } else {
      dc.left_wheel_backward(255);
      delay(1000);
      dc.stop();
    }
  }
  
}

/*
 * Motion Tests
 */
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

void wheel_test(AerDCMotors dc, int pwm_val, int dir) {
  if (dir == FORWARD) {
    dc.forward(pwm_val);
  } else if (dir == BACKWARD) {
    dc.backward(pwm_val);
  } else {
    dc.stop();
  }
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
