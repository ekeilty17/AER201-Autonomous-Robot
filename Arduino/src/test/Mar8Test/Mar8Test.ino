#include "AerServo/AerServo.cpp"
#include "AerSteppers/AerSteppers.cpp"
#include "AerDCMotors/AerDCMotors.cpp"

//Interrupts
#define Detect_Interr 2

//Cone Deployment System Definitions 
#define Bottom_Left 7
#define Bottom_Centre 3
#define Bottom_Right 8

#define NO 0
#define YES 1

#define HOLE 10
#define LEFT_CRACK 11
#define RIGHT_CRACK 110
#define CENTRE_CRACK 111

#define Hole 26
#define Crack 27

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

//Initialize global variable queues
//Queue Code//
struct Queue {
    int store[4]; /*the array with the elements*/
    int max_size;
    int size;
    int start;
};
typedef struct Queue Queue;

int isEmpty(Queue x); 
int deq(Queue *x, int *pval);
int enq(Queue *x, int val);
int init(Queue *x); 

int r=0;
Queue HC_q;
Queue HC_Dist_q; 
Queue C_Pos_q; 

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
  init(&HC_q);
  
  //Interrupts 
  pinMode(Bottom_Centre, INPUT);
  attachInterrupt(digitalPinToInterrupt(Bottom_Centre), detection_ISR, RISING);
  Serial.begin(9600);
  Serial.print("start");
  Serial.print("\n");
  Serial.print("\n");
  
}

void loop() {
  // Move forward
  dc.forward(255);
  while (1){
      if (isEmpty(HC_q)){
        Serial.print("Hey");
        cone_deployment();
//        dc.forward(255);
        cone_deploy = false; 
        Serial.print("done deployment");
        Serial.print("\n");
      }
  }
//  servo.move_to(9.5); 
//  stepp.drop_cone();

 while(1); 
}

//Interrupt sequence if cone detected
unsigned long lastInterrupt = 0;
void detection_ISR(){

    // To prevent repeated input due to bad sensor signal
    if (millis() - lastInterrupt > 1) {
        
        int sensors = 100*digitalRead(Bottom_Right) + 10*digitalRead(Bottom_Centre) + digitalRead(Bottom_Left);
        Serial.print(sensors);
        Serial.print("\n");

        if (sensors == 10) { // hole
          dc.stop();
          cone_deploy = true; 
          Serial.print(HC_q.store[0]);
          r=enq(&HC_q,Hole); // Global variable queue - hole or crack queue - enqueue(Hole)
          Serial.print(HC_q.store[0]);
          hole = YES; 
          hole_or_crack = Hole; 
          Serial.print("Hole");
          Serial.print("\t");
        } else if (sensors == 110) {  // right crack
          dc.stop();
          crack_position = RIGHT;
          cone_deploy = true; 
          right_crack = YES;
          r=enq(&HC_q,Crack); // Global variable queue - hole or crack queue - enqueue(Crack)
          hole_or_crack = Crack;
          Serial.print("Right Crack");
          Serial.print("\t");
        } else if (sensors == 11) {  // left crack
          dc.stop();
          crack_position = LEFT;
          cone_deploy = true; 
          left_crack = YES; 
          hole_or_crack = Crack;
          r=enq(&HC_q, Crack); // Global variable queue - hole or crack queue - enqueue(Crack)
          Serial.print("Left Crack");
          Serial.print("\t");
        } else if (sensors == 111) {  // centre crack
          dc.stop();
          crack_position = CENTRE;
          cone_deploy = true; 
          hole_or_crack = Crack; 
          r=enq(&HC_q,Crack); // Global variable queue - hole or crack queue - enqueue(Crack)
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
  int rval = 0; 
  r=deq(&HC_q,&rval);
  if (rval ==  Hole){
    hole_or_crack = NA; 
    hole_deploy();
  }
  else if (rval == Crack){
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
    } else if (crack_position == CENTRE) {
         servo.displace_right(5.1);
         stepp.drop_cone();
         servo.hard_right();
         servo.displace_left(5.1);
         stepp.drop_cone();
    } else{               //All three sensors triggered
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
    //servo.move_to(9.5);
    stepp.drop_cone();
    return;
}

//Queue Code//
int init(Queue *x)
{
    if (x == NULL)
    {
        return -1;
    }
    x->max_size = 4;
    x->size = 0;
    x->start = 0;
    return 0;
}

int enq(Queue *x, int val)
{
    if (x == NULL)
    {
        return -1;
    }
    if (x->size == x->max_size)
    {
        /*blew the memory*/
        return -1;
    }
    x->store[(x->start + x->size) % x->max_size] = val;
    x->size += 1;
    return 0;
}

/*pval is a return value*/
int deq(Queue *x, int *pval)
{
    int i=0;
    if (x == NULL)
    {
        return -1;
    }
    if (pval == NULL)
    {
        return -1;
    }
    if (x->size == 0)
    {
        return -1;
    }
    *pval = x->store[x->start];
    x->store[x->start] = 0;
    x->start = (x->start + 1) % x->max_size;
    x->size -= 1;
    return 0;
}

int isEmpty(Queue x)
{
    if (r == NULL)
    {
        return -1;
    }
    if (x.size == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
