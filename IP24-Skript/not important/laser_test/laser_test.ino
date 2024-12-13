#include <Wire.h>
#include <Adafruit_VL53L0X.h>

// Define objects for each sensor
Adafruit_VL53L0X sensor1 = Adafruit_VL53L0X();
Adafruit_VL53L0X sensor2 = Adafruit_VL53L0X();

// Define the XSHUT pins for each sensor
#define XSHUT_PIN1 1
#define XSHUT_PIN2 2

void setup() {
  Serial.begin(9600);
  Serial.println("setupstarting");
  // Initialize XSHUT pins
  pinMode(XSHUT_PIN1, OUTPUT);
  pinMode(XSHUT_PIN2, OUTPUT);

  // Ensure both sensors are off initially
  digitalWrite(XSHUT_PIN1, LOW);
  digitalWrite(XSHUT_PIN2, LOW);
  delay(10);

  // Initialize the first sensor
  digitalWrite(XSHUT_PIN1, HIGH);
  delay(10); // Allow time for sensor to initialize
  if (!sensor1.begin(0x30)) { // Set the I2C address to 0x30 for sensor 1
    Serial.println("Failed to initialize sensor 1");
    while (1);
  }
  sensor1.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_SPEED);
  
  // Initialize the second sensor
  digitalWrite(XSHUT_PIN2, HIGH);
  delay(10); // Allow time for sensor to initialize
  if (!sensor2.begin(0x31)) { // Set the I2C address to 0x31 for sensor 2
    Serial.println("Failed to initialize sensor 2");
    while (1);
  }
  sensor2.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_SPEED);
  
  Serial.println("Sensors initialized successfully");
}

void loop() {
  // Example code to read from sensors
  VL53L0X_RangingMeasurementData_t measure1, measure2;

  sensor1.rangingTest(&measure1, false);
  if (measure1.RangeStatus != 4) {
    Serial.print("Sensor 1: ");
    Serial.print(measure1.RangeMilliMeter);
    Serial.println(" mm");
  }

  sensor2.rangingTest(&measure2, false);
  if (measure2.RangeStatus != 4) {
    Serial.print("Sensor 2: ");
    Serial.print(measure2.RangeMilliMeter);
    Serial.println(" mm");
  }

  delay(500);
}
