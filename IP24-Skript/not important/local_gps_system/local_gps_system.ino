#include <Wire.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>

// Initialize the sensors
Adafruit_L3GD20_Unified gyro;
Adafruit_LSM303_Accel_Unified accelMag;

// Position and heading variables
float posX = 0.0, posY = 0.0;
float heading = 0.0;

// Anchor point array with angle
struct AnchorPoint {
  float x;
  float y;
  float angle;  // Desired heading angle at the anchor point
};
AnchorPoint anchorPoints[] = { {4, 0, 0}, {3, 0, 0}, {1, 0, 0},  };  // Example anchor points with angles

// Define an Anchor Line
struct AnchorLine {
  float xStart;
  float yStart;
  float xEnd;
  float yEnd;
  float angle;
};
AnchorLine anchorLine = {};  // Anchor line along the X-axis from (0, 0) to (2, 0)


// Complementary filter parameters
float gyroAngle = 0.0;
float accelAngle = 0.0;
float filterAngle = 0.0;
float alpha = 0.98;  // Complementary filter blending factor

// Timing for sampling
unsigned long prevTime = 0;
const float dt = 0.05;  // 10 ms sampling time

void setup() {
  delay(3000);
  Serial.begin(9600);
  Serial.println("start");
  Wire.begin();
  Serial.println("start");
  // Initialize sensors
  if (!accelMag.begin()) {
    Serial.println("Failed to initialize LSM303 accelerometer/magnetometer!");
  }
  delay(3000);
  if (!gyro.begin()) {
    Serial.println("Failed to initialize L3GD20 gyro!"); // GYRO ADRESS IS 69!!!!!!!!!!
   
  }
  

  prevTime = millis();
  Serial.println("finished setup");
}

void loop() {
  // Time tracking for sampling
  unsigned long currentTime = millis();
  if (currentTime - prevTime >= dt * 1000) {
    prevTime = currentTime;

    // Read gyro data
    sensors_event_t gyroEvent;
    gyro.getEvent(&gyroEvent);
    float gyroRate = gyroEvent.gyro.z * (180.0 / PI);  // Get the rotation rate around the Z axis [deg/s]

    // Read accelerometer data
    sensors_event_t accelEvent;
    accelMag.getEvent(&accelEvent);
    accelAngle = atan2(accelEvent.acceleration.y, accelEvent.acceleration.x) * 180 / PI;
    float physicalaccel = sqrt(accelEvent.acceleration.y * accelEvent.acceleration.y + accelEvent.acceleration.x * accelEvent.acceleration.x);
    // Complementary filter for heading
    gyroAngle += gyroRate * dt;  // Integrate gyro rate to get angle
    filterAngle = alpha * (filterAngle + gyroRate * dt) + (1 - alpha) * accelAngle;
    heading = filterAngle;

    // Update position based on heading
    float speed = physicalaccel * dt;  // (0.1) Assume a fixed speed for now (m/s)
    posX += speed * dt * cos(heading * PI / 180);
    posY += speed * dt * sin(heading * PI / 180);
    
    // Check for anchor points with angle correction
    for (int i = 0; i < sizeof(anchorPoints) / sizeof(anchorPoints[0]); i++) {
      if (abs(posX - anchorPoints[i].x) < 0.05 && abs(posY - anchorPoints[i].y) < 0.05) { // if within 5cm of Anchor point
        Serial.print("Reached anchor point: ");
        Serial.println(i);
        
        // Reset position and heading at anchor point
        posX = anchorPoints[i].x;
        posY = anchorPoints[i].y;
        heading = anchorPoints[i].angle;  // Set heading to the predefined angle

        // Update filter angle to match the new heading
        filterAngle = heading;
        gyroAngle = heading;
        
        Serial.print("Corrected position to: ");
        Serial.print(posX);
        Serial.print(", ");
        Serial.println(posY);
        Serial.print("Corrected heading to: ");
        Serial.println(heading);
      }

    }
    
    // Check and correct position along the anchor line
    if (posX >= anchorLine.xStart && posX <= anchorLine.xEnd) {  // Ensure robot is along the X range of the line
      float distanceToLine = abs(posY - anchorLine.yStart);
      if (distanceToLine < 0.05) {  // If within 5 cm of the line, correct the Y position
        posY = anchorLine.yStart;
        Serial.println("Corrected to anchor line Y-position.");
      }
    }

    

    // Debug output
    Serial.print("Position (X, Y): ");
    Serial.print(posX);
    Serial.print(", ");
    Serial.println(posY);
    Serial.print("Heading: ");
    Serial.println(heading);
  }
}
