#include "AerServo/AerServo.cpp"
#include "AerServo/Servo2.cpp"

#define RIGHT true
#define LEFT false

//Initiate Servo
AerServo servo(10);
//Servo servo;

void setup() {
  servo.init();
}

void loop() {

  servo.move_to_time(1000);
  
  /*
  servo.setCurr_pos(8.5);
  servo.move_right();
  delay(1000);
  servo.stop();
  */
  
  while(1); 

}
/*
void displacement_test(AerServo servo, int t, bool move_right) {
  if(move_right) {
    servo.move_right();
    delay(t);
    servo.stop();
  } else {
    servo.move_left();
    delay(t);
    servo.stop();
  }
}

void self_consistency_test(AerServo servo, int n, bool start_right) {
  if(start_right) {
    for(int i = 0; i < n; i++) {
      servo.displace_left(n);
      delay(500);
      servo.displace_right(n);
      delay(500);
    }
  } else {
    for(int i = 0; i < n; i++) {
      servo.displace_right(n);
      delay(500);
      servo.displace_left(n);
      delay(500);
    }
  }
}


void hole_deploy() {
  servo.to_middle();
  delay(1000);
  return;
}
void left_crack_deploy() {
  // Cone 1
  //servo.hard_left();
  servo.move_to_time(300);
  delay(1000);
  // Cone 2
  servo.move_to_time(1600);
  delay(1000);
  servo.setCurr_pos(10);
}
void right_crack_deploy() {
  // Cone 1
  //servo.hard_right();
  servo.move_to_time(-400);
  delay(1000);
  // Cone 2
  servo.move_to_time(-2100);
  delay(1000);
  servo.setCurr_pos(7);
}
void centre_crack_deploy() {
  // Cone 1
  servo.move_to_time(-2000);
  servo.move_to_time(700);
  delay(1000);
  // Cone 2
  servo.move_to_time(4500);
  servo.move_to_time(-800);
  delay(1000);
  servo.setCurr_pos(13);
}
void center_crack_deploy() {
  centre_crack_deploy();
}
*/
