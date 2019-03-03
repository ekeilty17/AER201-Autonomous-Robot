#include "AerServo/AerServo.cpp"
#include "AerSteppers/AerSteppers.cpp"
//#include "DeployCones.cpp"

AerServo servo(6);
AerSteppers steppers(2, 3, 4, 5);

void setup() {
  servo.init();
  steppers.init(); 
}



void loop() {

  //precision_test(servo, steppers);

  servo.move_left(2000);
  steppers.drop_cone();
  
//  delay(2000);
//  servo.move_to(10);
//  delay(1000);
//  cone_placement_test(servo, steppers, HOLE);
//  delay(1000);
//  servo.displace_right(8.5);
  
  while(1); 
}

void precision_test(AerServo servo, AerSteppers steppers) {
  
  delay(2000);
  steppers.drop_cone();
  
  servo.displace_right(13);
  delay(1000);
  steppers.drop_cone();
  
  servo.displace_left(6.5);

  // Cones should be 1cm apart
  
}
