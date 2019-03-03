#include "AerRotaryEncoders/AerRotaryEncoders.cpp"

AerRotaryEncoders encoders(2, 3);

void setup() {
  encoders.init();
}

void loop() {
  Serial.print(encoders.distance_travelled); 
  Serial.print("\t");
  delay(2000);
}
