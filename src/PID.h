#ifndef PID_h
#define PID_h

#include <Arduino.h>

class PID {
public:
    PID(double kp, double ki, double kd, double setPoint);
    double compute(int actualPosition);
private:
    double _kp, _ki, _kd;
    double _setPoint; 
    
    double e_int; 
    double e_previous;
    double current_time;
    double previous_time;
};

#endif