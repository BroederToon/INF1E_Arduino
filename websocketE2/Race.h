
//functions
void activate(int LF, int LB, int RF, int RB) {
  analogWrite(5, LF);
  analogWrite(18, LB);
  analogWrite(17, RF);
  analogWrite(16, RB);
}

void driveForward(){
  activate(0, 215, 0, 230);
}

void driveBackwards(){
  activate(200, 0, 200, 0);
}

void turnLeft() {
  activate(190, 0, 0, 190);
}

void turnRight() {
  activate(0, 190, 190, 0);
}

void stopDriving(){
  activate(0, 0, 0, 0);
}

boolean startRace(){
  int valueRight = analogRead(39);
  int valueLeft = analogRead(34);

  Serial.println(valueLeft);

  if(valueRight < 2 && valueLeft < 2){
    Serial.println("we finished");
    stopDriving();
    return true;
  }

  if(valueRight > 140 && valueLeft > 140){
    Serial.println("backwards");
    driveBackwards();
  }else{

    if(valueRight > 140){
      Serial.println("left");
      turnLeft();
    }
  
    if(valueLeft > 140){
      Serial.println("right");
      turnRight();
    }
  
    if(valueLeft < 140 && valueRight < 140){
      Serial.println("forwards");
      driveForward();
    }
  }
  return false;
}
