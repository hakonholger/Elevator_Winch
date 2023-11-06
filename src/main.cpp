//Email til filippo: filippo.sanfilippo@ntnu.no

#include <Arduino.h>
#include "MotorDriver.h"
#include "PID.h"

MotorDriver motor(7, 6, 5, 20, 21);
PID pid(1, 0.001, 0.001, 3000);

void setup() {
  Serial.begin(9600);
}

void loop() { 
  
  double controlSignal = pid.compute(MotorDriver::getPos());
  motor.driveMotor(controlSignal);


  Serial.print("Posisjon: "); Serial.print(MotorDriver::getPos());
  Serial.print(" Setpunkt: 3000");
  Serial.print(" Pådrag: "); Serial.println(-controlSignal);

  delay(10);
}

