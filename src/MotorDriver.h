#ifndef MotorDriver_h
#define MotorDriver_h

#include <Arduino.h>


class MotorDriver {
public:
  MotorDriver(int pwmPin, int phasePin, int decayPin, int encaPin, int encbPin);
  void driveMotor(int speed);
  static void readEncoder();
  static int getPos();
private:
  int _pwmPin, _phasePin, _decayPin, _encaPin, _encbPin;
  static int pos;
};

#endif
