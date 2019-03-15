#include <SoftwareSerial.h>

int reL = 2;
int reR = 3;

float distance_travelled = 0.0;

void setup() {
//  pinMode(reL, INPUT);
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), Distance_ISR, CHANGE);
  Serial.begin(9600);
}

void loop() {
  Serial.print(distance_travelled); 
  Serial.print("\t");
  delay(2000);
}

void Distance_ISR() {
  //Check if right wheel rotary encoder interrupt enabled and flag raised 
  if(digitalRead(2)) {
      distance_travelled += 0.1;
  }
//  //Check if left wheel rotary encoder interrupt enabled and flag raised 
//  else if(digitalRead(reR)) {
//     distance_travelled += 1.06103295395;
//  }
}
