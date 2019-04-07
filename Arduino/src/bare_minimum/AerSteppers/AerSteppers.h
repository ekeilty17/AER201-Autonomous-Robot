#ifndef AERSTEPPERS_H
#define AERSTEPPERS_H

class AerSteppers {
  //Internal variables
  private:
    int _pin1;
    int _pin2;
    int _pin3;
    int _pin4;

    int _time_delay = 0;

  public:
    //Constructors
    AerSteppers();
    AerSteppers(int pin1, int pin2, int pin3, int pin4);
    void init();
    
    //Setters
    void setTime_delay(int time_delay);
  
    //Methods
    void full_drive();
    void drop_cone();
    
};

#endif  /* AERSTEPPERS_H */
