#include "PID.h"
#include "MotorDriver.h"

PID::PID(double kp, double ki, double kd,
             MotorDriver &motorDriver) 
    : 
    motor(motorDriver),
    _kp(kp), _ki(ki), _kd(kd),
    e_int(0), e_previous(0), previous_time(0), nextIteration(10), dt(0) {}


void PID::compute(double setPoint) {

    nextIteration = 20;
    
    double actualPosition = motor.getPos();
    double current_time = millis();
    dt = (current_time - previous_time); 

    // Venter til neste intervall:
    while ((dt < nextIteration)) {
      current_time = millis();                             
      dt = (float)(current_time - previous_time);
    }
    
    previous_time = current_time;
    
    double e = setPoint*2000*5 - actualPosition;
    double e_der = (e - e_previous)/dt;
    e_int += e * dt;

    // kvitter oss med "Wind-Up"
    if(abs(e) >= 5) {
      e_int = 0;
    }

    double u = _kp*e + _ki*e_int + _kd*e_der ;

    e_previous = e;

    // Setter maks PWM til 50, så motor ikke går så fort...
      if (u < -255) {
    u = -255;
    } else if (u > 255) {
    u = 255;
    }

    motor.driveMotor(u);
  // delay(20);
    Serial.print(actualPosition); Serial.print("  Set: "); Serial.println(setPoint*2000*5);
   //Serial.println(dt);
}