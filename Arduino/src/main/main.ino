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
#include "config.h"
#include "AerServo/AerServo.cpp"
#include "AerServo/Servo2.cpp"
#include "AerSteppers/AerSteppers.cpp"
#include "AerDCMotors/AerDCMotors.cpp"
#include "Queue/Queue.cpp"

/* Definitions */
// Note: Left and Right is always from the robot's perspective

//Cone Deployment System Definitions 
#define SENSOR_LEFT         8
#define SENSOR_RIGHT        7
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

//Arduino-PIC Comm
const byte rxPin = 1;
const byte txPin = 0; 
volatile int stat = 0;
volatile char stat_char = 0;

volatile bool isDeploying = false;
volatile bool detectionSensorsOn = true;
volatile bool isTimer = false;
volatile bool returnTrip = false;

//Cone Deployment Functions
void detection_ISR();
void cone_deployment();
void hole_deploy();
void left_crack_deploy();
void right_crack_deploy();
void centre_crack_deploy();
void center_crack_deploy();
void safe_crack_deploy();

//Line Following Functions
void line_follow_ISR();
void line_follow();


/* Object Initializations */
//Arduino-PIC Comm
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin); 

//Initialize Queue
Queue HC_q(20);

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
   *    The PIC will send the number 100 to tell the arduino to begin operation
   */
  while (1) {
    
    // Waiting for PIC to initiate starting instruction
    if (mySerial.available() > 0) {
      stat =  mySerial.read();
    }
    if (stat == 100) {
      break;
    }
  }

  // Set Interrupt pins
  pinMode(SENSOR_CENTRE, INPUT);
  attachInterrupt(digitalPinToInterrupt(SENSOR_CENTRE), detection_ISR, CHANGE);

  pinMode(line_L,INPUT);
  pinMode(line_R,INPUT);
  pinMode(line_interr, INPUT);
  attachInterrupt(digitalPinToInterrupt(line_interr),line_follow_ISR,CHANGE);

  // Delay between operator pressing start button and machine beginning operation
  delay(500);

  // Operations begin by robot moving forward
  dc.forward();
  
}

void loop() {

  // Core operations
  /* Arduino protocol:
   *    1) Check if operation should terminate, which is based on 2 conditions
   *          - If the machine has no remaining cones to deploy
   *          - If the machine has traveled the full length of the lane (4m)
   *        The arduino will be told to stop by the PIC
   *    2) Check if an obstruction has been detected and deploy accordingly
   *        This is done by using a Queue, storing the type of obstruction and location in which it was detected
   */
  while(1) {
    
    // Polling PIC to see if the machine has reached end of operation should end
    if (mySerial.available() > 0) {
      stat_char =  mySerial.read();
      if (stat_char == 'E') {
        break;
      }
    }

    // Total time of operation must be less than 
    if (millis() > (180000 - (CONE_CLEAR + PIVOT + FORWARD_PIVOT + RETURN + 2000))) {
      isTimer = true;
      break;
    }
    
    // In case the machine stops on a line, which means the interrupt would not trigger
    line_follow();
    
    // Checking if there are obstructions that should be covered
    if (HC_q.isEmpty() == false){
      cone_deployment();

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
  
  // Clear any previously deployed cones
  dc.forward();
  delay(CONE_CLEAR);
  dc.stop();
  servo.to_middle();

  // Turn off line following
  isDeploying = true;
  
  // 180 degree turn
  dc.uturn_right(PIVOT, FORWARD_PIVOT, true);

  // reverse line following
  returnTrip = true;
  
  // starting return trip
  dc.forward();
  delay(RETURN);
  
  // Turning off drive system motors and line following sensors
  dc.stop();

  // detect line following pins
  detachInterrupt(digitalPinToInterrupt(line_interr));

  // Tell PIC operations are complete
  if (isTimer) {
    mySerial.write('E');
  } else {
    mySerial.write('A');
  }
  
  // Waiting idle
  while(1);
  
} /* end of main loop */



/* Cone Deployment
 * 
 * if (obstruction detected):
 *    add obstruciton type and position to Queue
 *    record position in separate array to send to PIC later
 * 
 */
unsigned long lastInterrupt = 0;
void detection_ISR(){

  if (detectionSensorsOn == false) {
    return;
  }
  
  // To prevent repeated input due to bad sensor signal
  if (millis() - lastInterrupt > DEBOUNCE_OBSTRUCTION) {
    
    //    ?        *         ?
    if (digitalRead(SENSOR_CENTRE) == LOW) {
      return;
    }
    
    int obstruction = 0;
    //    *        *         *
    if (digitalRead(SENSOR_RIGHT) == HIGH and digitalRead(SENSOR_LEFT) == HIGH) {
      // Center Crack
      obstruction = 111;
      mySerial.write('C'); 
    } 
    //   __        *         *
    else if (digitalRead(SENSOR_RIGHT) == HIGH) {
       // Right Crack
       obstruction = 110;
       mySerial.write('C'); 
    } 
    //    *        *         __
    else if (digitalRead(SENSOR_LEFT) == HIGH) {
      // Left Crack
      obstruction = 11;
      mySerial.write('C'); 
    } 
    //   __        *         __
    else if (digitalRead(SENSOR_RIGHT) == LOW and digitalRead(SENSOR_LEFT) == LOW) {
      // Hole
      obstruction = 10;
      mySerial.write('H');
    } else {
      // idk
      return;
    }

    dc_stop();    // for some reason calling dc.stop() in the IRS doesn't work
    HC_q.enq(obstruction);
  }
  lastInterrupt = millis();

  dc_stop();
  delay(100);
  line_follow();
  
  return;
}
void dc_stop() {
  dc.stop();
}



/*
 * 1) move forward to position chute on obstruction
 * 2) based on previous deployment and rotary encoder data determine if the obstruction should be covered
 *      - This will be sent by the PIC in the form of a 'T' for deploy and a 'F' for skip
 * 3) continue forward
 */

void cone_deployment(){
  
  detectionSensorsOn = false;
  int obstruction = HC_q.deq();

  dc.stop();
  int shouldDeploy = -1;
  mySerial.write('R');
  while(shouldDeploy == -1) {
    if (mySerial.available() > 0) {
      shouldDeploy = mySerial.read();
    }
  }
  
  if (shouldDeploy != 0) {
    // Once an obstruction is detected, the machine needs to move forward a certain distance in order to deploy
    dc.forward();
    line_follow();
    //int adjusted_delay = (int) FORWARD * (1 - shouldDeploy/20.0);
    //delay(adjusted_delay);
    delay(FORWARD);
    dc.stop();
    
    detectionSensorsOn = false;
    isDeploying = true;         // To prevent line following during deployment
    /*
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
    */
    if (obstruction == HOLE) {
      hole_deploy();
    } else {
      safe_crack_deploy();
    }
    
    detectionSensorsOn = true;
    isDeploying = false;
    
    // putting servo in neutral position
  }

  // Begin moving after done deployment
  line_follow();
  
  return;
}

void hole_deploy() {
  servo.to_middle();
  stepp.drop_cone();
}
void left_crack_deploy() {
  // Cone 1
  servo.move_to_time(-MIDDLE_TO_EDGE);
  servo.move_to_time(EDGE_TO_3+600);
  stepp.drop_cone();
  // Cone 2
  servo.move_to_time(_3_TO_10);
  stepp.drop_cone();
  servo.setCurr_pos(15);
}
void right_crack_deploy() {
  // Cone 1
  servo.move_to_time(MIDDLE_TO_EDGE);
  servo.move_to_time(-EDGE_TO_3);
  stepp.drop_cone();
  // Cone 2
  servo.move_to_time(-_3_TO_10);
  stepp.drop_cone();
  servo.setCurr_pos(7);
}
void centre_crack_deploy() {
  // Cone 1
  servo.move_to_time(-MIDDLE_TO_EDGE);
  servo.move_to_time(EDGE_TO_6);
  stepp.drop_cone();
  // Cone 2
  servo.move_to_time(EDGE_TO_EDGE);
  servo.move_to_time(-EDGE_TO_6);
  stepp.drop_cone();
  servo.setCurr_pos(13);
}
void center_crack_deploy() {
  centre_crack_deploy();
}
void safe_crack_deploy() {
  // Cone 1
  servo.move_to_time(-MIDDLE_TO_EDGE+250);
  servo.move_to_time(EDGE_TO_3);
  stepp.drop_cone();
  // Cone 2
  servo.move_to_time(EDGE_TO_EDGE+150);
  servo.move_to_time(-EDGE_TO_3);
  stepp.drop_cone();
  servo.setCurr_pos(15);
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
  if (returnTrip) {
    reverse_line_follow();
  } else {
    line_follow();
  }
  return;
}
void line_follow() {
  if (isDeploying) {
    return;
  }
  if (digitalRead(line_R) == LOW and digitalRead(line_L) == LOW) {
    // Sensing now lines, go straight
    dc.left_wheel_forward();
    dc.right_wheel_forward();
  } else if (digitalRead(line_R) == HIGH and digitalRead(line_L) == LOW) {
    // Sensor over right lane
    dc.right_wheel_stop();
    dc.left_wheel_forward();
  } else if (digitalRead(line_R) == LOW and digitalRead(line_L) == HIGH){
    // Sensor over left lane
    dc.left_wheel_stop();
    dc.right_wheel_forward();
  } else {
    // Sensing both lanes
    dc.left_wheel_forward();
    dc.right_wheel_forward();
  }
}
void reverse_line_follow() {
  if (isDeploying) {
    return;
  }
  if (digitalRead(line_R) == LOW and digitalRead(line_L) == LOW) {
    // Sensing now lines, go straight
    dc.left_wheel_forward();
    dc.right_wheel_forward();
  } else if (digitalRead(line_R) == HIGH and digitalRead(line_L) == LOW) {
    // Sensor over left lane
    dc.left_wheel_stop();
    dc.right_wheel_forward();
  } else if (digitalRead(line_R) == LOW and digitalRead(line_L) == HIGH){
    // Sensor over right lane
    dc.right_wheel_stop();
    dc.left_wheel_forward();
  } else {
    // Oops we in the lane
    dc.left_wheel_forward();
    dc.right_wheel_forward();
  }
}
