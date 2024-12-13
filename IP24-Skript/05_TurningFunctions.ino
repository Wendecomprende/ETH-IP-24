/*
Tunleft funktion dreht bis status "shoot" erreicht ist. (weiss-schwaz-schwarz-schwarz-weiss)
*/
void turnleft(){

  motorR->setSpeed(40);  //motoren für links/rechts eingstellen
  motorL->setSpeed(40);

  motorR->run(FORWARD);  // Anfangsrichtung für den motor links/rechts einstellen
  motorL->run(BACKWARD);
  delay(750);
  bool conline = false;
  while(conline == false) {
    int position = qtr.readLineBlack(sensorValues);
    
    if(abs(2000-position) < 250) {
      conline = true;
      motorR->run(BRAKE);
      motorL->run(BRAKE);
      //delay(78);
      motorR->setSpeed(0);
      motorL->setSpeed(0);
    }
  }
}

/*
Tunright funktion dreht bis status "shoot" erreicht ist. (weiss-schwaz-schwarz-schwarz-weiss)
*/
void turnright(){

  motorR->setSpeed(40);  //motoren für links/rechts eingstellen
  motorL->setSpeed(40);

  motorR->run(BACKWARD);  // Anfangsrichtung für den motor links/rechts einstellen
  motorL->run(FORWARD);
  delay(750);
  bool conline = false;
  while(conline == false) {
    int position = qtr.readLineBlack(sensorValues);
    
    if(abs(2000-position) < 250) {
      conline = true;
      motorR->run(BRAKE);
      motorL->run(BRAKE);
      //delay(78);
      motorR->setSpeed(0);
      motorL->setSpeed(0);
    }
  }
}

/*
turntime funktion dreht sich eine gewisse zeit in angegebene Richtung
*/
void turntime(int time, bool right){///IRGENDWO DELAY EINBAUEN, DASS SABRINA ZENTRIERT

  motorR->setSpeed(50);  //motoren für links/rechts eingstellen
  motorL->setSpeed(50);
  if(right){
    motorR->run(BACKWARD);  // Anfangsrichtung für den motor links/rechts einstellen
    motorL->run(FORWARD);
  }else{
    motorR->run(FORWARD);  // Anfangsrichtung für den motor links/rechts einstellen
    motorL->run(BACKWARD);
  }
  
  delay(time);
  
  motorR->setSpeed(0);  //motoren für links/rechts eingstellen
  motorL->setSpeed(0);
}

void standstill(){
  motorR->run(BRAKE);
  motorL->run(BRAKE);
  motorL->setSpeed(0);
  motorR->setSpeed(0);
}

void turn_90Right(){
  motorL->setSpeed(90);
  motorL->run(FORWARD);
  
  motorR->setSpeed(90);
  motorR->run(BACKWARD);
  
  delay (400);
  qtr.read(sensorValues);

  while(sensorValues[2] < 140 && sensorValues[3] < 110){ //Falls nicht besser durch diese Zeile ersetzen: while(sensorValues[2] < 150 && sensorValues[1] < 115){
    qtr.read(sensorValues);
    if (sensorValues[3] > 90){
      motorL->setSpeed(50);
      motorR->setSpeed(50);
    }
  }
  standstill();
}

void turn_90Left(){
  motorR->setSpeed(90);
  motorR->run(FORWARD);
  
  motorL->setSpeed(90);
  motorL->run(BACKWARD);
  
  delay (400);
  qtr.read(sensorValues);

  while(sensorValues[2] < 140 && sensorValues[1] < 110){ //Für den Moment so stehen lassen. 
    qtr.read(sensorValues);
    if (sensorValues[1] > 90){
      motorL->setSpeed(50);
      motorR->setSpeed(50);
    }
  }
  standstill();
}


void positionTurnRight(){
  motorR->setSpeed(40); 
  motorL->setSpeed(40);

  motorR->run(BACKWARD);  
  motorL->run(FORWARD);
  delay(400);

  int position = qtr.readLineBlack(sensorValues);
  updatePD(&pd, position);
  while(pd.prevError > 1800){
    int position = qtr.readLineBlack(sensorValues);
    updatePD(&pd, position);
    delay(25);
  } 
  bool stopFlag = false;  
  while(!stopFlag) {
    int position = qtr.readLineBlack(sensorValues);
    updatePD(&pd, position);

    int leftSpeed = constrain((int)(40 + pd.control), -255, 255);
    
    motorR->setSpeed(leftSpeed); 
    motorL->setSpeed(leftSpeed);
    motorR->run(BACKWARD);  
    motorL->run(FORWARD);
    
    delay(25);
    if(abs(2000-position) < 20){
      stopFlag = true;
    }
  }
  standstill();
}

void positionTurnLeft(){
  motorR->setSpeed(40); 
  motorL->setSpeed(40);

  motorR->run(FORWARD);  // Anfangsrichtung für den motor links/rechts einstellen
  motorL->run(BACKWARD);
  delay(400);

  int position = qtr.readLineBlack(sensorValues);
  updatePD(&pd, position);
  while(abs(2000-position) > 1800){
    int position = qtr.readLineBlack(sensorValues);
    updatePD(&pd, position);
    delay(25);
  } 
  bool stopFlag = false;  
  while(!stopFlag) {
    int position = qtr.readLineBlack(sensorValues);
    updatePD(&pd, position);

    int leftSpeed = constrain((int)(40 + pd.control), -255, 255);
    
    motorR->setSpeed(leftSpeed); 
    motorL->setSpeed(leftSpeed);
    motorR->run(BACKWARD);  
    motorL->run(FORWARD);
    
    delay(25);
    if(abs(2000 - position) < 20){
      stopFlag = true;
    }
  }
  standstill();
}