#ifndef functions_h
#define functions_h

#include <Arduino.h>
#include "PID.h"
#include "Door.h"

void startElevator();
void buttons();
void queueUp(int floor);
void queueDown(int floor);
void queue(String inputString);
bool isQueueEmpty(int queue[], int size);
void direction();
void currentFloor();
void moveElevator();
void removeFromQueueUp();
void removeFromQueueDown();



#endif