#ifndef AERDCMOTORS_H
#define AERDCMOTORS_H

class AerDCMotors{
  private:
    int _pinL1;
    int _pinL2;
    int _pinR1;
    int _pinR2;
  
  public:
    //Constructors
    AerDCMotors();
    AerDCMotors(int pinR2, int pinR1, int pinL2, int pinL1);
    void init();
  
    //Moving Wheels
    void left_wheel_forward();
    void right_wheel_forward();
    void left_wheel_backward();
    void right_wheel_backward();
    void left_wheel_stop();
    void right_wheel_stop();

    //Moving Robot (simple)
    void forward();
    void backward();
    void stop();

    //Moving Robot (complex)
    void pivot_right();
    void pivot_left();
    void swing_right();
    void swing_left();
    void uturn_right(int turn_delay, int forward_delay, bool pivot);
};

#endif  /* AERDCMOTORS_H */
