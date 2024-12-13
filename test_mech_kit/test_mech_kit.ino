
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>
// Erstellen einer Instanz des Motorschields
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);  
// Create Servo objects für belastungstest
Servo servo1;
Servo servo2;

void setup() {
  // Initialisieren des Motorschields
  AFMS.begin();
  // Anfangsgeschwindigkeit setzen (von 0 bis 255)
  motor1->setSpeed(0);
  motor2->setSpeed(0);
  motor3->setSpeed(0);
  motor4->setSpeed(0);
  // Anfangsrichtung setzen
  motor1->run(FORWARD);  
  motor2->run(FORWARD);
  motor3->run(FORWARD);
  motor4->run(FORWARD);
}

void loop() {
  float desiredVoltage = 6.0;
  float supplyVoltage = 12.0;
  int speed = (desiredVoltage / supplyVoltage) * 255; // Convert to 0-255 scale

  motor4->setSpeed(speed);
  
  
  /*motor4->setSpeed(100);
  motor4->run(FORWARD);
  */
}


