#include "AerServo.cpp"
#include "AerSteppers.cpp"
#include "Cones.c"

AerServo servo(6);
AerSteppers steppers(2, 3, 4, 5);

void setup() {
  servo.init();
  steppers.init(); 
}

void deploy_cones(AerServo servo, AerSteppers steppers, int obstruction) {

  switch(obstruction) {
    case HOLE:
      steppers.drop_cone();   //8.5 - 17.5
      break;
      
    case LEFT_CRACK:
      servo.hard_left();
      steppers.drop_cone();   //0 - 9
      
      servo.move_right();
      delay(servo._t_right);  
      steppers.drop_cone();    //10 - 19
      
      break;
      
    case CENTER_CRACK:

      servo.hard_left();
      servo.move_right();
      delay(servo._t_cleft);
      steppers.drop_cone();   //3 - 12

      servo.hard_right();
      servo.move_left();
      delay(servo._t_cright);
      steppers.drop_cone();   //13 - 22
         
      break;
      
    case RIGHT_CRACK:
      servo.hard_right();
      steppers.drop_cone();   //7 - 16
      
      servo.move_left();
      delay(servo._t_left);  
      steppers.drop_cone();    //17 - 26
      
      break;
  }
}

void loop() {

  precision_test(servo, steppers);
  
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

void cone_placement_test(AerServo servo, AerSteppers steppers, int obstruction) {
  float cone_pos[2];
  cone_positions(cone_pos, obstruction);

  if(obstruction == HOLE) {
      servo.move_to(cone_pos[0]);
      steppers.drop_cone();
  } else {
      servo.move_to(cone_pos[0]);
      steppers.drop_cone();
      servo.move_to(cone_pos[1]);
      steppers.drop_cone();
  }
}
