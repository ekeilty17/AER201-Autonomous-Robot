#include <Servo.h> 

#define Left_Wheel_GO 7
#define Right_Wheel_GO 9
#define Left_Wheel_Dir 8
#define Right_Wheel_Dir 10
#define FORWARD LOW
#define BACKWARD HIGH
#define OFF LOW
#define ON HIGH
#define VELOCITY 153

int curr_pos = 0;

//Servo
Servo servo; 
void Move_Servo(int current,int final_pos); 

//Stepper Functions
void Move_Stepper();
void full_drive();

int displace_pos(int dx, int dir){

  int t = dx/VELOCITY;

  if (dir == 1){
    servo.write(0); 
    delay(t);
    curr_pos += dx;
  }
  if (dir == -1){
    servo.write(180); 
    delay(t);
    curr_pos -= dx;
  }

  return; 
}

int move_to_pos(int x){
  int curr;
  int next_pos = abs(x - curr);
  int dir = abs(x - curr)/(x - curr);
  int t = (x-curr_pos)/VELOCITY;

  if (dir == 1){
    servo.write(0); 
    delay(t);
  }
  if (dir == -1){
    servo.write(180); 
    delay(t);
  }

  curr_pos = next_pos;
  
  return; 
}

void setup() {
  //Set up stepper pins
  pinMode(2,OUTPUT); 
  pinMode(3,OUTPUT); 
  pinMode(4,OUTPUT); 
  pinMode(5,OUTPUT); 

//  //Set up drive system pins
//  pinMode(Left_Wheel_Dir,OUTPUT); 
//  pinMode(Right_Wheel_Dir,OUTPUT); 
//  pinMode(Left_Wheel_GO,OUTPUT); 
//  pinMode(Right_Wheel_GO,OUTPUT);

  //Set up servo
  servo.attach(6);
}

void loop() {
  delay(2000);
  
  //Start in position 1, Drop 1 cone 
  for(int i=0;i<64;i++){    // 65 Steps = 45º turn 
      full_drive();
  }

  delay(2000);

  displace_pos(10,1);

  delay(1000);
  
  for(int i=0;i<64;i++){    // 65 Steps = 45º turn 
      full_drive();
  }

  delay(2000);
  
//  delay(2000);
//  digitalWrite(Left_Wheel_Dir,FORWARD);
//  digitalWrite(Right_Wheel_Dir,FORWARD); 
//  digitalWrite(Left_Wheel_GO,ON);
//  digitalWrite(Right_Wheel_GO,ON);
//  delay(5000);
//  digitalWrite(Left_Wheel_GO,OFF);
//  digitalWrite(Right_Wheel_GO,OFF);
  while(1);
}


void full_drive (){
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
    delay(3);
    digitalWrite(2,LOW);
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);
    delay(3);
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
    delay(3);
    digitalWrite(2,HIGH);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
    delay(3);
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
    delay(3);     
}
