//Email til filippo: filippo.sanfilippo@ntnu.no

#include <Arduino.h>
#include "MotorDriver.h"
#include "PID.h"
#include "functions.h"
#include "Door.h"
#include "globalPinsAndVariables.h"
#include "dac.h"


MotorDriver motor(7, 6, 5, 20, 21);
PID pid(1, 0.001, 0.4, motor);

  // Define pins for stepper
  int A = 69; 
  int A_phase = 68; 
  int B = 67; 
  int B_phase = 66;

void setup() {
  Serial.begin(9600);

    // set pins as output
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(A_phase, OUTPUT); 
  pinMode(B_phase, OUTPUT);

  //1A per motor phase
  dac_init();
  set_dac(4095,4095);
}

void loop() { 


startElevator();


}

