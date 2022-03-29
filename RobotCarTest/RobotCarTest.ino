#include <analogWrite.h>

int Aan = 1;

int MRB = 16;
int MRF = 17;
int MLF = 5;
int MLB = 18;

int RB = 0;
int RF = 0;
int LB = 0;
int LF = 0;

void setup() {
  pinMode(MLF,OUTPUT);
  pinMode(MLB,OUTPUT);
  pinMode(MRF,OUTPUT);
  pinMode(MRB,OUTPUT);
  
  Serial.begin(115200);
}

void loop() {
  // makes the robot drive at a predetermined speed
  if(Aan == 1){
    driveForward();
    Serial.println(RB);
    analogWrite(MLF, LF);
    analogWrite(MLB, LB);
    analogWrite(MRF, RF);
    analogWrite(MRB, RB);
  }else{
    analogWrite(MLF, 0);
    analogWrite(MLB, 0);
    analogWrite(MRF, 0);
    analogWrite(MRB, 0);
  }
}

//functions
void driveForward(){
  RB = 0;
  RF = 250;
  LB = 0;
  LF = 250;
}

void turnLeft() {
//  if(RB > 77) {
//  RB = RB - 10;
  RB = 0;
  LB = 105;
  RF = 105;
  LF = 0;
//  LB = LB + 5;
//  }
//  else {
//    RB = 0;
//  }
}

void turnRight() {
//  if(LB > 75) {
//  LB = LB - 10;
  LB = 0;
  RB = 105;
  LF = 105;
  RF = 0;
//  RB = RB + 5;
//  } else {
//    LB = 0;
//  }
}
