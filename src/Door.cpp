#include "Door.h"
#include "globalPinsAndVariables.h"
#include <dac.h>

  int stepsPerRev = 200;
  int speed = 5000;
  bool doorIsOpen = false;


void openDoor() {
  for(int i = 0; i < stepsPerRev/8; i++){ //halfstep
    digitalWrite(A, HIGH);  //STEP 1
    digitalWrite(A_phase, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(B_phase, HIGH);
    delayMicroseconds (speed);


    digitalWrite(A, LOW);  //STEP 2
    digitalWrite(A_phase, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(B_phase, HIGH);
    delayMicroseconds (speed);


    digitalWrite(A, HIGH); //STEP 3
    digitalWrite(A_phase, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(B_phase, HIGH);
    delayMicroseconds (speed);


    digitalWrite(A, HIGH); //STEP 4
    digitalWrite(A_phase, LOW);
    digitalWrite(B, LOW);
    digitalWrite(B_phase, HIGH);
    delayMicroseconds (speed);


    digitalWrite(A, HIGH); //STEP 5
    digitalWrite(A_phase, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(B_phase, LOW);
    delayMicroseconds (speed);


    digitalWrite(A, LOW); //STEP 6
    digitalWrite(A_phase, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(B_phase, LOW);
    delayMicroseconds (speed);


    digitalWrite(A, HIGH); //STEP 7
    digitalWrite(A_phase, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(B_phase, LOW);
    delayMicroseconds (speed);


    digitalWrite(A, HIGH); //STEP 8
    digitalWrite(A_phase, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(B_phase, HIGH);
    delayMicroseconds (speed);

    doorIsOpen = true;
  }
}

void closeDoor() {

  for(int i = 0; i < stepsPerRev/8; i++){

  digitalWrite(A, HIGH); //STEP 8
    digitalWrite(A_phase, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(B_phase, HIGH);
    delayMicroseconds (speed);

    digitalWrite(A, HIGH); //STEP 7
    digitalWrite(A_phase, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(B_phase, LOW);
    delayMicroseconds (speed);

    digitalWrite(A, LOW); //STEP 6
    digitalWrite(A_phase, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(B_phase, LOW);
    delayMicroseconds (speed);

    digitalWrite(A, HIGH); //STEP 5
    digitalWrite(A_phase, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(B_phase, LOW);
    delayMicroseconds (speed);

    digitalWrite(A, HIGH); //STEP 4
    digitalWrite(A_phase, LOW);
    digitalWrite(B, LOW);
    digitalWrite(B_phase, HIGH);
    delayMicroseconds (speed);

    digitalWrite(A, HIGH); //STEP 3
    digitalWrite(A_phase, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(B_phase, HIGH);
    delayMicroseconds (speed);   

    digitalWrite(A, LOW);  //STEP 2
    digitalWrite(A_phase, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(B_phase, HIGH);
    delayMicroseconds (speed); 

    digitalWrite(A, HIGH);  //STEP 1
    digitalWrite(A_phase, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(B_phase, HIGH);
    delayMicroseconds (speed);

    doorIsOpen = false;
  } 
}