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
/*
pinMode(MLF,OUTPUT);
pinMode(MLB,OUTPUT);
pinMode(MRF,OUTPUT);
pinMode(MRB,OUTPUT);
*/

Serial.begin(9600);
}

void loop() {

int valueRight = analogRead(lineSensorRight);
int valueLeft = analogRead(lineSensorLeft);

i++;

Serial.println(i);

//Serial.println(valueRight);
//Serial.println(valueLeft);

  // makes the robot drive at a predetermined speed
  if(Aan == 1){
analogWrite(MLF, LF);
analogWrite(MLB, LB);
analogWrite(MRF, RF);
analogWrite(MRB, RB);

  //calls functions to turn/drive straight when following line
if(valueRight < 500 && valueLeft > 500) {
  turnRight();
}



if(valueLeft < 500 && valueRight > 500) {
  turnLeft();
}


  
if(valueLeft > 500 && valueRight > 500) {
  driveForward();
}



// if(valueLeft < 500 && valueRight < 500) {
//  stopCar();
//  }
}
//wielen uit als aan == 0
else
{
analogWrite(MLF, 0);
analogWrite(MLB, 0);
analogWrite(MRF, 0);
analogWrite(MRB, 0);
}
}

//functions
void driveForward(){
  RB = 107;
  RF = 0;
  LB = 105;
  LF = 0;
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

//void stopCar() {
// // if(i == 200) {
//  RB = 0;
//  RF = 0;
//  LB = 0;
//  LF = 0;
// }
// if (i < 200) {
//   i++;
// }
//  }
