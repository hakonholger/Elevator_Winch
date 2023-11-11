// pins.h
#ifndef globalPinsAndVariables_h
#define globalPinsAndVariables_h

#include"PID.h"
#include"MotorDriver.h"

// Stepper:
extern int A;
extern int A_phase;
extern int B;
extern int B_phase;


extern PID pid;
extern MotorDriver motor;

extern bool doorIsOpen;

// Arrey for køsystem
const int numFloor = 3;
extern int queueUpArray[numFloor];
extern int queueDownArray[numFloor];
//starter i etasje 1
extern int current_Floor;
extern unsigned long lastActivityTime;

// Bolsk verdi for retning av heis
extern bool dir;
extern bool noQueue;

// for debug:
extern unsigned long lastPrintTime;
const unsigned long printInterval = 5000; // 5 sekunder i millisekunder

#endif