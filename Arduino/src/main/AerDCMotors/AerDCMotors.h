#ifndef AERDCMOTORS_H
#define AERDCMOTORS_H

class AerDCMotors{
  private:
    int _pinL1;
    int _pinL2;
    int _pinR1;
    int _pinR2;
    int _pwmL;
    int _pwmR;
  
  public:
    //Constructors
    AerDCMotors();
    AerDCMotors(int pinL1, int pinL2, int pinR1, int pinR2);
    AerDCMotors(int pinL1, int pinL2, int pinR1, int pinR2, int pwmL, int pwmR);
    void init();
  
    //Moving Wheels
    void left_wheel_forward(int pwm_val);
    void right_wheel_forward(int pwm_val);
    void left_wheel_backward(int pwm_val);
    void right_wheel_backward(int pwm_val);
    void left_wheel_stop();
    void right_wheel_stop();

    //Moving Robot (simple)
    void forward(int pwm_val);
    void backward(int pwm_val);
    void stop();

    //Moving Robot (complex)
    void pivot_right(int pwm_val);
    void pivot_left(int pwm_val);
    void u_turn_right();
    void u_turn_left();
};

#endif  /* AERDCMOTORS_H */
