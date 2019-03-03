#ifndef AERROTARYENCODERS_H
#define AERROTARYENCODERS_H

class AerRotaryEncoders{
  private:
    int _pinL;
    int _pinR;
  
  public:
    float distance_travelled = 0.0;
  
    //Constructors
    AerRotaryEncoders();
    AerRotaryEncoders(int pinL, int pinR);
    void init();
  
    //Methods
    void Distance();
};

#endif  /* AERROTARYENCODERS_H */
