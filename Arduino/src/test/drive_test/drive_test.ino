#include "AerDCMotors/AerDCMotors.cpp"

#define FORWARD 1
#define STOP 0
#define BACKWARD -1

#define RIGHT true
#define LEFT false

bool first = true;

//Initiate DC Motors
// AerDCMotors(int pinL1, int pinL2, int pinR1, int pinR2, int pwmL, int pwmR)
AerDCMotors dc(9, 11, 5, 6);

int line_L = 4;
int line_R = 8;
int line_interr = 3; 

int x = 0;

boolean stopp_left = false;
boolean stopp_right = false;  
boolean startt_left = false; 
boolean startt_right = false; 


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
  
//  dc.forward(255);
//  delay(2000);
//  dc.backward(255);
//  delay(2000);
//  dc.stop();
}

void loop() {
  //Serial.print(digitalRead(4));
  //PWM_test_turn(dc);
  //line_follow(dc);
  //dc.forward(225);
//  Serial.print(x);
//  Serial.print('\n');
////  dc.forward(255);
//  dc.stop();
//  dc.left_wheel_forward(255);
//  delay(1000);
//  dc.left_wheel_stop();
//  delay(1000);
//  dc.left_wheel_backward(255);
//  delay(1000);
//
//  dc.stop();
//  dc.right_wheel_forward(255);
//  delay(1000);
//  dc.right_wheel_stop();
//  delay(1000);
//  dc.right_wheel_backward(255);
//  delay(1000);

  if (stopp_left){
    dc.left_wheel_stop();
    stopp_left = false;
    Serial.print("left wheel stop");
    Serial.print('\n'); 
  }
  if (stopp_right){
    dc.right_wheel_stop();
    stopp_right = false;
    Serial.print("right wheel stop");
    Serial.print('\n');
  }
  if (startt_left){
    dc.left_wheel_forward(255);
    startt_left = false;
    Serial.print("left forward");
    Serial.print('\n');
  }
  if (startt_right){
    dc.right_wheel_forward(255);
    startt_right = false;
    Serial.print("right forward");
    Serial.print('\n'); 
  }
}


 
void line_follow(void) {

  //if (first) {
    //  int pwm_val = 230;
    Serial.print("change detected");
    Serial.print('\n');
    
    if (digitalRead(line_L) == HIGH) {
      //dc.left_wheel_forward(pwm_val);
      //Serial.print("stop");
      stopp_left = true; 
      Serial.print("LEFT Stop");
      Serial.print('\n');
    }if (digitalRead(line_L) == LOW) {
      //dc.left_wheel_forward(pwm_val);
      //Serial.print("stop");
      startt_left = true; 
      Serial.print("LEFT Start");
      Serial.print('\n');
    }
    if (digitalRead(line_R) == HIGH) {
      //Serial.print("stop"); 
      //dc.right_wheel_forward(pwm_val);
      stopp_right = true; 
      Serial.print("RIGHT Stop");
      Serial.print('\n');
    } if (digitalRead(line_R) == LOW) {
      //dc.left_wheel_forward(pwm_val);
      //Serial.print("stop");
      startt_right = true; 
      Serial.print("Right Start");
      Serial.print('\n');
    }

}



//void line_follow_old(void) {
//  if (digitalRead(line_L) && c == 0) {
//    dc.left_wheel_forward(255);
//    c = 0;
//  }else if (digitalRead(line_L) && c == 1){
//    for (int i=220;i<255;i++){
//      //analogWrite(5,i);
//      dc.left_wheel_forward(i);
//      delay(50); 
//    }
//    c = 0; 
//  }else{
//    for (int i=255;i<220;i--){
//      //analogWrite(5,i);
//      dc.left_wheel_forward(i);
//      delay(50); 
//    }
//    c = 1;
//  }
//  
//  if (digitalRead(line_R) && d == 0) {
//    dc.right_wheel_forward(255);
//    d = 0;
//  }else if (digitalRead(line_R) && d == 1){
//    for (int i=220;i<255;i++){
//      //analogWrite(6,i);
//      dc.right_wheel_forward(i);
//      delay(50); 
//    }
//    d = 0; 
//  }else{
//    for (int i=255;i<220;i--){
//      //analogWrite(6,i);
//      dc.right_wheel_forward(i);
//      delay(50); 
//    }
//    d = 1;
//  }
//}

/*
void sensor_test(int isRight) {
  Serial.print(digitalRead(2));
  if (isRight) {
    if (digitalRead(line_R) == HIGH) {
      dc.right_wheel_forward(255);
      delay(1000);
      dc.stop();
    } else {
      dc.right_wheel_backward();
      delay(1000);
      dc.stop();
    }
  } else {
    if (digitalRead(line_L) == HIGH) {
      dc.left_wheel_forward(255);
      delay(1000);
      dc.stop();
    } else {
      dc.left_wheel_backward();
      delay(1000);
      dc.stop();
    }
  }
  
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
  dc.right_wheel_backward();
  delay(1200);
  dc.stop();
  delay(1200);
  dc.forward(255); 
  delay(1200);
  dc.stop();
  delay(1200);
  dc.left_wheel_forward(255);
  dc.right_wheel_backward();
  delay(1200);
  dc.stop();
}

void wheel_test(AerDCMotors dc, int pwm_val, int dir) {
  if (dir == FORWARD) {
    dc.forward(pwm_val);
  } else if (dir == BACKWARD) {
    dc.backward();
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
*/
