#include <Arduino.h>
#include <Adafruit_MotorShield.h>


volatile unsigned long lastPulseTime = 0;
volatile unsigned long pulseInterval = 0;
volatile double measuredRPM = 0;

#define PULSES_PER_REVOLUTION 6

void encoderISR() {
  unsigned long currentTime = micros();
  pulseInterval = currentTime - lastPulseTime;
  lastPulseTime = currentTime;

  // Avoid division by zero and calculate RPM
  if (pulseInterval > 0) {
    measuredRPM = (60.0 * 1e6) / (pulseInterval * PULSES_PER_REVOLUTION);
  }
}



// Include relevant PID structures and defines from your main code
typedef struct {
  double kp;        // Proportional gain
  double kd;        // Derivative gain
  double ki;
  double target;    // Target RPM
  double derivative;
  double integral;
  double prevError;
  double control;
  double error;
} PIDControllerSHOOT;

// PD controller instance for shooting
PIDControllerSHOOT pidSHOOT;

double REGLER_KPSHOOT = 0.008;//0.056;//0.01895;
double REGLER_KDSHOOT = 0.00; //0.18;
double REGLER_KISHOOT = 0;//0.00;


// Initialize shooting components
void setupShooting() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), encoderISR, RISING);  // Encoder interrupt on pin 2
  pidSHOOT.kp = REGLER_KPSHOOT;
  pidSHOOT.kd = REGLER_KDSHOOT;
  pidSHOOT.ki = REGLER_KISHOOT;
  pidSHOOT.target = 2000;  // Example target RPM
}


// Shooting function
void shooting() {
  while(measuredRPM < 3000){
    unsigned long currentMillis = millis();
    noInterrupts();
    interrupts();
    double rpmCopy = measuredRPM; // Avoid race condition

    // PD control
    pidSHOOT.error = pidSHOOT.target - measuredRPM;
    pidSHOOT.derivative = pidSHOOT.error - pidSHOOT.prevError;
    pidSHOOT.integral += map(pidSHOOT.error, 0, 1000, -100, 100);
    pidSHOOT.integral = constrain(pidSHOOT.integral, -1 * 100, 100);
    pidSHOOT.control = (pidSHOOT.kp * pidSHOOT.error) + (pidSHOOT.kd * pidSHOOT.derivative) + (pidSHOOT.ki * pidSHOOT.integral);
    pidSHOOT.prevError = pidSHOOT.error;
    

    // Apply control signal to motor
    int motorSpeed = 20 + constrain(pidSHOOT.control, 0, 50);
    analogWrite(3, motorSpeed);
    Serial.print("Measured RPM: ");
    Serial.println(rpmCopy);
    Serial.print("calculated CONTROL: ");
    Serial.println(pidSHOOT.control);
    Serial.print("Set speed to:");
    Serial.println(motorSpeed);
    delay(25); // Adjust as needed for display update

    if(pidSHOOT.error < 50){
      analogWrite(3, motorSpeed);
      Serial.print("Finished adjusting, motorSpeed set to: ");
      Serial.println(motorSpeed);
      break;
    }
  }
  
}



////////////////////////////
////////////////////////////

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(1), encoderISR, RISING); // Adjust pin as needed
  setupShooting();
  delay(5000);
}

void loop() {
  shooting();
}














