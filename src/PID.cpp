#include "PID.h"
#include "MotorDriver.h"

PID::PID(double kp, double ki, double kd,
             MotorDriver &motorDriver) 
    : 
    motor(motorDriver),
    _kp(kp), _ki(ki), _kd(kd),
    e_int(0), e_previous(0), previous_time(0),dt(10), elapsed_time(0) {}


void PID::compute(double setPoint) {

    int actualPosition = motor.getPos();
    double current_time = millis();


    double dt = (current_time - previous_time);
    elapsed_time = (current_time - previous_time); 

    //Waiting foor loop time:
    while (!(elapsed_time >= dt)) {
      current_time = millis();                             
      elapsed_time = (float)(current_time - previous_time);
    }
 
    double e = setPoint - actualPosition;
    double e_der = (e - e_previous)/dt;
    e_int += e * dt; 

    double u = _kp*e + _ki*e_int + _kd*e_der ;

    if(abs(e) >= 5) {
      e_int = 0;
      e_der = 0;
    }

    e_previous = e;
    previous_time = current_time;

    // Constrains
      if (u < -255) {
    u = -255;
    } else if (u > 255) {
    u = 255;
    }

    motor.driveMotor(u);

  Serial.print("Posisjon: "); Serial.print(actualPosition);
  Serial.print(" Setpunkt: "); Serial.print(setPoint);
  Serial.print(" Pådrag: "); Serial.println(u);
}