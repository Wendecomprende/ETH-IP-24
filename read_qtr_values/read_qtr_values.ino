#include <QTRSensors.h>

#define NUM_SENSORS 1
#define THRESHOLDCOUNTER 2000 // Adjust based on your testing

QTRSensors qtrcounter;
int ballCounter = 0;
bool ballDetected = false;
uint16_t sensorValues[1];

void setup() {
  Serial.begin(9600);
  qtrcounter.setTypeRC(); 
  qtrcounter.setSensorPins((const uint8_t[]) {2}, NUM_SENSORS);
}

void loop() {
  qtrcounter.read(sensorValues);
  int sensorValue = sensorValues[0];

  // Write sensor value to the Serial Monitor
  writeSensorValueToSerial(sensorValue);

  // Check if the sensor detects the ball (i.e., value goes above/below threshold)
  if (sensorValue < THRESHOLDCOUNTER && !ballDetected) {
    ballDetected = true;
    ballCounter++;
    Serial.print("Ball Count: ");
    Serial.println(ballCounter);
  }

  // Reset the detection flag when the ball has moved away
  if (sensorValue >= THRESHOLDCOUNTER && ballDetected) {
    ballDetected = false;
  }
  if (ballCounter == 5) {
    Serial.print("YILLLEEEEE");
    Serial.end();
  }
  delay(100); // Small delay to stabilize readings
}

void writeSensorValueToSerial(int sensorValue) {
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
}
