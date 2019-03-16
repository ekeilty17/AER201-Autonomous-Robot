#include "AerServo/AerServo.cpp"
#include "AerSteppers/AerSteppers.cpp"
#include "AerDCMotors/AerDCMotors.cpp"
#include "Queue/Queue.cpp"

//Cone Deployment System Definitions 
#define SENSOR_LEFT       7
#define SENSOR_RIGHT     8
#define SENSOR_CENTRE     3     // Interrupt pin
#define SENSOR_CENTER     3     // Interrupt pin

#define HOLE              10
#define LEFT_CRACK        11
#define RIGHT_CRACK       110
#define CENTRE_CRACK      111
#define CENTER_CRACK      111

//Global variables for primary initializing 
volatile int current_position = 0;
volatile int number_of_cones_deployed = 0;

//Cone Deployment Functions
void cone_deployment();
void hole_deploy();
void left_crack_deploy();
void right_crack_deploy();
void centre_crack_deploy();
void center_crack_deploy();

//Detection ISR Variables 
volatile int last_obstruction = 0;
volatile float holes_distance[100];
volatile float cracks_distance[100];
volatile int holes_n = 0;
volatile int cracks_n = 0;
volatile int crack_position = NULL;

// ISR
void detection_ISR();

//Initialize Queues
Queue HC_q(10);
Queue HC_Dist_q(); 
Queue C_Pos_q(); 

//Initialize DC Motors
// AerDCMotors(int pinL1, int pinL2, int pinR1, int pinR2)
AerDCMotors dc(5, 6, 9, 11);

//Initialize Servo
AerServo servo(10); 

//Initialize Steppers, pins A0:3
AerSteppers stepp(14,15,16,17); 

void detection_ISR(); 

void setup() {
  // Initialize internal variables of classes
  dc.init();
  servo.init(); 
  stepp.init();
  HC_q.init();
  
  //Set Interrupt pin
  pinMode(SENSOR_CENTRE, INPUT);
  attachInterrupt(digitalPinToInterrupt(SENSOR_CENTRE), detection_ISR, RISING);
  Serial.begin(9600);
  
  Serial.print("start");
  Serial.print("\n");
  Serial.print("\n");
  
}

void loop() {
  while (1){
    Serial.print(HC_q.isEmpty());
    delay(1000);
    if (HC_q.isEmpty() == false){
      Serial.print("\n");
      Serial.print("begin deployment");
      Serial.print("\n");
      cone_deployment();
      Serial.print("done deployment");
      Serial.print("\n");
      Serial.print("\n");
    }
  }
}

//Interrupt sequence if cone detected
unsigned long lastInterrupt = 0;
void detection_ISR(){

    // To prevent repeated input due to bad sensor signal
    if (millis() - lastInterrupt > 1) {

        Serial.print("\n");
        Serial.print("Entered ISR");
        Serial.print("\n");
        
        int obstruction = 100*digitalRead(SENSOR_RIGHT) + 10*digitalRead(SENSOR_CENTRE) + digitalRead(SENSOR_LEFT);
        HC_q.enq(obstruction);

        Serial.print("Detected: ");
        Serial.print(obstruction);
        Serial.print("\t");
        int *q = HC_q.getQueue();
        for (int i=0; i<HC_q.getSize(); i++) {
          Serial.print(q[i]);
          Serial.print(" ");
        }
        Serial.print("\t");
        Serial.print("'HC_q.isEmpty()' = ");
        Serial.print(HC_q.isEmpty());
        Serial.print("\n");
    }

    lastInterrupt = millis();
    return;
}

//
//Cone Deployment Code//
void cone_deployment(){
  
//  delay(1000); 
//  dc.forward(255);
//  delay(5000);
  dc.stop();

  int obstruction = HC_q.deq();
  Serial.print("Deploying obstruction: ");
  Serial.print(obstruction);
  Serial.print("\n");
  
  switch(obstruction) {
    case HOLE:
      hole_deploy();
      break;
    case LEFT_CRACK:
      left_crack_deploy();
      break;
    case RIGHT_CRACK:
      right_crack_deploy();
      break;
    case CENTRE_CRACK:
      centre_crack_deploy();
      break;
    default:
      break;
  }
  return;
}

void hole_deploy() {
  Serial.print("In 'hole_deploy()'");
  Serial.print("\n");
//  servo.to_middle();
  servo.move_to(9.5);
  stepp.drop_cone();
  servo.hard_left();
  return;
}

void left_crack_deploy() {
  Serial.print("In 'left_crack_deploy()'");
  Serial.print("\n");
  servo.displace_right(2.1);
  stepp.drop_cone();
  servo.displace_right(10.5); //check for setting current_position inside code
  stepp.drop_cone();
}

void right_crack_deploy() {
  Serial.print("In 'right_crack_deploy()'");
  Serial.print("\n");
  servo.hard_right();
  servo.displace_left(2.1);
  stepp.drop_cone();
  servo.displace_left(10.5); //check for setting current_position inside code
  stepp.drop_cone();
}

void centre_crack_deploy() {
  Serial.print("In 'centre_crack_deploy()'");
  Serial.print("\n");
  servo.displace_right(5.1);
  stepp.drop_cone();
  servo.hard_right();
  servo.displace_left(5.1);
  stepp.drop_cone();
}
void center_crack_deploy() {
  centre_crack_deploy();
}
