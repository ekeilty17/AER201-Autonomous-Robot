#include "AerSteppers/AerSteppers.cpp"

AerSteppers steppers(2, 3, 4, 5);

void setup() {
  steppers.init();
}

void loop() {
  steppers.drop_cone();
  delay(1000);
  /*
  steppers.drop_cone();
  delay(2000);
  */
  
  while(1);
}

void rotation_test(AerSteppers steppers, int n) {
  for(int i = 0; i < n; i++) {
    steppers.drop_cone();
    delay(1000);
  }
}
