#include "PID.h"
#include "MotorDriver.h"

PID::PID(double kp, double ki, double kd,
             MotorDriver &motorDriver) 
    : 
    motor(motorDriver),
    _kp(kp), _ki(ki), _kd(kd),
    e_int(0), e_previous(0), previous_time(0) {}


void PID::compute(double setPoint) {

    int actualPosition = motor.getPos();
    double current_time = micros();

    // Comuputing dt /1e6 micros to seconds
    double dt = (current_time - previous_time) / 1.0e6;
    previous_time = current_time;
 
    double e = setPoint - actualPosition;
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

    motor.driveMotor(u);

  Serial.print("Posisjon: "); Serial.print(actualPosition);
  Serial.print(" Setpunkt: "); Serial.print(setPoint);
  Serial.print(" Pådrag: "); Serial.println(u);
}