#include <analogWrite.h>
#include "Adafruit_VL53L0X.h"
int lidarDistance = 0;
int lidarDistance2 = 0;
int holeDirection = 0;

void activateButler(int LF, int LB, int RF, int RB) {
  analogWrite(5, LF);
  analogWrite(18, LB);
  analogWrite(17, RF);
  analogWrite(16, RB);
}

void driveForwardButler(){
  activate(154, 0, 154, 0);
}

void driveBackwardsButler(){
  activate(0, 200, 0, 200);
}

void turnLeftButler() {
  activate(0, 190, 190, 0);
}

void turnRightButler() {
  activate(190, 0, 0, 190);
}

void stopDrivingButler(){
  activate(0, 0, 0, 0);
}

int getLidarResult(Adafruit_VL53L0X lox){
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  return measure.RangeMilliMeter;
}

boolean searchLeft(unsigned long timeNow, int duration, Adafruit_VL53L0X lox){
  while(millis() < timeNow + duration){
    turnLeftButler();
    lidarDistance2 = getLidarResult(lox);
    
    //when there's a sufficient difference is the old and new lidar value, we know its an opening
    if(lidarDistance2 - lidarDistance > 40){
      holeDirection = 1;
      stopDrivingButler();
      
      //after finding an opening, turn a little more to compensate for the wider frame
      unsigned long timeNow2 = millis();
      int duration2 = 40;
      while(millis() < timeNow2 + duration2){
        turnLeftButler();
      }
      
      return true;
    }
  }
  return false;
}

boolean searchRight(unsigned long timeNow, int duration, Adafruit_VL53L0X lox){
  while(millis() < timeNow + duration){
    turnRightButler();
    lidarDistance2 = getLidarResult(lox);
    
    //when there's a sufficient difference is the old and new lidar value, we know its an opening
    if(lidarDistance2 - lidarDistance > 40){
      holeDirection = 2;
      stopDrivingButler();
      
      //after finding an opening, turn a little more to compensate for the wider frame
      unsigned long timeNow2 = millis();
      int duration2 = 40;
      while(millis() < timeNow2 + duration2){
        turnRightButler();
      }
      
      return true;
    }
  }
  return false;
}

//@param the last measured distance to compare to
//@return will return either the newest lidar measurement or -1 in case of reaching the finish
int startButler(int lidarDistanceOld) {
  Adafruit_VL53L0X lox = Adafruit_VL53L0X();
  lox.begin();
  lidarDistance = lidarDistanceOld;
  lidarDistance2 = getLidarResult(lox);

  int valueRight = analogRead(39);
  int valueLeft = analogRead(34);

  if(valueRight < 2 && valueLeft < 2){
    Serial.println("the name's jeff");
    stopDriving();
    return -1;
  }

  //makes the robot drive forward while no obstacles close by
  if(lidarDistance > 400){
    driveForwardButler();

  //obtstacle found  
  }else{
    stopDrivingButler();

    //depending on the last opening seen, either left or right of the robot gets searched for the next opening
    switch(holeDirection){
      case 0: 
      case 2:
      if(searchLeft(millis(), 600, lox)){
        break;
      }
      searchRight(millis(), 1200, lox);
      break;

      case 1:
      if(searchRight(millis(), 600, lox)){
        break;
      }
      searchLeft(millis(), 1200, lox);
      break;
    }
  }
  
  return lidarDistance2;
}
