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
    void left_wheel_forward(int pwm_val);
    void right_wheel_forward(int pwm_val);
    void left_wheel_backward(int pwm_val);
    void right_wheel_backward(int pwm_val);
    void left_wheel_stop();
    void right_wheel_stop();

    //Moving Robot (simple)
    void forward(int pwm_val_L, int pwm_val_R, bool jump);
    void backward(int pwm_val);
    void stop(int pwm_val_L, int pwm_val_R, int gradual);
    void gradual_stop(int pwm_val_L, int pwm_val_R);

    //Moving Robot (complex)
    void pivot_right(int pwm_val);
    void pivot_left(int pwm_val);
    void swing_right(int pwm_val);
    void swing_left(int pwm_val);
    void uturn_right(int pwm_val_turn, int pwm_val_L, int pwm_val_R, int turn_delay_1, int turn_delay_2);
    void uturn_left(int pwm_val_turn, int pwm_val_L, int pwm_val_R, int turn_delay_1, int turn_delay_2);
};

#endif  /* AERDCMOTORS_H */
