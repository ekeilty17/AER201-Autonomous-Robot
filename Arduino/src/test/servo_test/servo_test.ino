#include "AerServo/AerServo.cpp"
#include "AerServo/Servo2.cpp"

#define RIGHT true
#define LEFT false

#define MIDDLE_TO_EDGE  2000
#define EDGE_TO_EDGE    3000
#define EDGE_TO_3       300
#define _3_TO_10        1200
#define EDGE_TO_6       600

//Initiate Servo
AerServo servo(10);
//Servo servo;

void setup() {
  servo.init();
}

void loop() {

  deploy(1, 1, 0);
  
  while(1); 

}

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

void deploy(int d2, int d1, int d0) {
  if (d2 == 1 and d1 == 1 and d0 == 1) {
    centre_crack_deploy();
  } else if (d2 == 0 and d1 == 1 and d0 == 1) {
    right_crack_deploy();
  } else if (d2 == 1 and d1 == 1 and d0 == 0) {
    left_crack_deploy();
  } else if (d2 == 0 and d1 == 1 and d0 == 0) {
    hole_deploy();
  }
}

void hole_deploy() {
  servo.to_middle();
  delay(1000);
  return;
}
void left_crack_deploy() {
  // Cone 1
  //servo.hard_right();
  servo.move_to_time(MIDDLE_TO_EDGE);
  servo.move_to_time(-EDGE_TO_3-300);
  delay(1000);
  // Cone 2
  servo.move_to_time(-_3_TO_10);
  delay(1000);
  servo.setCurr_pos(7);
}
void right_crack_deploy() {
  // Cone 1
  //servo.hard_left();
  servo.move_to_time(-MIDDLE_TO_EDGE);
  servo.move_to_time(EDGE_TO_3);
  delay(500);
  // Cone 2
  servo.move_to_time(_3_TO_10);
  delay(1000);
  servo.setCurr_pos(10);
}
void centre_crack_deploy() {
  // Cone 1
  servo.move_to_time(-MIDDLE_TO_EDGE);
  servo.move_to_time(EDGE_TO_6);
  delay(1000);
  // Cone 2
  servo.move_to_time(EDGE_TO_EDGE);
  servo.move_to_time(-EDGE_TO_6);
  delay(1000);
  servo.setCurr_pos(13);
}
void center_crack_deploy() {
  centre_crack_deploy();
}
