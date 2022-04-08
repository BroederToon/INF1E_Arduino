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
  startRace();
}

//functions
boolean startRace(){
  int valueRight = analogRead(lineSensorRight);
  int valueLeft = analogRead(lineSensorLeft);
  Serial.println(stuckCounter);

  if(finished){
    stopDriving();
  }

  if(valueRight < 40 && valueLeft < 40){
    finished = true;
  }

  if(valueRight > 140 && valueLeft > 140){
    driveBackwards();
  }else{

    if(valueRight > 140){
      turnLeft();
    }
  
    if(valueLeft > 140){
      turnRight();
    }
  
    if(valueLeft < 140 && valueRight < 140){
      driveForward();
    }
  }
}

void driveForward(){
  RB = 230;
  RF = 0;
  LB = 215;
  LF = 0;
  activate();
}

void driveBackwards(){
  RB = 0;
  RF = 200;
  LB = 0;
  LF = 200;
  activate();
}

void turnLeft() {
  LB = 0;
  RB = 190;
  LF = 190;
  RF = 0;
  activate();
}

void turnRight() {
  RB = 0;
  LB = 190;
  RF = 190;
  LF = 0;
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
