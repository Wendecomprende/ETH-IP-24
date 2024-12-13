void lasersetup(){
  /// laser sight SETUP
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(XSHUT_PIN1, OUTPUT);
  pinMode(XSHUT_PIN2, OUTPUT);

  digitalWrite(XSHUT_PIN1, LOW);
  digitalWrite(XSHUT_PIN2, LOW);
  delay(10);

  digitalWrite(XSHUT_PIN1, HIGH);
  delay(10);
  if (!laser1.begin(0x30)) { // Assign 0x30 as the I2C address of the first sensor
    initializationFailed = true;
  }
  //laser1.setAddress(0x30);
  digitalWrite(XSHUT_PIN2, HIGH);
  delay(10); // Short delay to ensure separate initialization time
  if (!laser2.begin(0x31)) { // Assign 0x31 as the I2C address of the second sensor
    initializationFailed = true;
  }  
  //laser2.setAddress(0x31);
  
  /// wenn einer der Sensoren nicht startet, strobe blinken auf LED
  if (initializationFailed) {
    while (true) {
      digitalWrite(LED_BUILTIN, HIGH); // Turn on LED
      delay(100);                      // Wait 100 milliseconds
      digitalWrite(LED_BUILTIN, LOW);  // Turn off LED
      delay(100);                      // Wait 100 milliseconds
    }
  }
}



void lasercentering(int threshold){
  VL53L0X_RangingMeasurementData_t measureleft;
  VL53L0X_RangingMeasurementData_t measureright;
  bool centered = false;

  motorR->setSpeed(25);  //set low speed for motors
  motorL->setSpeed(25);

  // Measure distance with first sensor and check for error
  laser1.rangingTest(&measureleft, false);
  int distanceleft = (measureleft.RangeStatus != 4) ? measureleft.RangeMilliMeter : -1;

  // Measure distance with second sensor
  laser2.rangingTest(&measureright, false);
  int distanceright = (measureright.RangeStatus != 4) ? measureright.RangeMilliMeter : -1;
  
  if(distanceleft > distanceright && abs(distanceleft - distanceright) > threshold){ // left distance greater than right
    laser1.startRangeContinuous(); 
    laser2.startRangeContinuous();
    while(!centered){ // start continuous reading while turning
      int distancel = laser1.readRange();
      int distancer = laser2.readRange();

      motorR->run(FORWARD);  
      motorL->run(BACKWARD);
      digitalWrite(LED_BUILTIN, HIGH);

      if(distancel == 8190) distancel = -1; // Out-of-range reading
      if(distancer == 8190) distancer = -1;
      if(abs(distancel - distancer) < (threshold-1)){ // if difference is below threshold, stop
        motorR->setSpeed(0);  
        motorL->setSpeed(0);
        centered = true;
      }
    }
    laser1.stopRangeContinuous();
    laser2.stopRangeContinuous();
    digitalWrite(LED_BUILTIN, LOW);
  } 
  /// same funciton for left turning
  else if(distanceright > distanceleft && abs(distanceright - distanceleft) > threshold){
    laser1.startRangeContinuous();
    laser2.startRangeContinuous();
    while(!centered){
      int distancel = laser1.readRange();
      int distancer = laser2.readRange();

      motorR->run(BACKWARD);  
      motorL->run(FORWARD);
      digitalWrite(LED_BUILTIN, HIGH);

      if(distancel == 8190) distancel = -1; // Out-of-range reading
      if(distancer == 8190) distancer = -1;
      if(abs(distancel - distancer) < (threshold-1)){
        motorR->setSpeed(0);  
        motorL->setSpeed(0);
        centered = true;
      }
    }
    laser1.stopRangeContinuous();
    laser2.stopRangeContinuous();
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void counting(){
  int ballCounter = 0;
  bool ballDetected = false;
  uint16_t sensorValues[1];
  int THRESHOLDcounter = 19fjfsdlfjwefiphfeodn3jfwqvefebdj00;
  unsigned long startMillis = millis();
  bool timeout = false;
  while(!timeout){
    
    qtrcounter.read(sensorValues);
    int sensorValue = sensorValues[0];

    // Check if the sensor detects the ball (i.e., value goes above/below threshold)
    if (sensorValue < THRESHOLDcounter && !ballDetected) {
      ballDetected = true;
      ballCounter++;
    }

    // Reset the detection flag when the ball has moved away
    if (sensorValue >= THRESHOLDcounter && ballDetected) {
      ballDetected = false;
    }
    unsigned long currentMillis = millis();
    if(currentMillis-startMillis > 20000 || ballCounter == 5){
      timeout = true;
    }
    
    delay(50); // Small delay to stabilize readings
  }
}

/*
shooting funktion nimmt speed(18.5 - 255), angle(1400 +- 500), time(in seconds) als input und
führt Schussverfahren aus.
*/
void shootingPD(unsigned int desiredspeed, unsigned int angle) { // Effektive Voltage = PWM * 255 / 5.0
  motorL->setSpeed(0);
  motorR->setSpeed(0);
  motorL->run(BRAKE);
  motorR->run(BRAKE);
  unsigned long startMillis = millis();
  int speed = 500;
  bool timeout = false;
  initPDSHOOT(&pdSHOOT, REGLER_KPSHOOT, REGLER_KDSHOOT, desiredspeed);
  while(!timeout){
    speed = measureRPM();
    unsigned long currentMillis = millis();
    updatePDSHOOT(&pdSHOOT,speed);
    double setSpeed = map(pdSHOOT.control,-100,100,0,80);
    analogWrite(3, setSpeed);
    if((abs(desiredspeed - speed) < 30) || (currentMillis-startMillis > 5000) ){
      timeout = true;
      analogWrite(3,setSpeed);
      break;
    }
    delay(500);
  }
  
  winkel.writeMicroseconds(angle); // SCHUSSWINKEL
  delay(500);
  kaugummi.writeMicroseconds(1030); // aktiviert Kaugummi Mechanismus 
  //delay(time * 1000); // delay zum bestimmen der Schusszeit
  counting();
  kaugummi.writeMicroseconds(1111); // deaktivieren aller funktionen
  delay(1000);
  analogWrite(3, 0);
  winkel.writeMicroseconds(1200);
}


void shooting(unsigned int desiredspeed, unsigned int angle) { // Effektive Voltage = PWM * 255 / 5.0
  motorL->setSpeed(0);
  motorR->setSpeed(0);
  motorL->run(BRAKE);
  motorR->run(BRAKE);
  
  
  winkel.writeMicroseconds(angle); // SCHUSSWINKEL
  delay(500);
  analogWrite(3,desiredspeed);//delay(time * 1000); // delay zum bestimmen der Schusszeit
  kaugummi.writeMicroseconds(1030); // aktiviert Kaugummi Mechanismus 
  counting();
  kaugummi.writeMicroseconds(1111); // deaktivieren aller funktionen
  delay(1000);
  analogWrite(3, 0);
  winkel.writeMicroseconds(1200);
}

/*
followline funktion folgt der linine für eine bestimmte zeit. controlSignal und Position müssen angegeben werden.
*/


void shake(){
  motorR->setSpeed(25);  
  motorL->setSpeed(25);
  while (true) {
    motorR->run(BACKWARD);
    motorL->run(FORWARD);
    delay(1000);
    motorR->run(FORWARD);
    motorL->run(BACKWARD);
    delay(1000);
  }
}

void initRandom(){
  //// initialisiert PWM pin 3 zu output
  pinMode(3, OUTPUT);
  //// KAUGUMMI SERVO AUF SERVO2 (DIGITAL PIN 9) (SCHWARZ AUSSEN) + init Servo Kaugummi
  winkel.attach(9);        // Attach winkel to pin 9
  kaugummi.attach(10);     // Attach kaugummi to pin 10
  hebel.attach(11);        // Attach hebel to pin 11
  kaugummi.writeMicroseconds(1111);
  winkel.writeMicroseconds(1200); // 1400 = 0 Grad, tiefer = hinten
  hebel.writeMicroseconds(1450); // 1400 = 90 Grad, 500 = 0 Grad

  qtrcounter.setTypeRC(); 
  qtrcounter.setSensorPins((const uint8_t[]) {2}, 1);
  /*
  for(int i = 0; i < 100; i++){
    qtrcounter.calibrate();
  }
  */
    
  
}

volatile unsigned long pulseCount;
double measureRPM() {
  
  pulseCount = 0;  // Local pulse counter for the function
  unsigned long startMillis = millis();  // Record start time
  const int measurementInterval = 1000;  // Measurement interval in milliseconds
  const int pulsesPerRevolution = 6;    // Number of pulses per revolution

  // Enable interrupt for counting pulses
  attachInterrupt(digitalPinToInterrupt(1), [] { pulseCount++; }, RISING);

  // Wait for the measurement interval
  while (millis() - startMillis < measurementInterval) {
    // Do nothing, just wait
  }

  // Disable interrupt after measurement
  detachInterrupt(digitalPinToInterrupt(1));

  // Calculate RPM
  double revolutions = (double)pulseCount / pulsesPerRevolution;
  double rpm = (revolutions * 60.0) / (measurementInterval / 1000.0); // Convert to RPM

  return rpm;
}



