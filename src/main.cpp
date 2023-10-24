#include <Arduino.h>
#include "MotorDriver.h"
#include "PIDController.h"

const int ENCA = 20;
const int ENCB = 21;

MotorDriver motor(7, 6, 5);
PIDController pid(1, 0.001, 0.001, 3000);
int pos = 0;

void readEncoder();

void setup() {
  Serial.begin(9600);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
}

void loop() {
  float controlSignal = pid.compute(pos);
  motor.driveMotor(-controlSignal);

  Serial.print("Posisjon: "); Serial.print(pos);
  Serial.print(" Setpunkt: 3000");
  Serial.print(" Pådrag: "); Serial.println(-controlSignal);

  delay(10);
}

void readEncoder() {
  int b = digitalRead(ENCB);
  if (b > 0) {
    pos++;
  } else {
    pos--;
  }
}
