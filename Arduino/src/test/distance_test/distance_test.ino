
#include <SoftwareSerial.h>

/****Distance Global Variables****/
float distance_travelled = 0; 

void setup() {
  //Interrupts
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), Distance, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), Distance, CHANGE);
  Serial.begin(9600);  
}

void loop() {
  Serial.print(distance_travelled); 
  Serial.print("\t");  
  delay(2000);
}


void Distance(){
    //Check if right wheel rotary encoder interrupt enabled and flag raised 
    if(digitalRead(2)){
        distance_travelled = distance_travelled + 1.06103295395;  
    }
    //Check if left wheel rotary encoder interrupt enabled and flag raised 
    else if(digitalRead(3)){
       distance_travelled = distance_travelled + 1.06103295395; 
    }
}
