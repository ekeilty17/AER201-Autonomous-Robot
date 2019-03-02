#include <Servo.h> 

//Servo
Servo servo; 
void Move_Servo(int current,int final_pos); 

void setup() {
  //Set up servo
  servo.attach(6);
}

void loop() {
  servo.write(0);    //one direction for 3 seconds
  delay(1158);
  servo.write(90);   //stop
  while(1); 
//  delay(1000); 
//  servo.write(180);   //opposite direction for 3 seconds 
//  delay(2000);
//  servo.write(90);   //stop
}
