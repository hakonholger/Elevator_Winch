//Email til filippo: filippo.sanfilippo@ntnu.no

#include <Arduino.h>
#include "MotorDriver.h"
#include "PID.h"
#include "functions.h"
#include "Door.h"
#include "globalPinsAndVariables.h"
#include "dac.h"
#include <LiquidCrystal.h>

// Deklarerer PID og motor 
MotorDriver motor(7, 6, 5, 20, 21);
PID pid(1, 0.001, 0.4, motor);

  // LCD deklarasjon
LiquidCrystal lcd(41, 40, 37, 36, 35, 34);

  // Define pins for stepper
  int A = 69; 
  int A_phase = 68; 
  int B = 67; 
  int B_phase = 66;
   //LED
  int led1 = 48;
  int led2 = 47;
  int led3 = 46;

  // Arrey for køsystem
int queueUpArray[numFloor] = {0};
int queueDownArray[numFloor] = {0};
//starter i etasje 1
int current_Floor = 1;
unsigned long lastActivityTime = 0;

// Bolsk verdi for retning av heis
bool dir = true;
bool halfOpen = false;
bool noQueue = true;
bool moveing = false;

// for debug:
unsigned long lastPrintTime = 0;

  
void setup() {
  Serial.begin(9600);
  
  // LCD
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  lcd.begin(16, 2);

  // DC motor
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(A_phase, OUTPUT); 
  pinMode(B_phase, OUTPUT);
   //LED
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  //1A per motor phase
  dac_init();
  set_dac(4095,4095);
}


void loop() { 

  /* Idle, lukker dører etter satt tid
   og blir stående i samme etasje som sist kommando. */
  Idle();

  /* Buttons, tar inn data ved trykket knapp og legger til
   i køsystemet. */
  buttons();

  /* Directions, setter retining på heis, skifter
  kun hvis det ikke ligger kø på samme retning */
  direction();

  /* moveElevator, gir ønsket etasje inn i pid som deretter
  gir hastighetsignal til motor frem til ønsket etasje er nådd */
  moveElevator();

  /* currentFloor, setter current_Floor variabel til den 
  etasjen heisen befinner seg på. */
  currentFloor();

  /* setLed, tenner LED lampe korresponderende til etasjen
  heisen befinner seg i. */
  setLed();

  /* liquidCrystal printer etasje og kø opp og ned i LCD 
  displayet.*/
  liquidCrystal();




// Alt under her er til Debug, Serial prints...

// Printer opp og ned kø (arrey) og etasjen vi befinner oss i:

unsigned long currentTime = millis();

  if (currentTime - lastPrintTime >= printInterval) {
    lastPrintTime = currentTime;

    // Print kø for oppover
    Serial.print("Queue Up: ");
    for (int i = 0; i < numFloor; i++) {
      Serial.print(queueUpArray[i]);
      Serial.print(" ");
    }
    Serial.println();

    // Print kø for nedover
    Serial.print("Queue Down: ");
    for (int i = 0; i < numFloor; i++) {
      Serial.print(queueDownArray[i]);
      Serial.print(" ");
    }

    // Print etasje:
    Serial.println();
    Serial.print("Current Floor: "); Serial.println(current_Floor);
  
    // Print dør state:
    if(doorIsOpen){
      Serial.println("Door is Open!");
    }
    if(!doorIsOpen){
      Serial.println("Door is Closed!");
    }
  } 
  

}

