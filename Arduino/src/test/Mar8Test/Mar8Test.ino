#include "AerServo/AerServo.cpp"
#include "AerSteppers/AerSteppers.cpp"
#include "AerDCMotors/AerDCMotors.cpp"

//Interrupts
#define Detect_Interr 2

//Cone Deployment System Definitions 
#define Bottom_Left 7
#define Bottom_Centre 3
#define Bottom_Right 8
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
volatile boolean first = true;

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
int hole = NA;
int left_crack = NA;
int right_crack = NA; 
int hole_or_crack = NA;
volatile int crack_position = NA; 
void detection_ISR();

//Initiate DC Motors
// AerDCMotors(int pinL1, int pinL2, int pwmL, int pwmR)
// pinR1 = A4, pinR2 = A5
AerDCMotors dc(5, 6, 9, 11);

//Initiate Servo
AerServo servo(10); 

//Initiate Steppers, pins A0:3
AerSteppers stepp(14,15,16,17); 

void detection_ISR(); 

void setup() {
  dc.init();
  servo.init(); 
  stepp.init();

  //Interrupts 
  pinMode(Bottom_Centre, INPUT);
  attachInterrupt(digitalPinToInterrupt(Bottom_Centre), detection_ISR, RISING);
  Serial.begin(9600);
  Serial.print("start");
  Serial.print("\n");
  
}

void loop() {
  // Move forward
  dc.forward(255);
  while (1){
      if (cone_deploy){
        cone_deployment();
//        dc.forward(255);
        cone_deploy = false; 
        Serial.print("done deploy");
      }
  }
//  servo.move_to(9.5); 
//  stepp.drop_cone();

 while(1); 
}

//Interrupt sequence if cone detected

     hole = NA;
     left_crack = NA; 
     right_crack = NA; 
     return;
}
*/

unsigned long lastInterrupt = 0;
void detection_ISR(){

    // To prevent repeated input due to bad sensor signal
    if (millis() - lastInterrupt > 1) {
        
        Serial.print("here");
        Serial.print("\t");
        
        int sensors = 100*digitalRead(Bottom_Right) + 10*digitalRead(Bottom_Centre) + digitalRead(Bottom_Left);
        Serial.print(sensors);
        Serial.print("\n");

        if (sensors == 10) { // hole
          dc.stop();
          cone_deploy = true; 
          hole = YES; 
          hole_or_crack = Hole; 
          Serial.print("Hole");
          Serial.print("\t");
        } else if (sensors == 110) {  // right crack
          dc.stop();
          crack_position = RIGHT;
          cone_deploy = true; 
          right_crack = YES;
          hole_or_crack = Crack;
          Serial.print("Right Crack");
          Serial.print("\t");
        } else if (sensors == 11) {  // left crack
          dc.stop();
          crack_position = LEFT;
          cone_deploy = true; 
          left_crack = YES; 
          hole_or_crack = Crack;
          Serial.print("Left Crack");
          Serial.print("\t");
        } else if (sensors == 111) {  // centre crack
           dc.stop();
          crack_position = CENTRE;
          cone_deploy = true; 
          hole_or_crack = Crack; 
          Serial.print("Centre Crack");
          Serial.print("\t");
        }
        
    }

    lastInterrupt = millis();
    hole = NA;
    left_crack = NA; 
    right_crack = NA; 
    return;
}

//
//Cone Deployment Code//
void cone_deployment(){
  Serial.print(hole_or_crack);
  if (hole_or_crack ==  Hole){
    hole_or_crack = NA; 
    hole_deploy();
  }
  else if (hole_or_crack == Crack){
    hole_or_crack = NA; 
    crack_deploy();
  }
  return;
}


void crack_deploy(){
//    delay(1000);
//    dc.forward(255);
//    delay(5000);
    dc.stop();     
    //Deploying Cone 
    if (crack_position == LEFT){
         servo.displace_right(2.1);
         stepp.drop_cone();
         servo.displace_right(10.5); //check for setting current_position inside code
         stepp.drop_cone();
    }else if(crack_position == RIGHT){
         servo.hard_right();
         servo.displace_left(2.1);
         stepp.drop_cone();
         servo.displace_left(10.5); //check for setting current_position inside code
         stepp.drop_cone();
    }else{               //All three sensors triggered
         servo.move_to(4.5); 
         stepp.drop_cone(); 
         servo.displace_right(10.5); //check for setting current_position inside code
         stepp.drop_cone(); 
    }    
    return;
} 

void hole_deploy(){
//    delay(1000); 
//    dc.forward(255);
//    delay(5000);
    dc.stop();
//     servo.to_middle()
    servo.move_to(9.5);
    stepp.drop_cone();
    return;
}
