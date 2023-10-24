#include <Arduino.h>

const int ENCA = 20;
const int ENCB = 21;
const int pwmSignal = 7;
const int phase = 6;
const int decay = 5;

//PID
int pos = 0;
long prevT = 0;
float eprev = 0;
float eintegral = 0;


void setup() {
  Serial.begin(9600);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  pinMode(pwmSignal, OUTPUT);
  pinMode(phase, OUTPUT);
  pinMode(decay, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
}

void loop() {

int target = 3000; //Settpunkt for DC-Motor

// PID parameters
  float kp = 1;
  float kd = 0;
  float ki = 0;
  int margin = 10;

  //compute time difference
  long currT = micros();

  float deltaT = ((float)(currT-prevT))/1.0e6;
  prevT = currT;
  int e = pos-target;
  float dedt = (e-eprev)/(deltaT);
  eintegral = eintegral + e*deltaT;
  float u = kp*e + kd*dedt + ki*eintegral;

  if(u<-255){
    u = -255;
  }
  else if(u>255){
    u = 255;
  }
  if(abs(e)<margin){
    u = 0;
    eintegral = 0; 
  }

driveMotor(-u);


Serial.print("Posisjon:"); Serial.print(" "); Serial.print(pos); Serial.print(" ");
Serial.print("Setpunkt"); Serial.print(" "); Serial.print(target); Serial.print(" ");
Serial.print("Pådrag"); Serial.print(" "); Serial.print(-u);
Serial.println();
delay(10);
}



void driveMotor(int speed) {
  if (speed > 0) {
    analogWrite(pwmSignal, speed);
    digitalWrite(phase, HIGH); // Fremover retning
    digitalWrite(decay, LOW);
  } else if (speed < 0) {
    analogWrite(pwmSignal, -speed);
    digitalWrite(phase, LOW); // Bakover retning
    digitalWrite(decay, LOW);
  } else {
    analogWrite(pwmSignal, 0); //Brems og stopp
    digitalWrite(phase, LOW);
    digitalWrite(decay, HIGH);
  }
}

void readEncoder(){
  int b = digitalRead(ENCB);
  if(b>0){
    pos++;
  }
  else{
    pos--;
  }
}