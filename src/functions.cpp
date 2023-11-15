#include <LiquidCrystal.h>
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
        if(j < numFloor -1){
        queueUpArray[j + 1] = queueUpArray[j];
        }
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
    break;
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
// Knapp på etasje 1:
if(inputString == "u1"){
  queueDown(1);
}
// Knapp på etasje 2:
if(inputString == "u2"){
  queueUp(2);
}
if(inputString == "d2"){
  queueDown(2);
}
// Knapp på etasje 3:
if(inputString == "d3"){
  queueUp(3);
}
// Knapper inne i heis:
if(inputString == "1" && current_Floor > 1){
queueDown(1);
}
if(inputString == "1" && current_Floor == 1){
  if(!doorIsOpen && noQueue){
  openDoor();
  }
}
if(inputString == "2" && current_Floor < 2){
queueUp(2);
}
if(inputString == "2" && current_Floor > 2){
queueDown(2);
}
if(inputString == "2" && current_Floor == 2){
  if(!doorIsOpen && noQueue){
  openDoor();
  }
}
if(inputString == "3" && current_Floor < 3){
queueUp(3);
}
if(inputString == "3" && current_Floor == 3){
  if(!doorIsOpen && noQueue){
  openDoor();
  }
}
}



// UTENFOR INTERVALL!!! SJEKK
void currentFloor(){
int pos = motor.getPos();
if(pos < 1*2000*5+5 && pos > 1*2000*5-5){
  current_Floor = 1;
  moveing = false;
}
else if(pos < 2*2000*5+5 && pos > 2*2000*5-5){
  current_Floor = 2;
  moveing = false;
  Serial.println(moveing);
}
else if(pos < 3*2000*5+5 && pos > 3*2000*5-5){
  current_Floor = 3;
  moveing = false;
}
else{
  moveing = true;
}
}



void  Idle(){
  // Setter idle til 2 minutter etter siste activitet.
  const unsigned long DOOR_CLOSE_DELAY = 60000*2;

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
    windingLCD();
    pid.compute(set);
    if (current_Floor == set) {
      motor.driveMotor(0);
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
      unwindingLCD();
      pid.compute(set);
    if (current_Floor == set) {
      motor.driveMotor(0);
      if(!doorIsOpen){
      openDoor();
      }
      removeFromQueue(queueDownArray);
    }}
  }
}
}


void setLed(){
  digitalWrite(led1, 0);
  digitalWrite(led2, 0);
  digitalWrite(led3, 0);

switch(current_Floor){
  case 1:
    digitalWrite(led1, 1);
    break;
  case 2:
    digitalWrite(led2, 1);
    break;
  case 3:
    digitalWrite(led3, 1);
    break;
}
}


void liquidCrystal(){
  if(moveing == false){
switch(current_Floor){
  case 1:
    lcd.clear();
    lcd.print("Floor: 1");
    /*
    lcd.setCursor(0, 2); 
    lcd.print("UP[   ]");
    lcd.setCursor(3, 2);
    lcd.print(queueUpArray[0]);
    lcd.setCursor(4, 2);
    lcd.print(queueUpArray[1]);
    lcd.setCursor(5, 2);
    lcd.print(queueUpArray[2]);
    lcd.setCursor(7, 2);
    lcd.print("DOWN[   ]");
    lcd.setCursor(12, 2);
    lcd.print(queueDownArray[0]);
    lcd.setCursor(13, 2);
    lcd.print(queueDownArray[1]);
    lcd.setCursor(14, 2);
    lcd.print(queueDownArray[2]);
    lcd.setCursor(15, 2);
    */
    delay(20);
    break;
  case 2:
    lcd.clear();
    lcd.print("Floor: 2");
    /*
    lcd.setCursor(0, 2); 
    lcd.print("UP[   ]");
    lcd.setCursor(3, 2);
    lcd.print(queueUpArray[0]);
    lcd.setCursor(4, 2);
    lcd.print(queueUpArray[1]);
    lcd.setCursor(5, 2);
    lcd.print(queueUpArray[2]);
    lcd.setCursor(7, 2);
    lcd.print("DOWN[   ]");
    lcd.setCursor(12, 2);
    lcd.print(queueDownArray[0]);
    lcd.setCursor(13, 2);
    lcd.print(queueDownArray[1]);
    lcd.setCursor(14, 2);
    lcd.print(queueDownArray[2]);
    lcd.setCursor(15, 2);
    */
    delay(20);
    break;
  case 3:
    lcd.clear();
    lcd.print("Floor: 3");
    /*
    lcd.setCursor(0, 2); 
    lcd.print("UP[   ]");
    lcd.setCursor(3, 2);
    lcd.print(queueUpArray[0]);
    lcd.setCursor(4, 2);
    lcd.print(queueUpArray[1]);
    lcd.setCursor(5, 2);
    lcd.print(queueUpArray[2]);
    lcd.setCursor(7, 2);
    lcd.print("DOWN[   ]");
    lcd.setCursor(12, 2);
    lcd.print(queueDownArray[0]);
    lcd.setCursor(13, 2);
    lcd.print(queueDownArray[1]);
    lcd.setCursor(14, 2);
    lcd.print(queueDownArray[2]);
    lcd.setCursor(15, 2);
    */
    delay(20);
    break;
    }
  }
}

void windingLCD(){
if(moveing){
lcd.clear();
lcd.print("Winding");
  }
}

void unwindingLCD(){
if(moveing){
lcd.clear();
lcd.print("Unwinding");
  }
}