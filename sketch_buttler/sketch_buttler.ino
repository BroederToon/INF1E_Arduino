#include <analogWrite.h>
#include "Adafruit_VL53L0X.h"

int Aan = 1;

int MRB = 16;
int MRF = 17;
int MLF = 5;
int MLB = 18;

int RB = 0;
int RF = 0;
int LB = 0;
int LF = 0;

int lidarDistance;
int lidarDistance2;
unsigned long timeNow;
int holeDirection = 0;
int duration = 0;

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
  // makes the robot drive at a predetermined speed
  if(Aan == 0){
    stopDriving();
    
  }else{
    lidarDistance = getLidarResult(lox);
    Serial.println(lidarDistance);
    
    if(lidarDistance > 400){
      driveForward();
      
    }else{
      stopDriving();
      duration = 1000;
      switch(holeDirection){
        case 1:
        duration = 950;
        break;

        case 2:
        duration = 1050;
        break;
      }
      timeNow = millis();
      while(millis() < timeNow + duration){
        turnLeft();
        lidarDistance2 = getLidarResult(lox);
        if(lidarDistance2 - lidarDistance > 40){
          holeDirection = 1;
          stopDriving();

          timeNow = millis();
          duration = 30;
          while(millis() < timeNow + duration){
            turnLeft();
          }
          break;
        }
      }
      stopDriving();
      duration = 2000;
      switch(holeDirection){
        case 1:
        duration = 2050;
        break;

        case 2:
        duration = 1950;
        break;
      }
      timeNow = millis();
      while(millis() < timeNow + duration){
        turnRight();
        lidarDistance2 = getLidarResult(lox);
        if(lidarDistance2 - lidarDistance > 40){
          holeDirection = 2;
          stopDriving();

          timeNow = millis();
          duration = 30;
          while(millis() < timeNow + duration){
            turnRight();
          }
          break;
        }
      }
      stopDriving();
    }
  }
}


//functions
int getLidarResult(Adafruit_VL53L0X lox){
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  return measure.RangeMilliMeter;
}

void setMotors(){
  analogWrite(MLF, LF);
  analogWrite(MLB, LB);
  analogWrite(MRF, RF);
  analogWrite(MRB, RB);
}

void driveForward(){
  RB = 0;
  RF = 170;
  LB = 0;
  LF = 150;
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
  LB = 150;
  RF = 150;
  LF = 0;
  setMotors();
}

void turnRight() {
  LB = 0;
  RB = 150;
  LF = 150;
  RF = 0;
  setMotors();
}
