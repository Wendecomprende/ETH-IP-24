float Q_angle = 0.001;      // Process noise variance for the angle
float Q_bias = 0.003;       // Process noise variance for the gyro bias
float R_measure = 0.03;     // Measurement noise variance for the accelerometer

float angle = 0.0;          // The angle calculated by the Kalman filter - part of the state
float bias = 0.0;           // The gyro bias calculated by the Kalman filter - part of the state
float rate;                 // Unbiased rate calculated from the rate and the calculated bias

float P[2][2] = { { 1, 0 }, { 0, 1 } }; // Error covariance matrix - 2x2 matrix

// Time variable
unsigned long prevTime;
float dt;

void setup() {
  Serial.begin(115200);
  prevTime = millis();
}

void loop() {
  // Time delta calculation
  unsigned long currentTime = millis();
  dt = (currentTime - prevTime) / 1000.0; // Convert ms to seconds
  prevTime = currentTime;

  // 1. Get the gyro rate (angular velocity) and subtract bias
  rate = getGyroRate() - bias;
  angle += dt * rate;  // Predict the new angle

  // 2. Update the error covariance matrix P
  P[0][0] += dt * (dt*P[1][1] - P[0][1] - P[1][0] + Q_angle);
  P[0][1] -= dt * P[1][1];
  P[1][0] -= dt * P[1][1];
  P[1][1] += Q_bias * dt;

  // 3. Compute the Kalman gain
  float S = P[0][0] + R_measure;  // Estimate error
  float K[2];                     // Kalman gain - 2x1 vector
  K[0] = P[0][0] / S;
  K[1] = P[1][0] / S;

  // 4. Calculate the angle difference from the accelerometer (measurement)
  float y = getAccelAngle() - angle;

  // 5. Update the angle and bias with the Kalman gain
  angle += K[0] * y;
  bias += K[1] * y;

  // 6. Update the error covariance matrix P
  float P00_temp = P[0][0];
  float P01_temp = P[0][1];
  P[0][0] -= K[0] * P00_temp;
  P[0][1] -= K[0] * P01_temp;
  P[1][0] -= K[1] * P00_temp;
  P[1][1] -= K[1] * P01_temp;

  // Now, "angle" is the estimated angle after applying the Kalman filter

  // Print the filtered angle
  Serial.print("Filtered Angle: ");
  Serial.println(angle);
}

// Dummy functions for getting gyro rate and accelerometer angle
float getGyroRate() {
  // Return gyro rate from your sensor here
  return 0.0;
}

float getAccelAngle() {
  // Return accelerometer angle from your sensor here
  return 0.0;
}
