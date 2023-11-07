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

// Buttons:

#endif