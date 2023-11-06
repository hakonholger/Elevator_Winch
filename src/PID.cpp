#include "PID.h"



PID::PID(double kp, double ki, double kd, double setPoint) {

    _kp = kp; 
    _ki = ki; 
    _kd = kd;
    _setPoint = setPoint;
    e_int = 0; 
    e_previous = 0;
    previous_time = 0;
    e_int = 0;
}


double PID::compute(int actualPosition) {

    double current_time = micros();
    // Comuputing dt
    double dt = (current_time - previous_time) / 1.0e6;
    previous_time = current_time;
 
    double e = _setPoint - actualPosition;
    double e_der = (e - e_previous)/dt;
    e_int += e * dt; 

    double u = _kp*e + _ki*e_int + _kd*e_der ;
    e_previous = e;

    // Constrains
      if (u < -255) {
    u = -255;
    } else if (u > 255) {
    u = 255;
    }

    return u;
}