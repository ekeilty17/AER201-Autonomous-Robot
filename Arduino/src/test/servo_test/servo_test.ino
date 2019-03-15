#include "AerServo/AerServo.cpp"

#define RIGHT true
#define LEFT false

//Initiate Servo
AerServo servo(10);

void setup() {
  servo.init();
}

void loop() {

//  servo.setCurr_pos(19.0); 
//  servo.hard_left();
  delay(2000); 
  servo.move_right();
  delay(1000);
  servo.stop();
  delay(1000);
  servo.move_left();
  delay(1000);
  servo.stop();

//  delay(1000);
//  servo.displace_right(10);
  //displacement_test(servo, 1200, RIGHT);
//  self_consistency_test(servo, 10, LEFT);
  
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
