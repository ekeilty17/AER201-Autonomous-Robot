#ifndef AERSERVO_H
#define AERSERVO_H

#include <Servo.h>

class AerServo : public Servo {
  private:
    const float _velocity_r = 8.01;
    const float _velocity_l = 8.01;

    int _pin;
    float _curr_pos;
  
  public:
    const int _t_left_10 = 1300;
    const int _t_right_10 = 1300;
    int _t_left_3 = 430;
    int _t_right_3 = 500;
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
    void move_right(int t);
    void move_left(int t);
    void stop();

    //Relative Displacement
    void displace_right(float dx);
    void displace_left(float dx);

    // Moving to specific positions
    float move_to(float x);
    float middle();
    float hard_left();
    float hard_right();
    void reset();
};

#endif  /* AERSERVO_H */
