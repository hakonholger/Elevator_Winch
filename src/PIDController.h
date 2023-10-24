#ifndef PIDController_h
#define PIDController_h

#include <Arduino.h>

class PIDController {
public:
  PIDController(float kp, float ki, float kd, int target);
  float compute(int currentPos);
private:
  float _kp, _ki, _kd;
  int _target;
  float _eprev;
  long _prevT;
  float _eintegral;
};

#endif
