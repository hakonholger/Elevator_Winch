#ifndef PID_h
#define PID_h

#include <Arduino.h>
#include "MotorDriver.h"

class PID {
    MotorDriver &motor;
public:
    PID(double kp, double ki, double kd, MotorDriver &motorDriver);
    void compute(double setPoint);
private:
    double _kp, _ki, _kd; 
    
    double e_int; 
    double e_previous;
    double current_time;
    double previous_time;
    double nextIteration;
    double dt;
};

#endif