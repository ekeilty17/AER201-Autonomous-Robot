#ifndef AERSERVO_H
#define AERSERVO_H

//#include <SoftwareSerial.h>
#include "Servo2.h"

class AerServo : public Servo {
  private:
    const float _velocity_r = 8.0;
    const float _velocity_l = 8.0;

    int _pin;
    float _curr_pos;
  
  public:
    const int _t_left_10 = 1300;
    const int _t_right_10 = 1300;
    const int _t_left_3 = 430;
    const int _t_right_3 = 500;
    const int _t_left_m = 1000;
    const int _t_right_m = 1000;

    //Constructors
    AerServo();
    AerServo(int pin);
    AerServo(int pin, float curr_pos);
    void init();

    //Setter
    void setPin(int pin);
    void setCurr_pos(float curr_pos);
  
    //Basic Motion
    void move_right();
    void move_left();
    void stop();

    //Relative Displacement
    float displace_right(float dx);
    float displace_left(float dx);

    // Moving to specific positions
    float move_to(float x);
    float move_to_time(int t);
    float to_middle();
    float hard_left();
    float hard_right();
    float reset();
};

#endif  /* AERSERVO_H */
