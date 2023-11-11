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

  // Arrey for køsystem
int queueUpArray[numFloor] = {0};
int queueDownArray[numFloor] = {0};
//starter i etasje 1
int current_Floor = 1;
unsigned long lastActivityTime = 0;

// Bolsk verdi for retning av heis
bool dir = true;
bool noQueue = true;

// for debug:
unsigned long lastPrintTime = 0;

  
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

  Idle();
  buttons();
  direction();
  moveElevator();
  currentFloor();



unsigned long currentTime = millis(); // Hent gjeldende tid
  // Sjekk om det har gått 5 sekunder siden siste utskrift


  //debug:
  if (currentTime - lastPrintTime >= printInterval) {
    lastPrintTime = currentTime; // Oppdater siste utskriftstidspunkt

    // Print kø for oppover
    Serial.print("Queue Up: ");
    for (int i = 0; i < numFloor; i++) {
      Serial.print(queueUpArray[i]);
      Serial.print(" ");
    }
    Serial.println(); // Ny linje etter utskrift av kø

    // Print kø for nedover
    Serial.print("Queue Down: ");
    for (int i = 0; i < numFloor; i++) {
      Serial.print(queueDownArray[i]);
      Serial.print(" ");
    }
    Serial.println(); // Ny linje etter utskrift av kø
    Serial.print("Current Floor: "); Serial.println(current_Floor);
  
    if(doorIsOpen){
      Serial.println("Door is Open!");
    }
    if(!doorIsOpen){
      Serial.println("Door is Closed!");
    }
  }

}

