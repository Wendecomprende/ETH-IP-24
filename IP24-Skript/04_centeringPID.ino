#include "03_Defines.h"
// PID Constants
float Kp = 0.1; // Proportional Gain
float Ki = 0.0; // Integral Gain
float Kd = 0.015; // Derivative Gain

// PID Variables
float error = 0;
float prevError = 0;
float integral = 0;
float derivative = 0;
float output = 0;

// Motor Speed Limits
int MAX_SPEED = 255; // Maximum motor speed (adjust as needed)

// Stopping Threshold
float stopThreshold = 0.1; // Adjust based on sensitivity (e.g., 0.1 to 0.5)

// Sensor Weights for Line Position
int sensorWeights[5] = {3, 1, 0, -1, -3};
uint16_t sensorValuesPID[5]; // Array to hold QTR sensor readings

// Function to Calculate Weighted Error
float calculateError() {
    float weightedSum = 0;
    float sum = 0;
    qtr.readLineBlack(sensorValuesPID);
    for (int i = 0; i < 5; i++) {
        
        //sensorValuesPID[i] = QTR.readSensor(i); // Replace with your sensor reading function
        weightedSum += sensorValuesPID[i] * sensorWeights[i];
        sum += sensorValuesPID[i];
    }

    if (sum == 0) {
        return prevError; // No line detected; continue with the previous error
    }

    return weightedSum / sum; // Weighted average
}

void centeringPID(bool right) {
  if(right){
    motorR->setSpeed(65);
    motorL->setSpeed(60);
    motorR->run(BACKWARD);
    motorL->run(FORWARD);
    delay(800);
    motorR->setSpeed(0);
    motorL->setSpeed(0);
    motorR->run(RELEASE);
    motorL->run(RELEASE);
  }else{
    motorR->setSpeed(60);
    motorL->setSpeed(65);
    motorL->run(BACKWARD);  
    motorR->run(FORWARD);
    delay(800);
    motorR->setSpeed(0);
    motorL->setSpeed(0);
    motorL->run(RELEASE);  
    motorR->run(RELEASE);
  }
  

  while (1) {
    digitalWrite(LED_BUILTIN, HIGH); // Indicate that PID is active

    // Calculate the current error
    error = calculateError();

    // PID Calculations
    integral += error;
    derivative = error - prevError;
    output = Kp * error + Ki * integral + Kd * derivative;

    // Save the current error for the next iteration
    prevError = error;

    // Stopping Condition
    if (abs(error) < stopThreshold) {
        // Line is centered; stop the motors
        motorL->setSpeed(0);
        motorR->setSpeed(0);
        motorL->run(RELEASE); // Stop the left motor
        motorR->run(RELEASE); // Stop the right motor
        digitalWrite(LED_BUILTIN, LOW); // Turn off LED
        break; // Exit the loop
    } else {
        // Adjust Motor Speeds
        int turnSpeed = constrain(output, -255, 255);
        
        if (turnSpeed > 0) {
          motorL->setSpeed((abs(turnSpeed) > 22 ? turnSpeed+2 : 22+2)); // Left motor forward
          motorR->setSpeed((abs(turnSpeed) > 22 ? turnSpeed : 22)); // Right motor backward
          motorL->run(BACKWARD);
          motorR->run(FORWARD);
        } else {
          motorL->setSpeed((abs(turnSpeed) > 22 ? turnSpeed : 22)); // Left motor backward
          motorR->setSpeed((abs(turnSpeed) > 22 ? turnSpeed+2 : 22+2)); // Right motor forward
          motorL->run(FORWARD);
          motorR->run(BACKWARD);
        }
      }
    
  }
}
