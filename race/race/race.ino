#include <analogWrite.h>

int Aan = 1;

int MRB = 16;
int MRF = 17;
int MLF = 5;
int MLB = 18;
int lineSensorRight = 39;
int lineSensorLeft = 34;

int RB = 0;
int RF = 0;
int LB = 0;
int LF = 0;

int i = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(MLF,OUTPUT);
  pinMode(MLB,OUTPUT);
  pinMode(MRF,OUTPUT);
  pinMode(MRB,OUTPUT);
  
  Serial.begin(9600);

  delay(2000);
}

void loop() {
  int valueRight = analogRead(lineSensorRight);
  int valueLeft = analogRead(lineSensorLeft);

  if(valueRight > 100 && valueLeft > 100){
    backOff(750);
  }

  while(valueRight > 100){
    valueRight = analogRead(lineSensorRight);
    valueLeft = analogRead(lineSensorLeft);
    turnLeft();
  }

  while(valueLeft > 100){
    valueRight = analogRead(lineSensorRight);
    valueLeft = analogRead(lineSensorLeft);
    turnRight();
  }
  driveForward();

}

//functions
void driveForward(){
  RB = 180;
  RF = 0;
  LB = 180;
  LF = 0;
  activate();
}

void backOff(int duration){
  Serial.println("backwards, GOOOOOOOOOOO");
  unsigned long timeNow = millis();
  while(millis() < timeNow + duration){
    driveBackwards();
  }
}

void driveBackwards(){
  RB = 0;
  RF = 180;
  LB = 0;
  LF = 180;
  activate();
}

void turnLeft() {
  RB = 0;
  LB = 180;
  RF = 180;
  LF = 0;
  activate();
}

void turnRight() {
  LB = 0;
  RB = 180;
  LF = 180;
  RF = 0;
  activate();
}

void stopDriving(){
  LB = 0;
  RB = 0;
  LF = 0;
  RF = 0;
  activate();
}

void activate() {
  analogWrite(MLF, LF);
  analogWrite(MLB, LB);
  analogWrite(MRF, RF);
  analogWrite(MRB, RB);
}
