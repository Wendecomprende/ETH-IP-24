
#include "03_Defines.h"

#define BLACK_THRESHOLD 555  // Define threshold for black detection
#define WHITE_THRESHOLD 555  // Define threshold for white detection

bool stopFlag;  
int position;
double controlSignal;
double speed = 35;//0.4 * defaultSpeed;


bool allSensorsDetectBlack() {
  int counter = 0;
  for (uint8_t i = 0; i < SensorCount; i++) {
    if (sensorValues[i] > BLACK_THRESHOLD) {  
      counter++;
    }
  }
  return (counter == SensorCount);
}

bool allSensorsDetectWhite() {
  int counter = 0;
  for (uint8_t i = 0; i < SensorCount; i++) {
    if (sensorValues[i] < WHITE_THRESHOLD) {  
      counter++;
    }
  }
  return (counter == SensorCount);
}

void driveBackSENSE() {
  motorR->setSpeed(25);   
  motorL->setSpeed(25);

  motorR->run(BACKWARD);
  motorL->run(BACKWARD);
  bool timeout = false;
  unsigned long startMillis = millis();

  // Keep driving back until all sensors detect black
  while (!timeout) {
    unsigned long currentMillis = millis();
    if(currentMillis - startMillis > 5000){
      motorR->run(BRAKE);
      motorL->run(BRAKE);
      motorR->setSpeed(0);   
      motorL->setSpeed(0);
    }
    delay(25); // Small delay to avoid excessive CPU usage
  }
}

void driveForwSENSE() {
  motorR->run(FORWARD);
  motorL->run(FORWARD);
  motorR->setSpeed(40);   
  motorL->setSpeed(40);
  
  bool timeout = false;
  unsigned long startMillis = millis();

  // Keep driving back until all sensors detect black
  while (!timeout) {
    unsigned long currentMillis = millis();
    if(currentMillis - startMillis > 5000){
      motorR->run(BRAKE);
      motorL->run(BRAKE);
      motorR->setSpeed(0);   
      motorL->setSpeed(0);
    }
    delay(25); // Small delay to avoid excessive CPU usage
  }
}

void alternadriveBack() {
  motorR->setSpeed(40);   
  motorL->setSpeed(40);

  motorR->run(BACKWARD);
  motorL->run(BACKWARD);

  // Keep driving back until all sensors detect black
  while (!allSensorsDetectBlack()) {
    position = qtr.readLineBlack(sensorValues);
    if(position > 3000){
      motorL->setSpeed(0);
      delay(100);
      motorL->setSpeed(25);
    }
    else if (position < 1000){
      motorR->setSpeed(0);
      delay(100);
      motorR->setSpeed(25);
    }
    delay(20); // Small delay to avoid excessive CPU usage
  }
  delay(3000);
  motorR->setSpeed(0);   
  motorL->setSpeed(0);
  motorR->run(RELEASE);
  motorL->run(RELEASE);
}

void followingPIDBACK() {
  stopFlag = false;  

  while (!stopFlag) {
    position = qtr.readLineBlack(sensorValues);
    
    updatePDBACK(&pdBACK, position);
    /*
    if((pi.derivative > 0 && pi.prevError > 0) || (pi.derivative < 0 && pi.prevError < 0)){
      pi.control = -1 * pi.control;
    }
    */
    
    // Calculate and constrain motor speeds
    int leftSpeed = constrain((int)(speed - pdBACK.control), -55, 40);
    int rightSpeed = constrain((int)(speed +  pdBACK.control), -55, 40);

    motorR->setSpeed((abs(rightSpeed) > 28 ? rightSpeed : 28));  
    motorL->setSpeed((abs(leftSpeed) > 28 ? leftSpeed : 28));
    
    motorR->run(BACKWARD);
    motorL->run(BACKWARD);

    // Check if all sensors detect white
    if (allSensorsDetectBlack()) {
      stopFlag = true; 
      motorR->run(BRAKE);
      motorL->run(BRAKE); 
      motorR->setSpeed(0);  
      motorL->setSpeed(0);
      
    }
    delay(10); // Small delay for smoother operation
  }
}


void followingPID() {
  stopFlag = false;  

  while (!stopFlag) {
    position = qtr.readLineBlack(sensorValues);
    updatePIDSLOW(&pidSLOW, position);

    // Calculate and constrain motor speds
    int leftSpeed = constrain((int)(speed + pidSLOW.control), -40, 40);
    int rightSpeed = constrain((int)(speed - pidSLOW.control), -40, 40);

    motorR->setSpeed(rightSpeed);  
    motorL->setSpeed(leftSpeed);
    
    motorR->run(FORWARD);
    motorL->run(FORWARD);

    // Check if all sensors detect white
    if (allSensorsDetectWhite()) {
      stopFlag = true; 
      motorR->run(BRAKE);
      motorL->run(BRAKE); 
      motorR->setSpeed(0);  
      motorL->setSpeed(0);
       
    }
    delay(10); // Small delay for smoother operation
  }
  delay(100);
}


void flocode(int duration) {
  double controlSignal;  // Variable zur Speicherung des Steuersignals für die Motoren.
  int position;          // Variable zur Speicherung der aktuellen Position der Linie.
  int goalCount;         // Zähler für die Verweildauer des Roboters im Zielzustand.

  Backward(35, duration);

  defaultSpeed = 32;
  backwardsLimit = defaultSpeed / 1.5;
  highestIntegral = defaultSpeed * 3;
  delay(200);

  while (1) {
    position = qtr.readLineBlack(sensorValues);
    updatePID(&pid, position);
    setMotorSpeeds(pid.control);
    updateLineStatus();
    if (lineStatus == goal) {
   
      motorR->setSpeed(0);
      motorL->setSpeed(0);
      break;
    }
  }
  defaultSpeed = 65;
  backwardsLimit = defaultSpeed / 1.5;
  highestIntegral = defaultSpeed * 3;
  //DriveBackward(35, duration);
}
