 /**
 * @file
 * @author Nkemjika Okeke
 * 
 * Date: February 23, 2019 2:46 PM
 * 
 * @brief Entire AutoCone12 Program - Arduino Nano Side
 * 
 * Preconditions:
 * @pre The Co-processor is not driving lines on the UART bus (e.g. the JP_TX
 *      and JP_RX jumpers are removed)
 * @pre PIC-Arduino link switches are enabled (ON) for D1 of the Arduino (the RX
 *      pin). However, make sure that neither of D0 and D1 are enabled (ON) 
 *      while programming the Arduino Nano
 */
// Includes
#include <SoftwareSerial.h>
#include "AerServo/AerServo.h"
#include "AerSteppers/AerSteppers.h"


//Drive System Definitions
#define Left_Wheel_GO 7
#define Right_Wheel_GO 9
#define Left_Wheel_Dir 8
#define Right_Wheel_Dir 10
#define FORWARD LOW
#define BACKWARD HIGH


//General Definitions
#define ON HIGH
#define OFF LOW


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

//Steering System Definitions 
#define Left_Sensor 0
#define Right_Sensor 1

//Detection ISR Definitions 
#define Light 11

//Interrupts
#define Drive_Interr 2
#define Detect_Interr 3

// Read-only
const byte rxPin = 1;
const byte txPin = 0;

// Set up objects
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

//Global variables for primary initializing 
volatile int current_position = 0;
volatile int number_of_cones_deployed = 0; 
volatile float total_distance = 0; 
volatile int cone_deployed = NO; 
  
//Variables for sending PIC Arduino status
unsigned char stat = 'N'; 

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
volatile int cone_deployment_func = 0; 
void detection_ISR();

//Drive System ISR
void drive_system_ISR();

//Stepper Functions
void Move_Stepper();
void full_drive();

//Servo
Servo servo; 
void Move_Servo(int current,int final_pos); 

void setup() {
  //Setup for communication with PIC
  // Configure pin modes for tx and rx
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
    
  // Open software serial port with baud rate = 9600
  mySerial.begin(9600);

  //Set up drive system pins
  pinMode(Left_Wheel_Dir,OUTPUT); 
  pinMode(Right_Wheel_Dir,OUTPUT); 
  pinMode(Left_Wheel_GO,OUTPUT); 
  pinMode(Right_Wheel_GO,OUTPUT); 

  //Set up Detection ISR pins
  pinMode(Light,OUTPUT);

  //Set up stepper pins
  pinMode(A0,OUTPUT); 
  pinMode(A1,OUTPUT); 
  pinMode(A2,OUTPUT); 
  pinMode(A3,OUTPUT); 

  //Set up servo
  servo.attach(5);

  //Interrupts
  pinMode(Detect_Interr, INPUT);
  attachInterrupt(digitalPinToInterrupt(Detect_Interr), detection_ISR, RISING);
  pinMode(Drive_Interr, INPUT);
  attachInterrupt(digitalPinToInterrupt(Drive_Interr), drive_system_ISR, CHANGE);
  
}

void loop() {
    while (1){
      // Wait to receive the message from the main PIC
      if(mySerial.available() > 0){
          stat = mySerial.read();
      }
      if (stat == 'A'){
         break;
      }
    }
    digitalWrite(Left_Wheel_Dir,FORWARD);
    digitalWrite(Right_Wheel_Dir,FORWARD); 
    digitalWrite(Left_Wheel_GO,ON);
    digitalWrite(Right_Wheel_GO,ON);
    while (1){
      if (~isEmpty(HC_q)){
        cone_deployment();
        if (isEmpty(HC_q)){
          digitalWrite(Left_Wheel_Dir,FORWARD);
          digitalWrite(Right_Wheel_Dir,FORWARD); 
          digitalWrite(Left_Wheel_GO,ON);
          digitalWrite(Right_Wheel_GO,ON);
        }
      }
      if(mySerial.available() > 0){
          total_distance = mySerial.read();
          if (number_of_cones_deployed == 12){
            break;
          }
          if (total_distance >= 400){
            break;
          }
      }
    }
    digitalWrite(Left_Wheel_GO,OFF);
    digitalWrite(Right_Wheel_GO,OFF);
    //Turn off all interrupts
    detachInterrupt(digitalPinToInterrupt(Detect_Interr));
    detachInterrupt(digitalPinToInterrupt(Drive_Interr)); 
    //Get to the end
    digitalWrite(Left_Wheel_GO,OFF);
    digitalWrite(Right_Wheel_GO,OFF);
    while(1){
      if(mySerial.available() > 0){
            total_distance = mySerial.read();
      }
      if (total_distance == 440){
            digitalWrite(Left_Wheel_GO,OFF);
            digitalWrite(Right_Wheel_GO,OFF);
            break;
      }
    }
    right_pivot_turn();
    digitalWrite(Left_Wheel_Dir,FORWARD);
    digitalWrite(Right_Wheel_Dir,FORWARD);
    digitalWrite(Left_Wheel_GO,ON);
    digitalWrite(Right_Wheel_GO,ON);
    while(1){
      if(mySerial.available() > 0){
            total_distance = mySerial.read();
      }
      if (total_distance == 480){
            digitalWrite(Left_Wheel_GO,OFF);
            digitalWrite(Right_Wheel_GO,OFF);
            break;
      }
    }
    right_pivot_turn();
    digitalWrite(Left_Wheel_Dir,FORWARD);
    digitalWrite(Right_Wheel_Dir,FORWARD);
    digitalWrite(Left_Wheel_GO,ON);
    digitalWrite(Right_Wheel_GO,ON);
    while(1){
      if(mySerial.available() > 0){
            total_distance = mySerial.read();
      }
      if (total_distance == 920){
            digitalWrite(Left_Wheel_GO,OFF);
            digitalWrite(Right_Wheel_GO,OFF);
            break;
      }
    }
    right_pivot_turn();
    digitalWrite(Left_Wheel_Dir,FORWARD);
    digitalWrite(Right_Wheel_Dir,FORWARD);
    digitalWrite(Left_Wheel_GO,ON);
    digitalWrite(Right_Wheel_GO,ON);
    while(1){
      if(mySerial.available() > 0){
            total_distance = mySerial.read();
      }
      if (total_distance == 960){
            digitalWrite(Left_Wheel_GO,OFF);
            digitalWrite(Right_Wheel_GO,OFF);
            break;
      }
    }
    right_pivot_turn();
    digitalWrite(Left_Wheel_Dir,FORWARD);
    digitalWrite(Right_Wheel_Dir,FORWARD);
    digitalWrite(Left_Wheel_GO,ON);
    digitalWrite(Right_Wheel_GO,ON);
    while(1){
      if(mySerial.available() > 0){
            total_distance = mySerial.read();
      }
      if (total_distance == 990){
            digitalWrite(Left_Wheel_GO,OFF);
            digitalWrite(Right_Wheel_GO,OFF);
            break;
      }
    }
    mySerial.write('A');    //Send "stop" signal to PIC
    
    //Send PIC all Operation Data
    mySerial.write(number_of_cones_deployed);
    mySerial.write(cracks_n);
    mySerial.write(holes_n);
    for (int i=0;i<cracks_n;i++){
      mySerial.write(cracks_distance[i]);
    }
    for (int i=0;i<holes_n;i++){
      mySerial.write(holes_distance[i]);
    }
}

//Detection ISR
void detection_ISR(){
     digitalWrite(Left_Wheel_GO,OFF);
     digitalWrite(Right_Wheel_GO,OFF);
     //Internal variable for position only
     int crack_position = NA; 
     digitalWrite(Light,ON);  //turn on light
     delay(2000);
     digitalWrite(Light,OFF);
    if (digitalRead(Bottom_Left)==0 && digitalRead(Bottom_Centre)==1 && digitalRead(Bottom_Right)==0){    //indicates hole detected
        if (last_obstruction == Hole){
           if (total_distance - holes_distance[holes_n-1] >= 15 && number_of_cones_deployed < 12){  //check if distance between current position and last hole <= 15cm AND if you have cones left to deploy 
                  holes_n++;  
                  holes_distance[holes_n-1] = total_distance;    //save distance of current hole detected
                  last_obstruction = Hole;  
                  cone_deployment_func = YES; 
                  r=enq(&HC_q,Hole); // Global variable queue - hole or crack queue - enqueue(Hole)
                  r=enq(&HC_Dist_q,total_distance); // Global variable queue - crack/hole distance queue - enqueue(total_distance)
           }else{
                if (cone_deployed == NO && number_of_cones_deployed <12){
                    holes_n++;
                    holes_distance[holes_n-1] = total_distance;    //save distance of current hole detected 
                    last_obstruction = Hole; 
                    cone_deployment_func = YES; 
                    r=enq(&HC_q,Hole); // Global variable queue - hole or crack queue - enqueue(Hole)
                    r=enq(&HC_Dist_q,total_distance); // Global variable queue - crack/hole distance queue - enqueue(total_distance)
                }else{
                    holes_n++;  
                    holes_distance[holes_n-1] = total_distance;    //save distance of current hole detected 
                    last_obstruction = Hole; 
                }
            }
        }else{
           if (last_obstruction == Crack){
                if (total_distance - cracks_distance[cracks_n-1] >= 20){ 
                      holes_n++; 
                      holes_distance[holes_n-1] = total_distance;
                      last_obstruction = Hole; 
                      cone_deployment_func = YES; 
                      r=enq(&HC_q,Hole); // Global variable queue - hole or crack queue - enqueue(Hole)
                      r=enq(&HC_Dist_q,total_distance); // Global variable queue - crack/hole distance queue - enqueue(total_distance)
                }else{
                      if (cone_deployed == NO){
                          holes_n++;
                          holes_distance[holes_n-1] = total_distance;
                          last_obstruction = Hole; 
                          cone_deployment_func = YES; 
                          r=enq(&HC_q,Hole); // Global variable queue - hole or crack queue - enqueue(Hole)
                          r=enq(&HC_Dist_q,total_distance); // Global variable queue - crack/hole distance queue - enqueue(total_distance)
                      }else{
                           holes_n++; 
                           holes_distance[holes_n-1] = total_distance; 
                           last_obstruction = Hole; 
                      } 
                }
           }
        }
    }
    else{                                           //2 or 3 sensors triggered - It's a crack
       if (digitalRead(Bottom_Left)==1 && digitalRead(Bottom_Centre)==1 && digitalRead(Bottom_Right)==0)  //Left crack   
           crack_position = LEFT;          
       if (digitalRead(Bottom_Left)==1 && digitalRead(Bottom_Centre)==1 && digitalRead(Bottom_Right)==1)  //Centre crack
           crack_position = CENTRE;
       if (digitalRead(Bottom_Left)==0 && digitalRead(Bottom_Centre)==1 && digitalRead(Bottom_Right)==1)  //Right crack
           crack_position = RIGHT; 
       if (last_obstruction == Hole){
           if (total_distance - cracks_distance[cracks_n-1] >= 20 && number_of_cones_deployed < 12){  
                cracks_n++;
                cracks_distance[cracks_n-1] = total_distance;
                last_obstruction = Crack; 
                cone_deployment_func = YES; 
                r=enq(&HC_q,Crack); // Global variable queue - hole or crack queue - enqueue(Crack)
                r=enq(&HC_Dist_q,total_distance); // Global variable queue - crack/hole distance queue - enqueue(total_distance)
                r=enq(&C_Pos_q,crack_position);// Global variable queue - crack location queue - enqueue(crack_position)
           }else{
                if (cone_deployed == NO && number_of_cones_deployed < 12){
                    cracks_n++;
                    cracks_distance[cracks_n-1] = total_distance;
                    last_obstruction = Crack;
                    cone_deployment_func = YES; 
                    r=enq(&HC_q,Crack); // Global variable queue - hole or crack queue - enqueue(Crack)
                    r=enq(&HC_Dist_q,total_distance); // Global variable queue - crack/hole distance queue - enqueue(total_distance)
                    r=enq(&C_Pos_q,crack_position);// Global variable queue - crack location queue - enqueue(crack_position)          
                }else{
                    cracks_n++;  
                    cracks_distance[cracks_n-1] = total_distance; 
                    last_obstruction = Crack; 
                }
           }
       }else{
           if (last_obstruction == Crack){
                if (total_distance - cracks_distance[cracks_n-1] >= 10 && number_of_cones_deployed < 12){
                      cracks_n++; 
                      cracks_distance[cracks_n-1] = total_distance;
                      last_obstruction = Crack;
                      cone_deployment_func = YES; 
                      r=enq(&HC_q,Crack); // Global variable queue - hole or crack queue - enqueue(Crack)
                      r=enq(&HC_Dist_q,total_distance); // Global variable queue - crack/hole distance queue - enqueue(total_distance)
                      r=enq(&C_Pos_q,crack_position);// Global variable queue - crack location queue - enqueue(crack_position)
                }else{
                      if (cone_deployed == NO && number_of_cones_deployed < 12){
                           cracks_n++; 
                           cracks_distance[cracks_n-1] = total_distance;
                           last_obstruction = Crack;
                           cone_deployment_func = YES; 
                           r=enq(&HC_q,Crack); // Global variable queue - hole or crack queue - enqueue(Crack)
                           r=enq(&HC_Dist_q,total_distance); // Global variable queue - crack/hole distance queue - enqueue(total_distance)
                           r=enq(&C_Pos_q,crack_position);// Global variable queue - crack location queue - enqueue(crack_position)
                      }else{
                           cracks_n++; 
                           cracks_distance[cracks_n-1] = total_distance; 
                           last_obstruction = Crack; 
                      }
                }
           }
       }
    }
    return;
}

//Drive System ISR
void drive_system_ISR(){
   if(digitalRead(Left_Sensor)){   //left sensor detected black
        digitalWrite(Left_Wheel_GO,OFF);
   }
   else if(digitalRead(!Left_Sensor)){    //left sensor detects white         
        digitalWrite(Left_Wheel_GO, ON);
   }
   else if(digitalRead(Right_Sensor)){       //right sensor detects black      
        digitalWrite(Right_Wheel_GO, OFF);
   }
   else if(digitalRead(!Right_Sensor)){      //right sensor detects white 
        digitalWrite(Right_Wheel_GO,ON);
   }
   return;
} 

//Cone Deployment Code//
void cone_deployment(){
  int rval = 0; 
  r=deq(&HC_q,&rval);
  if (rval ==  Hole){
    hole_deploy();
  }
  else if (rval == Crack){
    crack_deploy(); 
  }
  return; 
}

void crack_deploy(){
   //Variables for internal dequeuing of each global variable queue 
   int crack_dist;
   deq(&HC_Dist_q, &crack_dist); 
   int crack_position;
   deq(&C_Pos_q,&crack_position);

   //Moving machine forward 
    while(1){
      digitalWrite(Left_Wheel_Dir,FORWARD);
      digitalWrite(Right_Wheel_Dir,FORWARD); 
      digitalWrite(Left_Wheel_GO,ON);
      digitalWrite(Right_Wheel_GO,ON);
      if(mySerial.available() > 0){
            total_distance = mySerial.read();
      }
      if (total_distance == (crack_dist+24)){
            digitalWrite(Left_Wheel_GO,OFF);
            digitalWrite(Right_Wheel_GO,OFF);
            break;
      }
    }

    //Deploying Cone 
    if (crack_position == LEFT){
         Move_Servo(current_position,1); 
         Move_Stepper();
         Move_Servo(1,2);
         Move_Stepper();
         current_position = 2; 
         number_of_cones_deployed++;
         cone_deployed = YES; 
    }else if(crack_position == RIGHT){
         Move_Servo(current_position, 6);
         Move_Stepper(); 
         Move_Servo(6,5);
         Move_Stepper(); 
         current_position = 5;  
         number_of_cones_deployed++;
         cone_deployed = YES;
    }else{               //All three sensors triggered
         Move_Servo(current_position, 3);
         Move_Stepper(); 
         Move_Servo(3,4); 
         Move_Stepper(); 
         current_position = 4; 
         number_of_cones_deployed++; 
         cone_deployed = YES; 
    }
    return; 
}

void hole_deploy(){
    int hole_dist;
    deq(&HC_Dist_q,&hole_dist);
    
   //Moving machine forward 
    while(1){
      digitalWrite(Left_Wheel_Dir,FORWARD);
      digitalWrite(Right_Wheel_Dir,FORWARD); 
      digitalWrite(Left_Wheel_GO,ON);
      digitalWrite(Right_Wheel_GO,ON);
      if(mySerial.available() > 0){
            total_distance = mySerial.read();
      }
      if (total_distance == (hole_dist+24)){
            digitalWrite(Left_Wheel_GO,OFF);
            digitalWrite(Right_Wheel_GO,OFF);
            break;
      }
    }
    
    Move_Servo(current_position, 0);
    Move_Stepper();
    current_position = 0;
    number_of_cones_deployed++; 
    cone_deployed = YES; 
    return; 
}

//Stepper Functions
void Move_Stepper(){
  for(int i=0;i<83;i++){    // 83 Steps = 45º turn 
            full_drive();
  }
  return; 
}

void full_drive (){
    digitalWrite(A3,LOW);
    digitalWrite(A2,LOW);
    digitalWrite(A1,HIGH);
    digitalWrite(A0,HIGH);
    delay(3);
    digitalWrite(A3,LOW);
    digitalWrite(A2,HIGH);
    digitalWrite(A1,HIGH);
    digitalWrite(A0,LOW);
    delay(3);
    digitalWrite(A3,HIGH);
    digitalWrite(A2,HIGH);
    digitalWrite(A1,LOW);
    digitalWrite(A0,LOW);
    delay(3);
    digitalWrite(A3,HIGH);
    digitalWrite(A2,LOW);
    digitalWrite(A1,LOW);
    digitalWrite(A0,HIGH);
    delay(3);
    digitalWrite(A3,LOW);
    digitalWrite(A2,LOW);
    digitalWrite(A1,HIGH);
    digitalWrite(A0,HIGH);
    delay(3);     
}

//Servo Functions
void Move_Servo(int current,int final_pos){
  int time_elaps;
  int servo_dir;
  if (current == final_pos){
    return; 
  }
  if (current == 0 && final_pos == 6){
    servo_dir = 0; 
    time_elaps = 472;
  }
  else if (current == 0 && final_pos == 1){
    servo_dir = 180; 
    time_elaps = 472;
  }
  else if (current == 0 && final_pos == 3){
    servo_dir = 180; 
    time_elaps = 295;
  }
  else if (current == 3 && final_pos == 4){
    servo_dir = 0; 
    time_elaps = 589;
  }
  else if (current == 1 && final_pos == 2){
    servo_dir = 0; 
    time_elaps = 589;
  }
  else if (current == 6 && final_pos == 5){
    servo_dir = 180; 
    time_elaps = 589;
  }
  else if (current == 5 && final_pos == 0){
    servo_dir = 0; 
    time_elaps = 147;
  }
  else if (current == 2 && final_pos == 0){
    servo_dir = 180; 
    time_elaps = 147;
  }
  else if (current == 4 && final_pos == 0){
    servo_dir = 180;
    time_elaps = 295;
  }
  else if (current == 4 && final_pos == 1){
    servo_dir = 180; 
    time_elaps = 766;
  }
  else if (current == 4 && final_pos == 6){
    servo_dir = 0;
    time_elaps = 177; 
  }
  else if (current == 2 && final_pos == 3){
    servo_dir = 180; 
    time_elaps = 177;
  }
  else if (current == 2 && final_pos == 6){
    servo_dir = 0; 
    time_elaps = 354;
  }
  else if (current == 5 && final_pos == 1){
    servo_dir = 180; 
    time_elaps = 354;
  }
  else if (current == 5 && final_pos == 3){
    servo_dir = 180; 
    time_elaps = 177;
  }
  servo.write(servo_dir);
  delay(time_elaps);
  servo.write(90); 
  return;
}

//Turning
void right_pivot_turn(){
  digitalWrite(Left_Wheel_GO,OFF);
  digitalWrite(Right_Wheel_GO,OFF);
  delay(10);
  digitalWrite(Left_Wheel_Dir,FORWARD);
  digitalWrite(Right_Wheel_Dir,BACKWARD); 
  digitalWrite(Left_Wheel_GO,ON);
  digitalWrite(Right_Wheel_GO,ON);
  delay(3000);    //time required for 90º turn 
  digitalWrite(Left_Wheel_GO,OFF);
  digitalWrite(Right_Wheel_GO,OFF);
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
