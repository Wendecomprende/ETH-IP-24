#include <QTRSensors.h>

#define QTR_EMITTER_PIN 12
#define LED_PIN LED_BUILTIN  // Use built-in Arduino LED
const uint8_t SensorCount = 5;
QTRSensors qtr;
uint16_t sensorValues[SensorCount];

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud
  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){4, 8, 7, 6, 5}, SensorCount);  // Set up QTR sensor pins
  qtr.setEmitterPin(QTR_EMITTER_PIN);  // Set the emitter pin

  // Calibrate the sensors
  Serial.println("Calibrating sensors...");
  digitalWrite(LED_PIN, HIGH);  // Turn on LED during calibration
  for (uint16_t i = 0; i < 400; i++) {  // Calibrate for 400 readings (approx 4 seconds)
    qtr.calibrate();
    delay(10);
  }
  digitalWrite(LED_PIN, LOW);  // Turn off LED after calibration
  Serial.println("Calibration complete");
}

void loop() {
  qtr.read(sensorValues);  // Read raw sensor values

  // Print raw sensor values for debugging
  Serial.print("Sensor values: ");
  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  Serial.println();

  int position = qtr.readLineBlack(sensorValues);  // Get the position value from QTR sensors
  Serial.print("Position: ");
  Serial.println(position);  // Print position to the serial monitor
  delay(100);  // Delay for readability (100 ms)
}
