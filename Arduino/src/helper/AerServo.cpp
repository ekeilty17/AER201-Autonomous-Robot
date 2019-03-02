#include "Arduino.h"    // needed in order to call delay()
#include <Servo.h>

class AerServo : public Servo {
  private:
    const float _velocity_r = 8.01;
    const float _velocity_l = 8.01;
    
    float _curr_pos;
  
  public:
    int _pin;
    const int _t_left = 1300;
    const int _t_right = 1300;
    const int _t_cleft = (int) _t_left * (3 / 10);
    const int _t_cright = (int) _t_right * (3 / 10);
    const int _t_mleft = 1200;
    const int _t_mright = 1200;
    
    //Constructors
    AerServo() {
    }
    AerServo(int pin){
      _pin = pin;
      _curr_pos = 0.0;
    }
    AerServo(int pin, float curr_pos){
      _pin = pin;
      _curr_pos = curr_pos;
    }
    void init() {
      this -> attach(_pin);
    }

    //Setter
    void setCurr_pos(float curr_pos) {
      _curr_pos = curr_pos;
    }
  
    //Basic Motion
    void move_right() {
      this -> write(0);
    }
    void move_left() {
      this -> write(180);
    }
    void stop() {
      this -> write(90);
    }

    //Relative Displacement
    void displace_right(float dx) {
      this -> move_right();
      int t = (int) (dx * 1000) / _velocity_r;
      delay(t);
      this -> stop();
    }
    void displace_left(float dx) {
      this -> move_left();
      int t = (int) (dx * 1000) / _velocity_l;
      delay(t);
      this -> stop();
    }

    // Moving to specific positions
    float move_to(float x) {
      if (x > _curr_pos) {
        this -> displace_right(x - _curr_pos);
      } else {
        this -> displace_left(_curr_pos - x);
      }
      _curr_pos = x;
      delay(500);
      return _curr_pos;
    }
    float middle() {
      this -> move_to(8.5);
      return 8.5;
    }
    float hard_left() {
      this -> move_to(0 - 2);   //over shoot to ensure it's flush
      _curr_pos = 0;
      return 0;
    }
    float hard_right() {
      this -> move_to(17 + 2);  //over shoot to ensure it's flush
      _curr_pos = 17;
      return 17;
    }
    void reset(){
      if (_curr_pos < 8.5) {
        this -> hard_left();
      } else {
        this -> hard_right();
      }
      this -> middle();
    }
};
