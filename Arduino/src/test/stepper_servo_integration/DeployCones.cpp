/*
#include "AerServo/AerServo.cpp"
#include "AerSteppers/AerSteppers.cpp"

#define HOLE 1
#define LEFT_CRACK 2
#define CENTER_CRACK 3
#define RIGHT_CRACK 4

void deploy_cones(AerServo servo, AerSteppers steppers, int obstruction) {
    
    switch(obstruction) {
        case HOLE:
            steppers.drop_cone();   //8.5 - 17.5
            break;
            
        case LEFT_CRACK:
            servo.hard_left();
            steppers.drop_cone();   //0 - 9
            
            servo.move_right();
            delay(servo._t_right_10);
            steppers.drop_cone();    //10 - 19
            
            break;
            
        case CENTER_CRACK:
            
            servo.hard_left();
            servo.move_right();
            delay(servo._t_left_3);
            steppers.drop_cone();   //3 - 12
            
            servo.hard_right();
            servo.move_left();
            delay(servo._t_right_3);
            steppers.drop_cone();   //13 - 22
            
            break;
            
        case RIGHT_CRACK:
            servo.hard_right();
            steppers.drop_cone();   //7 - 16
            
            servo.move_left();
            delay(servo._t_left_10);
            steppers.drop_cone();    //17 - 26
            
            break;
    }
}
*/
