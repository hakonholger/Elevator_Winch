#include "PIDController.h"

PIDController::PIDController(float kp, float ki, float kd, int target) {
  _kp = kp;
  _ki = ki;
  _kd = kd;
  _target = target;
  _eprev = 0;
  _prevT = 0;
  _eintegral = 0;
}

float PIDController::compute(int currentPos) {
  long currT = micros();
  float deltaT = ((float)(currT - _prevT)) / 1.0e6;
  _prevT = currT;
  int e = currentPos - _target;
  float dedt = (e - _eprev) / (deltaT);
  _eintegral += e * deltaT;
  float u = _kp * e + _kd * dedt + _ki * _eintegral;

  // Constraints and integral wind-up prevention
  if (u < -255) {
    u = -255;
  } else if (u > 255) {
    u = 255;
  }

  return u;
}
