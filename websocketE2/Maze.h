#include <Arduino.h>
#include <WiFi.h>
#include <analogWrite.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_VL53L0X.h>

int RB = 0;
int RF = 0;
int LB = 0;
int LF = 0;

int minBlack = 550;
int minGrey = 60;
int maxGrey = 200;

String colorR = "";
String colorL = "";

String gameAction = "";
unsigned long timermillis = 0;
unsigned long currentmillis = millis();


//functions
void stopWheels() {
  RF = 0;
  LF = 0;
  RB = 0;
  LB = 0;
}
void driveForwardMaze() {
  RF = 107;
  LF = 105;
  RB = 0;
  LB = 0;
}

void turnLeftMaze() { //correct the course to the left if the robot veers off the line
  RF = 0;
  LF = 105;
  RB = 0;
  LB = 0;
}

void turnRightMaze() { //correct the course to the right if the robot veers off the line
  RF = 107;
  LF = 0;
  RB = 0;
  LB = 0;
}

void driveBack() {
  RF = 0;
  LF = 0;
  RB = 87;
  LB = 85;
}

void T90Right() { //drives a bit forward, then make a 90 degree turn to the right.
  if (timermillis == 0) {
    timermillis = millis();
    currentmillis = millis();
  }
  if (currentmillis - timermillis < 200) {
    RF = 85;
    LF = 87;
    RB = 0;
    LB = 0;
  } else if (currentmillis - timermillis < 300) {
    RF = 85;
    LF = 0;
    RB = 0;
    LB = 125;
  } else if (currentmillis - timermillis < 350) {
    RF = 0;
    LF = 0;
    RB = 0;
    LB = 0;
  } else {
    timermillis = 0;
    gameAction = "";
  }
}

void uTurn() { //makes a 180 degree turn for when the robot encounters a dead end
  if (timermillis == 0) {
    timermillis = millis();
    currentmillis = millis();
  }
  if (currentmillis - timermillis < 400) {
    RF = 125;
    LF = 0;
    RB = 0;
    LB = 125;
  } else {
    timermillis = 0;
    gameAction = "";
  }
}

void T90Left() { //runs when a left turn is encountered to check if going straight ahead is an option
  if (timermillis == 0) {
    currentmillis = millis();
    timermillis = millis();
  }
  if (currentmillis - timermillis < 300) {
    RF = 0;
    LF = 85;
    RB = 125;
    LB = 0;
  } else if (currentmillis - timermillis < 350) {
    RF = 0;
    LF = 0;
    RB = 0;
    LB = 0;
  } else if (colorL == "black") {
    if (colorR == "black") {  
    //checks to see if both sensors detect black, which means driving straigh was an option
      //if yes, undo the last turn, and drive forward.
      if (currentmillis - timermillis < 450) { 
        RF = 0;
        LF = 0;
        RB = 87;
        LB = 85;
      } else if (currentmillis - timermillis < 600) {
        if (currentmillis - timermillis < 900) {
          RF = 85;
          LF = 0;
          RB = 0;
          LB = 125;
        } else if (currentmillis - timermillis < 950) {
          RF = 0;
          LF = 0;
          RB = 0;
          LB = 0;
        }
      } else {
        timermillis = 0;
        gameAction = "";
      }
    //else, if no black was detected on both sensors, it means it was just a turn to left, 
    //so drive forward to ensure the sensor wont pick up the black square a second time
    } else if (currentmillis - timermillis < 450) {
      RF = 87;
      LF = 85;
      RB = 0;
      LB = 0;
    } else {
      timermillis = 0;
      gameAction = "";
    }
  } else {
  //if neither sensor detects black, it means the robot drove too far, so it needs to back up a bit
    if (currentmillis - timermillis < 450) {

      RF = 0;
      LF = 0;
      RB = 87;
      LB = 85;
    } else {
      timermillis = 0;
      gameAction = "";
    }
  }
}


boolean startMaze() {
  Adafruit_SSD1306 display(128, 64, &Wire, 4);
  int lineRight = analogRead(34);
  int lineLeft = analogRead(39);

  if(lineRight < 2 && lineLeft < 2){
    stopWheels();
    return true;
  }

//setting the value for the sensors as a range
  if (lineRight <= minGrey) {
    colorR = "white";
  } else if (lineRight > minGrey && lineRight <= maxGrey) {
    colorR = "grey";
  } else if (lineRight >= minBlack) {
    colorR = "black";
  }

  if (lineLeft <= minGrey) {
    colorL = "white";
  } else if (lineLeft > minGrey && lineLeft <= maxGrey) {
    colorL = "grey";
  } else if (lineLeft >= minBlack) {
    colorL = "black";
  }

  //udate current time
  currentmillis = millis();
  // makes the robot drive at a speed determined by the input recieved from the functions
  analogWrite(16, LF);
  analogWrite(17, LB);
  analogWrite(18, RF);
  analogWrite(5, RB);

  if (gameAction != "") {
    if (gameAction == "turnRight") {
      turnRight();
    } else if (gameAction == "turnLeft") {
      turnLeft();
    } else if (gameAction == "driveForward") {
      driveForward();
    } else if (gameAction == "90Right") {
      T90Right();
      return false;;
    } else if (gameAction == "90Left") {
      T90Left();
      return false;
    } else if (gameAction == "uTurn") {
      uTurn();
      return false;
    } else if (gameAction == "stop") {
      stopWheels();
    }
  }



  //calls functions to turn/drive straight when following line
  if (colorR == "white" && colorL == "grey") {
    gameAction = "turnRight";
  }

  if (colorR == "grey" && colorL == "white") {
    gameAction = "turnLeft";
  }

  if (colorR == "white" && colorL == "white") {
    gameAction = "driveForward";
  }

  if (colorR == "black") {
    gameAction = "90Right";
  }

  if (colorR != "black" && colorL == "black") {
    gameAction = "90Left";
  }

  if (colorR == "grey" && colorL == "grey") {
    gameAction = "uTurn";
  }
  return false;
}
