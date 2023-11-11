#include "PID.h"
#include "MotorDriver.h"

PID::PID(double kp, double ki, double kd,
             MotorDriver &motorDriver) 
    : 
    motor(motorDriver),
    _kp(kp), _ki(ki), _kd(kd),
    e_int(0), e_previous(0), previous_time(0), dt(10), elapsed_time(0) {}


void PID::compute(double setPoint) {

    dt = 10;
    
    double actualPosition = motor.getPos();
    double current_time = millis();

    elapsed_time = (current_time - previous_time); 

    // Venter til neste intervall:
    while (!(elapsed_time >= dt)) {
      current_time = millis();                             
      elapsed_time = (float)(current_time - previous_time);
    }
 
    double e = setPoint*1000 - actualPosition;
    double e_der = (e - e_previous)/dt;
    e_int += e * dt; 

    // kvitter oss med "Wind-Up"
    if(abs(e) >= 5) {
      e_int = 0;
    }

    double u = _kp*e + _ki*e_int + _kd*e_der ;

    e_previous = e;
    previous_time = current_time;

    // Setter maks PWM til 50, så motor ikke går så fort...
      if (u < -50) {
    u = -50;
    } else if (u > 50) {
    u = 50;
    }

    motor.driveMotor(u);
    delay(20);
    //  Serial.print(actualPosition); Serial.print("  Set: "); Serial.println(setPoint*1000);
}