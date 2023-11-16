#include "MotorDriver.h"

int MotorDriver::pos = 0;

MotorDriver::MotorDriver(int pwmPin, int phasePin, int decayPin, int encaPin, int encbPin) {
  _pwmPin = pwmPin;
  _phasePin = phasePin;
  _decayPin = decayPin;
  _encaPin = encaPin;
  _encbPin = encbPin;

  pinMode(_pwmPin, OUTPUT);
  pinMode(_phasePin, OUTPUT);
  pinMode(_decayPin, OUTPUT);
  pinMode(_encaPin, INPUT);
  pinMode(_encbPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(_encaPin), MotorDriver::readEncoder, RISING);
}

//This void function checks whether the speed variable is positive, negative or zero. It will send the raw speed value to
//the PWM pin, and setting the correct combination for direction pins.
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

int MotorDriver::getPos() {
  return pos;
}

//The readEncoder function checks whether encoder pin B are high once encoder pin A are on rising edge to determine
//if the direction is positive or negative. If B is high, it means that the direction is positive. If B is low
//once A is on rising edge, it means negative direction.

void MotorDriver::readEncoder() {
  int ENCB = 21;
  int b = digitalRead(ENCB);
  if (b > 0) {
    pos++;
  } else {
    pos--;
  }
}