#include "functions.h"
#include "PID.h"
#include "MotorDriver.h"
#include "globalPinsAndVariables.h"
#include "Door.h"

// Arrey for køsystem
const int numFloor = 3;
int queueUpArray[numFloor] = {0};
int queueDownArray[numFloor] = {0};
int current_Floor;
unsigned long lastActivityTime = 0;




// Bolsk verdi for retning av heis
bool dir = true;
bool noQueue = true;

unsigned long lastPrintTime = 0;
const unsigned long printInterval = 5000; // 5 sekunder i millisekunder

void startElevator(){

  buttons();
  direction();



unsigned long currentTime = millis(); // Hent gjeldende tid
  // Sjekk om det har gått 5 sekunder siden siste utskrift
  if (currentTime - lastPrintTime >= printInterval) {
    lastPrintTime = currentTime; // Oppdater siste utskriftstidspunkt

    // Print kø for oppover
    Serial.println("Queue Up:");
    for (int i = 0; i < numFloor; i++) {
      Serial.print(queueUpArray[i]);
      Serial.print(" ");
    }
    Serial.println(); // Ny linje etter utskrift av kø

    // Print kø for nedover
    Serial.println("Queue Down:");
    for (int i = 0; i < numFloor; i++) {
      Serial.print(queueDownArray[i]);
      Serial.print(" ");
    }
    Serial.println(); // Ny linje etter utskrift av kø
    Serial.println(current_Floor);
  }
}

void buttons(){
  // Oppretter string for inndata
  static String inputString = "";

  while (Serial.available()) {
    char inChar = (char)Serial.read();

    // Når "enter" trykkes, leses inn strengen
    if (inChar == '\n') {
      inputString.trim();

      // Sjekker om input er gyldig:
      if (inputString == "u1" || 
          inputString == "u2" || 
          inputString == "d2" || 
          inputString == "3d" || 
          (inputString.toInt() > 0 && inputString.toInt() <= 3)) 
      {
        Serial.print("Command received: ");
        Serial.println(inputString);
        queue(inputString);
      } 
      else 
      {
        Serial.println("Invalid command.");
      }

      // Nulstiller stringen, sjekker for carriage return
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
    
    // Sett noQueue basert på om begge køene er tomme
    noQueue = upEmpty && downEmpty;

    if(noQueue){
      lastActivityTime = millis();
    }
    else if (upEmpty) {
        // Bare ned-køen har elementer, sett retning nedover
        dir = false;
    } else if (downEmpty) {
        // Bare opp-køen har elementer, sett retning oppover
        dir = true;
    } else {
        // Begge køene har elementer, beholder den nåværende retningen.
        // Ingen kode nødvendig her siden vi ikke endrer 'dir' når begge køene har elementer.
    }
    moveElevator();
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


void removeFromQueueUp(){
  if (queueUpArray[0] == 0) {
    return;
  }
  else{
  for (int i = 0; i < numFloor - 1; i++) {
    queueUpArray[i] = queueUpArray[i + 1];
  }
  queueUpArray[numFloor - 1] = 0;
  }
}

void removeFromQueueDown(){
    if (queueDownArray[0] == 0) {
    return;
  }
  else{
  for (int i = 0; i < numFloor - 1; i++) {
    queueDownArray[i] = queueDownArray[i + 1];
  }
  queueDownArray[numFloor - 1] = 0;
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
if(pos < 1010 && pos > 990){
  current_Floor = 1;
}
if(pos < 2010 && pos > 1990){
  current_Floor = 2;
}
if(pos < 3010 && pos > 2990){
  current_Floor = 3;
}
}

void Idle(){
  const unsigned long DOOR_CLOSE_DELAY = 30000;
    if ((millis() - lastActivityTime) > DOOR_CLOSE_DELAY) {
    if(doorIsOpen){
    closeDoor();
    }
    lastActivityTime = millis();
  }
}


//fiks her noe 
void moveElevator() {
  if(!noQueue){
  if (dir == true) { 
    if(queueUpArray[0] != 0){
    int set = queueUpArray[0];
    pid.compute(set);
    currentFloor();
    if (current_Floor == set) {
      if(!doorIsOpen){
      openDoor();
      }
      removeFromQueueUp();
    }}
  } 
  else if (dir == false) {
    if(queueDownArray[0] != 0){
      int set = queueDownArray[0];
      pid.compute(set);
    if (current_Floor == set) {
      if(!doorIsOpen){
      openDoor();
      }
      removeFromQueueDown();
    }}
  }
}
}