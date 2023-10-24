#include "MotorDriver.h"

MotorDriver::MotorDriver(int pwmPin, int phasePin, int decayPin) {
  _pwmPin = pwmPin;
  _phasePin = phasePin;
  _decayPin = decayPin;
  pinMode(_pwmPin, OUTPUT);
  pinMode(_phasePin, OUTPUT);
  pinMode(_decayPin, OUTPUT);
}

void MotorDriver::driveMotor(int speed) {
  if (speed > 0) {
    analogWrite(_pwmPin, speed);
    digitalWrite(_phasePin, HIGH);
    digitalWrite(_decayPin, LOW);
  } else if (speed < 0) {
    analogWrite(_pwmPin, -speed);
    digitalWrite(_phasePin, LOW);
    digitalWrite(_decayPin, LOW);
  } else {
    analogWrite(_pwmPin, 0);
    digitalWrite(_phasePin, LOW);
    digitalWrite(_decayPin, HIGH);
  }
}
