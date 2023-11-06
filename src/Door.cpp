#include <dac.h>

  int A = 69; // pin connected to enable A
  int A_phase = 68; // pin connected to phase A
  int B = 67; // pin connected to enable B
  int B_phase = 66; // pin connected to phase B
  int stepsPerRev = 200; // defining steps per revolution
  int speed = 5000;
  bool doorIsOpen = 0;

void openDoor();
void closeDoor();

void setup() {
  
 
  Serial.begin(9600);
  // set pins as output
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(A_phase, OUTPUT); 
  pinMode(B_phase, OUTPUT);

  dac_init();
  set_dac(4095,4095); //1A per motor phase


}

void loop(){

openDoor();
Serial.println("Door is open");
delay(1000);
closeDoor();
Serial.println("Door is closed");
delay(1000);



}

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

  }

  doorIsOpen = 1;
  

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


    
  }

  doorIsOpen = 0;


}