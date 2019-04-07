#ifndef AERSERVO_H
#define AERSERVO_H

//#include <SoftwareSerial.h>
#include "Servo2.h"

class AerServo : public Servo {
  private:
    int _pin;
    float _curr_pos;
    const float _width = 16.5;  // cm
    float _v = 1.0;             // cm/s
  
  public:

    //Constructors
    AerServo();
    AerServo(int pin);
    AerServo(int pin, float curr_pos);
    void init();

    //Setter
    void setPin(int pin);
    void setCurr_pos(float curr_pos);
    void setV(float v);
    void setVelocity(float t);
  
    //Basic Motion
    void stop();
    void move_right(int t);
    void move_left(int t);
    void move(int t);

    //Relative Displacement
    float displace_right(float dx);
    float displace_left(float dx);
    float displace(float dx);

    // Moving to specific positions
    float move_to(float x);
    float to_middle();
    float hard_left();
    float hard_right();
    float reset();
};

#endif  /* AERSERVO_H */
