 /**
 * @file
 * @author Nkemjika Okeke & Eric Keilty
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
#include "AerServo/AerServo.cpp"
#include "AerServo/Servo2.cpp"
#include "AerSteppers/AerSteppers.cpp"
#include "AerDCMotors/AerDCMotors.cpp"
#include "Queue/Queue.cpp"

/* Definitions */
// Note: Left and Right is always from the robot's perspective

//Cone Deployment System Definitions 
#define SENSOR_LEFT         7
#define SENSOR_RIGHT        8
#define SENSOR_CENTRE       3     // Interrupt pin
#define SENSOR_CENTER       3     // Interrupt pin

#define HOLE               10
#define LEFT_CRACK         11
#define RIGHT_CRACK       110
#define CENTRE_CRACK      111
#define CENTER_CRACK      111

//Line Following Sensors Definitions
#define line_L             18    // A4
#define line_R             19    // A5
#define line_interr         2    // Interrupt pin

//Convert Rotary Encoder ticks to distance traveled
#define DISTANCE_CONVERSION 0.1

// Full Batteries
#define PWM_L     170
#define PWM_R     170
#define PWM_ADJ_L 140
#define PWM_ADJ_R 140
/*
// Half Dead Batteries
#define PWM_L     200
#define PWM_R     210
#define PWM_ADJ   130
*/

//Arduino-PIC Comm
const byte rxPin = 1;
const byte txPin = 0; 
volatile int stat = 0;
volatile char stat_char = 0;

//Global variables for primary initializing 
volatile int curr_pos = 0;
volatile int total_number_of_cones = 13;
volatile int number_of_cones_deployed = 0;

//Detection ISR Variables 
volatile bool isDeploying = false;
volatile int last_obstruction[2] = {0, 0};
volatile bool isPreviouslyDeployed = false;

//Data to send to PIC at end of operation
volatile int holes[20];
volatile int hole_num = 0;
volatile int cracks[20];
volatile int crack_num = 0;

//Cone Deployment Functions
void detection_ISR();
void cone_deployment();
bool shouldDeploy(int prev, int curr, float diff);
void hole_deploy();
void left_crack_deploy();
void right_crack_deploy();
void centre_crack_deploy();
void center_crack_deploy();

//Line Following Functions
void line_follow_ISR();
void line_follow();


/* Object Initializations */
//Arduino-PIC Comm
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin); 

//Initialize Queues
Queue HC_q(8);
Queue HC_Dist_q(8);

//Initialize DC Motors
// AerDCMotors(int pinL1, int pinL2, int pinR1, int pinR2)
AerDCMotors dc(5, 6, 9, 11);

//Initialize Servo
AerServo servo(10);

//Initialize Steppers, pins A0:3
AerSteppers stepp(14, 15, 16, 17); 

void setup() {

  // Initialize internal variables of classes
  dc.init();
  dc.stop();
  
  servo.init(); 
  servo.stop();
  servo.setCurr_pos(8.5);
  
  stepp.init();
  
  HC_q.init();


  //Arduino-PIC Comm
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT); 
  mySerial.begin(9600); 

  // Waiting for start instruction
  /* On keypad: 
   *    1) User presses 'A' key to being
   *    2) User types 2-digit number, i.e. '05' or '11' that represents number of cones loaded into the machine
   *    3) User presses 'A' to begin operation
   *  
   *  In PIC code:
   *    The PIC will then send the user-input to the Arduino
   *    Finally the PIC will send the number 100 to tell the arduino to begin operation
   */
  while (1) {
    
    // Polling PIC for input
    if (mySerial.available() > 0) {
      stat = mySerial.read();
    }

    // Loading user-input if within correct range
    if (4 <= stat and stat <= 12) {
      total_number_of_cones = stat;
    }

    // Checking if starting instruction was recieved
    if (stat == 100) {
      if (4 <= total_number_of_cones and total_number_of_cones <= 12) {
        // If user-input within correct range, break out of the waiting loop
        break;
      } else {
        // Otherwise, operation will not ensue
        while(1);
      }
    }
  }

  // Set Interrupt pins
  pinMode(SENSOR_CENTRE, INPUT);
  attachInterrupt(digitalPinToInterrupt(SENSOR_CENTRE), detection_ISR, CHANGE);

  pinMode(line_L,INPUT);
  pinMode(line_R,INPUT);
  pinMode(line_interr, INPUT);
  attachInterrupt(digitalPinToInterrupt(line_interr),line_follow_ISR,CHANGE);

  // Small delay so user has time to back away after pressing 'A' on keypad
  delay(1000);

  // Operations begin by robot moving forward
  dc.forward(PWM_L, PWM_R);
  
}

void loop() {

  // Core operations
  /* Arduino protocol:
   *    1) Poll PIC for rotary encoder data to update the total distance traveled
   *    2) Check if operation should terminate, which is based on 2 conditions
   *          - If the machine has no remaining cones to deploy
   *          - If the machine has traveled the full length of the lane (4m)
   *        To terminate operation, the arduino sends 'A' to the PIC
   *    3) Check if an obstruction has been detected and deploy accordingly
   *        This is done by using a Queue, storing the type of obstruction and location in which it was detected
   */
  while(1) {
    
    // Polling PIC for data from rotary encoders
    //curr_pos = get_rotary_encoder_data() * DISTANCE_CONVERSION;
    if (mySerial.available() > 0) {
       curr_pos =  mySerial.read() * DISTANCE_CONVERSION;
    }

    // Checking if operation should terminate
    if (number_of_cones_deployed >= total_number_of_cones or curr_pos >= 4000000000) {
      dc.stop();
      mySerial.write('A');
      break;
    }

    // Checking if there are obstructions that should be covered
    if (HC_q.isEmpty() == false){
      isDeploying = true;
      cone_deployment();
      isDeploying = false;

      /*
       * The isDeploying variable is to toggle line following. 
       *    Sometimes when the machine moves to position itself to deploy an obstruction 
       *    it happens to veer to one side slightly and the lane following sensor would become activated.
       *    If the isDeploying variable was taken out, this would trigger an interrupt 
       *    and the wheels would begin to move as a cone was being deployed.
       */
      
    }
  }

  // Returning to startline
  /*
   * After the completion of core operations, the machine must return to behind the startline.
   * This is done using the following sequence
   *    1) The obstructions sensors are detached to ensure no cones will be deployed on the return trip.
   *       Note: line following is kept on to that the machine does not veer into the lane on the return trip.
   *    2) The machine moves forward to ensure it has cleared any cones that were previously deployed
   *    3) The machine does a uturn, which consists of the following
   *          - 90 degree swing turn
   *          - forward movement
   *          - 90 degree swing turn
   *    4) Using rotary encoder data, the machine travels the distance of the lane (4m) until it is behind the start line
   *    5) All motors are turned off and all sensors detached. The machine then waits idle.
   */
  // Turn off obstruction sensors
  detachInterrupt(digitalPinToInterrupt(SENSOR_CENTRE));

  /*
  // Clear any previously deployed cones
  dc.forward(PWM_L, PWM_R);
  delay(3000);
  dc.stop();
  delay(1000);
  
  // 180 degree turn
  isDeploying = true;
  //detachInterrupt(digitalPinToInterrupt(line_interr));
  dc.uturn_right(255, 255, 255);
  
  // starting return trip
  dc.forward(PWM_L, PWM_R);
  */

  // Sending metadata
  char number_of_cones_deployed_char[3];
  dtostrf(number_of_cones_deployed, 2, 0, number_of_cones_deployed_char);
  mySerial.write('^');
  for (int i=0; i<3; i++) {
    if (number_of_cones_deployed_char[i] == ' ') {
      mySerial.write('0');
      wait();
      continue;
    }
    mySerial.write(number_of_cones_deployed_char[i]);
    wait();
  }

  char hole_num_char[3];
  dtostrf(hole_num, 2, 0, hole_num_char);
  mySerial.write('*');
  for (int i=0; i<3; i++) {
    if (hole_num_char[i] == ' ') {
      mySerial.write('0');
      wait();
      continue;
    }
    mySerial.write(hole_num_char[i]);
    wait();
  }

  char crack_num_char[3];
  dtostrf(crack_num, 2, 0, crack_num_char);
  mySerial.write('_');
  for (int i=0; i<3; i++) {
    if (crack_num_char[i] == ' ') {
      mySerial.write('0');
      wait();
      continue;
    }
    mySerial.write(crack_num_char[i]);
    wait();
  }

  // Sending crack data
  mySerial.write('c');
  for (int i=0; i<crack_num; i++) {
    char buf[6];
    dtostrf(cracks[i], crack_num, 1, buf);
    for (int j=0; j<6; j++) {
      if (buf[j] == ' ') {
        mySerial.write('0');
        wait();
        continue;
      }
      mySerial.write(buf[j]);
      wait();
    }
  }
  
  // Sending hole data
  mySerial.write('h');
  for (int i=0; i<hole_num; i++) {
    char buf[6];
    dtostrf(holes[i], hole_num, 1, buf);
    for (int j=0; j<6; j++) {
      if (buf[j] == ' ') {
        mySerial.write('0');
        wait();
        continue;
      }
      mySerial.write(buf[j]);
      wait();
    }
  }
  
  // Turning off drive system motors and line following sensors
  delay(3000);
  dc.stop();
  
  // Waiting idle
  while(1);
  
} /* end of main loop */

/* Rotary Encoders
 * 
 */
int get_rotary_encoder_data() {
  
  int RE_right = -1;
  mySerial.write('R');
  while(RE_right == -1) {
    if (mySerial.available() > 0) {
      RE_right = mySerial.read();
    }
  }
  
  int RE_left = -1;
  mySerial.write('L');
  while(RE_left == -1) {
    if (mySerial.available() > 0) {
      RE_left = mySerial.read();
    }
  }
  
  return (RE_left << 8) | RE_right;
}
void wait() {
  while(stat_char != 'D') {
    if (mySerial.available() > 0) {
      stat_char = mySerial.read();
    }
  }
  stat_char = '\0';
}

/* Cone Deployment
 * 
 * if (obstruction detected):
 *    add obstruciton type and position to Queue
 *    record position in separate array to send to PIC later
 * 
 */
unsigned long lastInterrupt = 0;
void detection_ISR(){
  
  // To prevent repeated input due to bad sensor signal
  if (millis() - lastInterrupt > 200) {
    
    if (digitalRead(SENSOR_CENTRE) == LOW) {
      return;
    }

    dc_stop();
    //curr_pos = get_rotary_encoder_data() * DISTANCE_CONVERSION;

    int obstruction = 0;
    if (digitalRead(SENSOR_RIGHT) == HIGH and digitalRead(SENSOR_LEFT) == HIGH) {
      // Center Crack
      obstruction = 111;
      cracks[crack_num] = curr_pos;
      crack_num += 1;
      mySerial.write('C'); 
    } else if (digitalRead(SENSOR_RIGHT) == HIGH) {
       // Right Crack
       obstruction = 110;
       cracks[crack_num] = curr_pos;
       crack_num += 1;
       mySerial.write('C'); 
    } else if (digitalRead(SENSOR_LEFT) == HIGH) {
      // Left Crack
      obstruction = 11;
      cracks[crack_num] = curr_pos;
      crack_num += 1;
      mySerial.write('C'); 
    } else if (digitalRead(SENSOR_RIGHT) == LOW and digitalRead(SENSOR_LEFT) == LOW) {
      // Hole
      obstruction = 10;
      holes[hole_num] = curr_pos;
      hole_num += 1;
      mySerial.write('H');
    } else {
      // idk
      return;
    }
    
    HC_q.enq(obstruction);
    HC_Dist_q.enq(curr_pos);
  }
  lastInterrupt = millis();
  
  return;
}
void dc_stop() {
  dc.stop();
}

/*
 * 1) move forward to position chute on obstruction
 * 2) based on previous deployment and rotary encoder data determine if the obstruction should be covered
 * 3) continue forward
 */

void cone_deployment(){

  int obstruction = HC_q.deq();
  int pos = HC_Dist_q.deq();
  bool shouldCurrentlyDeploy = shouldDeploy(last_obstruction[0], obstruction, pos-last_obstruction[1]);

  if (!isPreviouslyDeployed or (isPreviouslyDeployed and shouldCurrentlyDeploy)) {

    // Once an obstruction is detected, the machine needs to move forward a certain distance in order to deploy
    /*
    isDeploying = false;
    delay(1000);
    dc.forward(PWM_L, PWM_R);
    delay(1000);
    dc.stop();
    isDeploying = true;
    */
    
    switch(obstruction) {
      case HOLE:
        hole_deploy();
        number_of_cones_deployed += 1;
        hole_num += 1;
        break;
      case LEFT_CRACK:
        left_crack_deploy();
        number_of_cones_deployed += 2;
        crack_num += 1;
        break;
      case RIGHT_CRACK:
        right_crack_deploy();
        number_of_cones_deployed += 2;
        crack_num += 1;
        break;
      case CENTRE_CRACK:
        centre_crack_deploy();
        number_of_cones_deployed += 2;
        crack_num += 1;
        break;
      default:
        break;
    }
    
    // putting servo in neutral position
    //servo.to_middle();
    
  }
  
  last_obstruction[0] = obstruction;
  last_obstruction[1] = pos;
  isPreviouslyDeployed = (!isPreviouslyDeployed or (isPreviouslyDeployed and shouldCurrentlyDeploy));

  // Begin moving after done deployment
  dc.forward(PWM_L, PWM_R);
  
  return;
}

bool shouldDeploy(int prev, int curr, float diff) {
  
  if (diff > 20.0) {
    // hole, then crack, >20cm  or
    // crack, then hole, >20cm
    return true;
  }
  
  if (prev == HOLE) {
    if (curr == HOLE) {
      if (diff < 15.0) {
        // hole, then hole, <15cm
        return false;
      }
      // hole, then hole, >15cm
      return true;
    }
    // hole, then crack <20cm
    return false;
  } else { /*prev == CRACK*/
    if (curr != HOLE) { /*curr == CRACK*/
      if (diff < 10) {
        // crack, then crack <10cm
        return false;
      }
      // crack, then crack, >10cm
      return true;
    }
    // crack, then hole, <20cm
    return false;
  }
}

void hole_deploy() {
  servo.to_middle();
  stepp.drop_cone();
}
void left_crack_deploy() {
  // Cone 1
  servo.move_to_time(-2500);
  servo.move_to_time(300);
  stepp.drop_cone();
  // Cone 2
  servo.move_to_time(1500);
  stepp.drop_cone();
  servo.setCurr_pos(10);
}
void right_crack_deploy() {
  // Cone 1
  servo.move_to_time(2000);
  servo.move_to_time(-400);
  stepp.drop_cone();
  // Cone 2
  servo.move_to_time(-2100);
  stepp.drop_cone();
  servo.setCurr_pos(7);
}
void centre_crack_deploy() {
  // Cone 1
  servo.move_to_time(-2500);
  servo.move_to_time(600);
  stepp.drop_cone();
  // Cone 2
  servo.move_to_time(3500);
  servo.move_to_time(-500);
  stepp.drop_cone();
  servo.setCurr_pos(13);
}
void center_crack_deploy() {
  centre_crack_deploy();
}

/*  Line Following
 * 
 *    if (detected a lane):
 *      turn off corresponding motor
 *      set other motor to a lower speed (called PWM_ADJ)
 *    
 *    if (neither lanes detected):
 *        put both motors back to regular driving speed
 */
void line_follow_ISR(void) {
  line_follow();
  return;
}
void line_follow() {
  if (isDeploying) {
    return;
  }
  if (digitalRead(line_R) == LOW and digitalRead(line_L) == LOW) {
    // Sensing now lines, go straight  
    dc.left_wheel_forward(PWM_L);
    dc.right_wheel_forward(PWM_R);
  } else if (digitalRead(line_R) == HIGH and digitalRead(line_L) == LOW) {
    // Sensor over right lane
    dc.left_wheel_forward(PWM_ADJ_L);
    dc.right_wheel_stop();
  } else if (digitalRead(line_R) == LOW and digitalRead(line_L) == HIGH){
    // Sensor over left lane
    dc.left_wheel_stop();
    dc.right_wheel_forward(PWM_ADJ_R);
  } else {
    // Sensing both lanes
    dc.stop();
  }
}
