#include <analogWrite.h>

int Aan = 1;

boolean finished = false;

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

int stuckCounter = 0;

unsigned long timeDiff;

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

  if(finished){
    stopDriving();
  }

  if(valueRight < 40 && valueLeft < 40){
    finished = true;
  }

  if(valueRight > 140 && valueLeft > 140){
    stuckCounter++;
  }

  if(valueRight > 140){
    turnRight();
  }

  if(valueLeft > 140){
    turnLeft();
  }

  if(stuckCounter > 1000){
    backOff(2000);
  }

  if(valueLeft < 140 && valueRight < 140){
    driveForward();
  }

}

//functions
void driveForward(){
  RB = 195;
  RF = 0;
  LB = 180;
  LF = 0;
  activate();
}

void backOff(int duration){
  Serial.println("backwards, GOOOOOOOOOOO");
  unsigned long timeNow = millis();
  while(millis() < timeNow + duration){
    Serial.println("check");
    driveBackwards();
  }
}

void driveBackwards(){
  RB = 0;
  RF = 100;
  LB = 0;
  LF = 100;
  activate();
}

void turnLeft() {
  RB = 0;
  LB = 150;
  RF = 150;
  LF = 0;
  activate();
}

void turnRight() {
  LB = 0;
  RB = 150;
  LF = 150;
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
