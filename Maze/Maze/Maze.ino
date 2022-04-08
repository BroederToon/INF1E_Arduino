#include <Arduino.h>
#include <WiFi.h>
#include <analogWrite.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_VL53L0X.h>


int Aan = 1;

int MLF = 16;
int MLB = 17;
int MRB = 5;
int MRF = 18;
int lineSensorRight = 34;
int lineSensorLeft = 39;
int LineRight = 0;
int LineLeft = 0;

int RB = 0;
int RF = 0;
int LB = 0;
int LF = 0;

int minBlack = 201;
int minGrey = 60;
int maxGrey = 200;

String colorR = "";
String colorL = "";

String gameAction = "";
unsigned long timermillis = 0;
unsigned long currentmillis = millis();

Adafruit_SSD1306 display(128, 64, &Wire, 4);
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  

  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());


  //setup display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.display();




}

void loop() {

  int LineRight = analogRead(lineSensorRight);
  int LineLeft = analogRead(lineSensorLeft);
  
  if(LineRight <= minGrey){
    colorR = "white";
  } else if ( LineRight > minGrey && LineRight <= maxGrey ) {
    colorR = "grey";
  } else if ( LineRight >= minBlack){
    colorR = "black";
  }

  if(LineLeft <= minGrey){
    colorL = "white";
  } else if (LineLeft > minGrey && LineLeft <= maxGrey) {
    colorL = "grey";
  } else if (LineLeft >= minBlack){
    colorL = "black";
  }

  //clear the display
  display.clearDisplay();

  //write the values from the IR sensor on the display
  display.setCursor(0, 0);            // Start at top-left corner
  display.print("LeftSens: ");
  display.println(LineLeft);
  display.print("RightSens: ");
  display.println(LineRight);
//  display.println(gameAction);
  display.display();

  //udate current time
  currentmillis = millis();
  // makes the robot drive at a predetermined speed
  if (Aan == 1) {
    analogWrite(MLF, LF);
    analogWrite(MLB, LB);
    analogWrite(MRF, RF);
    analogWrite(MRB, RB);

   if (gameAction != "") {
      if (gameAction == "turnRight"){
        turnRight();
      }
      else if (gameAction == "turnLeft"){
        turnLeft();
      }
      else if (gameAction == "driveForward"){
        driveForward();
      }
      else if (gameAction == "90Right") {
        T90Right();
        return;
      }
      else if (gameAction == "90Left"){
        T90Left();
        return;
      }
      else if (gameAction == "uTurn"){
        uTurn();
        return;
      }
      else if (gameAction == "stop") {
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

    if (colorR != "black" && colorL == "black" ) {
      gameAction = "90Left";
    }

    if (colorR == "grey" && colorL == "grey") {
      gameAction = "uTurn";
    }

  }
  //wielen uit als aan == 0
  if (Aan == 0)
  {
    analogWrite(MLF, 0);
    analogWrite(MLB, 0);
    analogWrite(MRF, 0);
    analogWrite(MRB, 0);
  }
}

//functions
void stopWheels() {
  RF = 0;
  LF = 0;
  RB = 0;
  LB = 0;
}
void driveForward() {
  RF = 107;
  LF = 105;
  RB = 0;
  LB = 0;
}

void turnLeft() {
  RF = 0;
  LF = 105;
  RB = 0;
  LB = 0;
}

void turnRight() {
  RF = 107;
  LF = 0;
  RB = 0;
  LB = 0;
}

void driveBack () {
  RF = 0;
  LF = 0;
  RB = 87;
  LB = 85;
}

void T90Right() {
  if (timermillis == 0) {
    timermillis = millis();
    currentmillis = millis();
  } if (currentmillis - timermillis < 200) {
    RF = 85;
    LF = 87;
    RB = 0;
    LB = 0;
  }
  else if (currentmillis - timermillis < 300) {
    RF = 85;
    LF = 0;
    RB = 0;
    LB = 125;
  }
  else if (currentmillis - timermillis < 350) {
    RF = 0;
    LF = 0;
    RB = 0;
    LB = 0;
  } else {
    timermillis = 0;
    gameAction = "";
  }
}

void uTurn() {
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

void T90Left() {
      if (timermillis == 0) {
        currentmillis = millis();
        timermillis = millis();
      }  if (currentmillis - timermillis < 300) {
    RF = 0;
    LF = 85;
    RB = 125;
    LB = 0;
  }
  else if (currentmillis - timermillis < 350) {
    RF = 0;
    LF = 0;
    RB = 0;
    LB = 0;
      } else   if (colorL == "black") {
    if (colorR == "black") {
      if (currentmillis - timermillis < 450) {
  RF = 0;
  LF = 0;
  RB = 87;
  LB = 85;
      }
      else if (currentmillis - timermillis < 600) {
        if (currentmillis - timermillis < 900) {
    RF = 85;
    LF = 0;
    RB = 0;
    LB = 125;
  }
  else if (currentmillis - timermillis < 950) {
    RF = 0;
    LF = 0;
    RB = 0;
    LB = 0;
  }
      } else {
        timermillis = 0;
        gameAction = "";
      }
    }
    else 
      if (currentmillis - timermillis < 450) {
  RF = 87;
  LF = 85;
  RB = 0;
  LB = 0;
      }else {
        timermillis = 0;
        gameAction = "";
    }
  } else {
    if (currentmillis - timermillis < 450) {
  
  RF = 0;
  LF = 0;
  RB = 87;
  LB = 85;
      }else {
        timermillis = 0;
        gameAction = "";
    }
  }

}
