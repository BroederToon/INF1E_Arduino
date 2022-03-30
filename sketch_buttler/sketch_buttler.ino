#include <analogWrite.h>
#include "Adafruit_VL53L0X.h"

int Aan = 1;

//motor pins
int MRB = 16;
int MRF = 17;
int MLF = 5;
int MLB = 18;

//pin power
int RB = 0;
int RF = 0;
int LB = 0;
int LF = 0;

//the older and newer lidar data
int lidarDistance;
int lidarDistance2;

//number of searches for an opening with no result
int stuckCount;

//the direction the last seen hole was in. a value of 1 being left, 2 being right, and 0 neutral
int holeDirection = 0;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
void setup() {
  pinMode(MLF,OUTPUT);
  pinMode(MLB,OUTPUT);
  pinMode(MRF,OUTPUT);
  pinMode(MRB,OUTPUT);
  
  Serial.begin(115200);
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  lox.begin();
}

void loop() {
  if(Aan == 0){
    stopDriving();
    
  }else{
    lidarDistance = getLidarResult(lox);
    Serial.println(lidarDistance);

    //checks if the robot is stuck
//    if(stuckCount > 4){
//      
//      driveBackwards();
//    }

    //makes the robot drive forward while no obstacles close by
    if(lidarDistance > 400){
      driveForward();

    //obtstacle found  
    }else{
      stopDriving();

      //depending on the last opening seen, either left or right of the robot gets searched for the next opening
      switch(holeDirection){
        case 0: 
        case 2:
        if(searchLeft(millis(), 600)){
          break;
        }
        searchRight(millis(), 1200);
        break;

        case 1:
        if(searchRight(millis(), 600)){
          break;
        }
        searchLeft(millis(), 1200);
        break;
      }
    }
  }
}


//functions
int getLidarResult(Adafruit_VL53L0X lox){
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  return measure.RangeMilliMeter;
}

boolean searchLeft(unsigned long timeNow, int duration){
  while(millis() < timeNow + duration){
    turnLeft();
    lidarDistance2 = getLidarResult(lox);
    
    //when there's a sufficient difference is the old and new lidar value, we know its an opening
    if(lidarDistance2 - lidarDistance > 40){
      stuckCount = 0;
      holeDirection = 1;
      stopDriving();
      
      //after finding an opening, turn a little more to compensate for the wider frame
      unsigned long timeNow2 = millis();
      int duration2 = 10;
      while(millis() < timeNow2 + duration2){
        turnLeft();
      }
      
      return true;
    }
  }
  return false;
}

boolean searchRight(unsigned long timeNow, int duration){
  while(millis() < timeNow + duration){
    turnRight();
    lidarDistance2 = getLidarResult(lox);
    
    //when there's a sufficient difference is the old and new lidar value, we know its an opening
    if(lidarDistance2 - lidarDistance > 40){
      stuckCount = 0;
      holeDirection = 2;
      stopDriving();
      
      //after finding an opening, turn a little more to compensate for the wider frame
      unsigned long timeNow2 = millis();
      int duration2 = 10;
      while(millis() < timeNow2 + duration2){
        turnRight();
      }
      
      return true;
    }
  }
  return false;
}

void setMotors(){
  analogWrite(MLF, LF);
  analogWrite(MLB, LB);
  analogWrite(MRF, RF);
  analogWrite(MRB, RB);
}

void driveForward(){
  RB = 0;
  RF = 150;
  LB = 0;
  LF = 154;
  setMotors();
}

void driveBackwards(){
  RB = 150;
  RF = 0;
  LB = 154;
  LF = 0;
  setMotors();
}

void stopDriving(){
  RB = 0;
  RF = 0;
  LB = 0;
  LF = 0;
  setMotors();
}

void turnLeft() {
  RB = 0;
  LB = 170;
  RF = 170;
  LF = 0;
  setMotors();
}

void turnRight() {
  LB = 0;
  RB = 170;
  LF = 170;
  RF = 0;
  setMotors();
}
