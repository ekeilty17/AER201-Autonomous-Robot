#include "AerServo/AerServo.cpp"
#include "AerSteppers/AerSteppers.cpp"
#include "AerDCMotors/AerDCMotors.cpp"

//Cone Deployment System Definitions 
#define Bottom_Left 4
#define Bottom_Centre 5
#define Bottom_Right 6
#define Hole 26
#define Crack 27
#define NO 0
#define YES 1
#define LEFT 11
#define RIGHT 12
#define CENTRE 13
#define NA 14

//Interrupts
#define Detect_Interr 3

//Cone Deployment System Definitions 
#define Bottom_Left 4
#define Bottom_Centre 5
#define Bottom_Right 6
#define Hole 26
#define Crack 27
#define NO 0
#define YES 1
#define LEFT 11
#define RIGHT 12
#define CENTRE 13
#define NA 14

//Global variables for primary initializing 
volatile int current_position = 0;
volatile int number_of_cones_deployed = 0;
volatile boolean cone_deploy = false;

//Cone Deployment Functions
void cone_deployment();
void crack_deploy();
void hole_deploy();

//Detection ISR Variables 
volatile int last_obstruction = 0;
volatile float holes_distance[100];
volatile float cracks_distance[100];
volatile int holes_n = 0;
volatile int cracks_n = 0;
volatile int hole_or_crack = NA;
volatile int crack_position = NA; 
void detection_ISR();

//Initiate DC Motors
// AerDCMotors(int pinL1, int pinL2, int pwmL, int pwmR)
// pinR1 = A4, pinR2 = A5
AerDCMotors dc(8, 10, 9, 11);

//Initiate Servo
AerServo serv(6); 

//Initiate Steppers, pins A0:3
AerSteppers stepp(); 

void setup() {
  dc.init();

  serv.init(); 

  stepp.init();
  
  //Interrupts 
  pinMode(Detect_Interr, INPUT);
  attachInterrupt(digitalPinToInterrupt(Detect_Interr), detection_ISR, RISING);
}

void loop() {
  // Move forward
  dc.forward(255);
  while (1){
      if (cone_deploy){
        cone_deployment();
        dc.forward(255);
        cone_deploy = false; 
      }
}

//Interrupt sequence if cone detected 
void detection_ISR(){
     //Immediately stop
     dc.stop(); 

     if (digitalRead(Bottom_Left)==0 && digitalRead(Bottom_Centre)==1 && digitalRead(Bottom_Right)==0){    //indicates hole detected
         cone_deploy = true; 
         hole_or_crack = Hole; 
     }
     else{                                           //2 or 3 sensors triggered - It's a crack
       if (digitalRead(Bottom_Left)==1 && digitalRead(Bottom_Centre)==1 && digitalRead(Bottom_Right)==0){  //Left crack   
           crack_position = LEFT;
       }if (digitalRead(Bottom_Left)==1 && digitalRead(Bottom_Centre)==1 && digitalRead(Bottom_Right)==1){  //Centre crack
           crack_position = CENTRE;
       }if (digitalRead(Bottom_Left)==0 && digitalRead(Bottom_Centre)==1 && digitalRead(Bottom_Right)==1){  //Right crack
           crack_position = RIGHT;
       }
       cone_deploy = true; 
       hole_or_crack = Crack; 
    }
    return;
}

//Cone Deployment Code//
void cone_deployment(){
  if (hole_or_crack ==  Hole){
    hole_deploy();
  }
  else if (hole_or_crack == Crack){
    crack_deploy();
  }
  return;
}


void crack_deploy(){
    //NEED TO DETERMINE TIMING TO MOVE MACHINE FORWARD
    
    //Deploying Cone 
    if (crack_position == LEFT){
         current_position = serv.hard_left();
         stepp.drop_cone();
         serv.displace_right(5.0); //check for setting current_position inside code
         stepp.drop_cone();
    }else if(crack_position == RIGHT){
         current_position = serv.hard_right();
         stepp.drop_cone(); 
         serv.displace_left(5.0); //check for setting current_position inside code
         stepp.drop_cone(); 
    }else{               //All three sensors triggered
         serv.move_to(3.0); 
         stepp.drop_cone(); 
         serv.displace_right(5.0); //check for setting current_position inside code
         stepp.drop_cone(); 
    }    
    return;
} 

void hole_deploy(){
    //NEED TO DETERMINE TIMING TO MOVE MACHINE FORWARD
    
    serv.middle(); 
    stepp.drop_cone();
    return;
}
