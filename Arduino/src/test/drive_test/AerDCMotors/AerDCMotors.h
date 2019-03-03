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
    AerDCMotors(int pinL1, int pinL2, int pinR1, int pinR2);
    void init();
  
    //Moving Wheels
    void left_wheel_forward();
    void right_wheel_forward();
    void left_wheel_backward();
    void right_wheel_backward();
    void left_wheel_slow();
    void right_wheel_slow();
    void left_wheel_stop();
    void right_wheel_stop();

    //Moving Robot (simple)
    void forward();
    void backward();
    void slow();
    void stop();

    //Moving Robot (complex)
    void pivot_right();
    void pivot_left();
    void u_turn_right();
    void u_turn_left();
};

#endif  /* AERDCMOTORS_H */
