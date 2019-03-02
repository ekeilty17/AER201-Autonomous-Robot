#define Left_Wheel_GO 6
#define Right_Wheel_GO 8
#define Left_Wheel_Dir 7
#define Right_Wheel_Dir 9
#define FORWARD LOW
#define BACKWARD HIGH
#define OFF LOW
#define ON HIGH

void setup() {
  //Set up drive system pins
  pinMode(Left_Wheel_Dir,OUTPUT); 
  pinMode(Right_Wheel_Dir,OUTPUT); 
  pinMode(Left_Wheel_GO,OUTPUT); 
  pinMode(Right_Wheel_GO,OUTPUT); 

}

void loop() {
  digitalWrite(Left_Wheel_Dir,FORWARD);
  digitalWrite(Right_Wheel_Dir,BACKWARD); 
  digitalWrite(Left_Wheel_GO,ON);
  digitalWrite(Right_Wheel_GO,ON);
  delay(3000);
  while(1);
//  digitalWrite(Left_Wheel_GO,ON);
//  digitalWrite(Right_Wheel_GO,OFF);
//  delay(3000);
//  digitalWrite(Left_Wheel_GO,OFF);
//  digitalWrite(Right_Wheel_GO,ON);
//  delay(3000);
}
