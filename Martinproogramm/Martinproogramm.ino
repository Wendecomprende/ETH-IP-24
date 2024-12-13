#include <Wire.h>  // Bibliothek zur I2C-Kommunikation
// Verwendete Version: 2.1 (Die Wire-Bibliothek ist Teil des Arduino Cores und wird in der Regel mit der IDE aktualisiert)
#include <Adafruit_MotorShield.h>  // Bibliothek für das Adafruit Motor Shield
// Verwendete Version: 1.1.3 von Adafruit (Prüfen Sie regelmäßig den Arduino Library Manager für Updates)
#include <QTRSensors.h>  // Bibliothek für die QTR-Sensoren
// Verwendete Version: 4.0.0 von Pololu (Verfügbar im Arduino Library Manager)
#include <Servo.h>  // Bibliothek zur Servo-Ansteuerung

Servo winkel;  
Servo kaugummi;

void setup() {
  delay(2000);
  pinMode(3, OUTPUT);
  winkel.attach(9);
  winkel.writeMicroseconds(1400); 
  kaugummi.attach(10);
  kaugummi.writeMicroseconds(1111); 
  Serial.begin(9600);
  Serial.println("setup finished");
  Serial.println("enter throttle and angle divided by a Komma");
}

void loop() {
  kaugummi.writeMicroseconds(1030);
  if (Serial.available() > 0) {
    String input1 = Serial.readStringUntil(',');
    float value1 = input1.toFloat();
    
    analogWrite(3, value1);
    
    String input2 = Serial.readStringUntil('\n');
    float value2 = input2.toFloat();
    Serial.println(input1 );
    Serial.println(input2 );
    winkel.writeMicroseconds(value2); 
    if(value1 == 0){
      kaugummi.writeMicroseconds(1111);
    }
  }
}