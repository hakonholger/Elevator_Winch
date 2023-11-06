#include "functions.h"
#include "PID.h"
#include "MotorDriver.h"

void startElevator(PID& pid){


pid.compute(3000);


    delay(10);
}

