#ifndef MotorDriver_h
#define MotorDriver_h

#include <Arduino.h>

class MotorDriver {
public:
  MotorDriver(int pwmPin, int phasePin, int decayPin);
  void driveMotor(int speed);
private:
  int _pwmPin, _phasePin, _decayPin;
};

#endif
