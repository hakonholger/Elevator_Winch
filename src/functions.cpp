#include "functions.h"
#include "PID.h"
#include "MotorDriver.h"
#include "globalPinsAndVariables.h"
#include "Door.h"


void buttons(){
  // Inndata string:
  static String inputString = "";

  while (Serial.available()) {
    char inChar = (char)Serial.read();
    // Leses inn når enter trykkes:
    if (inChar == '\n') {
      inputString.trim();
      // Gyldig inputt?:
      if (inputString == "u1" || 
          inputString == "u2" || 
          inputString == "d2" || 
          inputString == "d3" || 
          (inputString.toInt() > 0 && inputString.toInt() <= 3)) {
        Serial.print("Command received: ");
        Serial.println(inputString);
        queue(inputString);
        lastActivityTime = millis();
      } 
      else {
        Serial.println("Invalid command.");
      }
      // Nuller ut sting:
      inputString = "";
    } 
    else if (inChar != '\r') {
      inputString += inChar;
    }
  }
}

bool isQueueEmpty(int queue[], int size) {
  for (int i = 0; i < size; i++) {
    if (queue[i] != 0) {
      return false;
    }
  }
  return true;
}

void direction() {
    bool upEmpty = isQueueEmpty(queueUpArray, numFloor);
    bool downEmpty = isQueueEmpty(queueDownArray, numFloor);
    noQueue = upEmpty && downEmpty;
    
    // Kø ned ikke tom:
    if (upEmpty) {
        dir = false;
    }
    // Kø opp ikke tom: 
    else if (downEmpty) {
        dir = true;
    } 
  }

void queueUp(int floor) {
  for (int i = 0; i < numFloor; i++) {
    if (floor == queueUpArray[i]) { 
      return;
    }
    if (floor < queueUpArray[i] || queueUpArray[i] == 0) {
      for (int j = numFloor - 1; j >= i; j--) {
        queueUpArray[j + 1] = queueUpArray[j];
      }
      queueUpArray[i] = floor; 
      break;
    }
  }
}

void queueDown(int floor) {
  for (int i = 0; i < numFloor; i++) {
    if (floor == queueDownArray[i]) { 
      return;
    }
    if (floor > queueDownArray[i] || queueDownArray[i] == 0) {
      for (int j = numFloor - 1; j > i; j--) {
      queueDownArray[j] = queueDownArray[j - 1];
    }
    queueDownArray[i] = floor;
    }
  }
}

void removeFromQueue(int queueArrey[]){
  if (queueArrey[0] == 0) {
    return;
  }
  else{
  for (int i = 0; i < numFloor - 1; i++) {
    queueArrey[i] = queueArrey[i + 1];
  }
  queueArrey[numFloor - 1] = 0;
  }
}

void queue(String inputString){
// Pressed at floor 1:
if(inputString == "u1"){
  queueDown(1);
}
// Pressed at floor 2:
if(inputString == "u2"){
  queueUp(2);
}
if(inputString == "d2"){
  queueDown(2);
}
// Pressed at floor 3:
if(inputString == "d3"){
  queueUp(3);
}
// Pressed inside elevator:
if(inputString == "1" && current_Floor > 1){
queueDown(1);
}
if(inputString == "1" && current_Floor == 1){
openDoor();
}
if(inputString == "2" && current_Floor < 2){
queueUp(2);
}
if(inputString == "2" && current_Floor > 2){
queueDown(2);
}
if(inputString == "2" && current_Floor == 2){
openDoor();
}
if(inputString == "3" && current_Floor < 3){
queueUp(3);
}
if(inputString == "3" && current_Floor == 3){
openDoor();
}
}

void currentFloor(){
int pos = motor.getPos();
if(pos < 1005 && pos > 995){
  current_Floor = 1;
}
if(pos < 2015 && pos > 1995){
  current_Floor = 2;
}
if(pos < 3015 && pos > 2995){
  current_Floor = 3;
}
}

void Idle(){
  const unsigned long DOOR_CLOSE_DELAY = 10000;
    if ((millis() - lastActivityTime) > DOOR_CLOSE_DELAY) {
    if(doorIsOpen){
    closeDoor();
    lastActivityTime = millis();
    }
    }
  }

void moveElevator() {
  if(!noQueue){
    // Opp:
  if (dir == true) { 
    if(queueUpArray[0] != 0){
    int set = queueUpArray[0];
    if(doorIsOpen){
    closeDoor();
    }
    pid.compute(set);
    motor.driveMotor(0);
    if (current_Floor == set) {
      if(!doorIsOpen){
      openDoor();
      }
      removeFromQueue(queueUpArray);
    }}
  } 
  // Ned:
  else if (dir == false) {
    if(queueDownArray[0] != 0){
      int set = queueDownArray[0];
      if(doorIsOpen){
      closeDoor();
      }
      pid.compute(set);
      motor.driveMotor(0);
    if (current_Floor == set) {
      if(!doorIsOpen){
      openDoor();
      }
      removeFromQueue(queueDownArray);
    }}
  }
}
}