#include "AerDCMotors/AerDCMotors.cpp"

#define FORWARD 0
#define BACKWARD 1
#define SLOW  2
#define STOP 3

#define RIGHT true
#define LEFT false

//Initiate DC Motors
AerDCMotors dc(7, 8, 9, 10);

int c = 0; 
int d = 0;

void setup() {
  dc.init();
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(11,INPUT);
  pinMode(12,INPUT); 
}

void loop() {
  delay(2000);
  PWM_test_turn(dc);
  while(1);
}

void PWM_test_turn(AerDCMotors dc){
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH); 
  analogWrite(5,255);
  analogWrite(6,255);
  delay(500); 
  analogWrite(5,230);
  analogWrite(6,230);
//  for (int i=200;i<230;i++){
//    analogWrite(5,i);
//    analogWrite(6,i);
//    delay(50); 
//  }
  delay(2000); 
  dc.stop(); 
}

void wheel_test(AerDCMotors dc, int dir) {
  if (dir == FORWARD) {
    dc.forward();
  } else if (dir == BACKWARD) {
    dc.backward();
  } else if (dir == SLOW) {
    dc.slow();
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

void right_turn(AerDCMotors dc) {
  dc.left_wheel_forward();
  dc.right_wheel_backward();
  delay(1200);
  dc.stop();
  delay(1200);
  dc.forward(); 
  delay(1200);
  dc.stop();
  delay(1200);
  dc.left_wheel_forward();
  dc.right_wheel_backward();
  delay(1200);
  dc.stop();
}

//Line following with rotary encoders
//  I'm guessing with this method, I'm not really sure how it's going to be implemented
//  Again, at this point it's more for reference to show you what the logic should be
//  based on the inputs
void line_follow(AerDCMotors dc) {
  if (digitalRead(11) && c == 0) {
    dc.left_wheel_forward();
    c = 0;
  }else if (digitalRead(11) && c == 1){
    digitalWrite(7,HIGH);
    digitalWrite(8,LOW);
    for (int i=220;i<255;i++){
      analogWrite(5,i);
      delay(50); 
    }
    c = 0; 
  }else{
    digitalWrite(7,HIGH);
    digitalWrite(8,LOW);
    for (int i=255;i<220;i--){
      analogWrite(5,i);
      delay(50); 
    }
    c = 1;
  }
  
  if (digitalRead(12) && d == 0) {
    dc.right_wheel_forward();
    d = 0;
  }else if (digitalRead(12) && d == 1){
    digitalWrite(9,HIGH);
    digitalWrite(10,LOW);
    for (int i=220;i<255;i++){
      analogWrite(6,i);
      delay(50); 
    }
    d = 0; 
  }else{
    digitalWrite(9,HIGH);
    digitalWrite(10,LOW);
    for (int i=255;i<220;i--){
      analogWrite(6,i);
      delay(50); 
    }
    d = 1;
  }

  // The logic more explicitly written
  /*
  if(irL == HIGH && irR == HIGH) {
    dc.left_wheel_forward();
    dc.right_wheel_forward();
  } else if(irL == HIGH && irR == LOW) {
    dc.left_wheel_forward();
    dc.right_wheel_stop();
  } else if (irL == LOW && irR == HIGH) {
    dc.left_wheel_stop();
    dc.right_wheel_forward();
  } else {
    // something probably went wrong
    dc.left_wheel_stop();
    dc.right_wheel_stop();
  }
  */
}
