#include <Wire.h>  // Bibliothek zur I2C-Kommunikation
#include <Adafruit_MotorShield.h>  // Bibliothek für das Adafruit Motor Shield
#include <QTRSensors.h>  // Bibliothek für die QTR-Sensoren
#include <Servo.h>  // Bibliothek zur Servo-Ansteuerung
#include <Adafruit_VL53L0X.h>

Servo winkel;  
const int signalPin = 1;         // Pin where the digital signal is connected
volatile unsigned long pulseCount = 0;  // Counter for high pulses
unsigned long previousMillis = 0;       // Timer for 1-second interval
const int pulsesPerRevolution = 6;      // Number of pulses per revolution
float rpm = 0;

void measureRPM() {
  
  pulseCount = 0;  // Local pulse counter for the function
  unsigned long startMillis = millis();  // Record start time
  const int measurementInterval = 500;  // Measurement interval in milliseconds
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

  Serial.println(rpm);
}
void measure() {
  // Calculate RPM every second
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) { // 1-second interval
    detachInterrupt(digitalPinToInterrupt(signalPin)); // Disable interrupts temporarily
    rpm = (pulseCount / pulsesPerRevolution) * 60.0;   // Calculate RPM
    pulseCount = 0;                                    // Reset pulse count
    previousMillis = currentMillis;                    // Reset timer
    attachInterrupt(digitalPinToInterrupt(signalPin), countPulse, RISING); // Re-enable interrupts
    
    // Print RPM to Serial Monitor
    Serial.print("RPM: ");
    Serial.println(rpm);
  }
}

void shooting(unsigned int speed, unsigned int angle, unsigned int time) { 
  analogWrite(3, speed);          // SPEED (18.5-255)
  winkel.writeMicroseconds(angle); // Set shooting angle
  delay(time * 1000);              // Delay to determine shooting time
  analogWrite(3, 0);               // Stop motor
  winkel.writeMicroseconds(1400);  // Reset angle
}

void setup() {
  delay(2000);                   // Startup delay
  pinMode(3, OUTPUT);            // Set pin 3 as output
  winkel.attach(9);              // Attach servo to pin 9
  winkel.writeMicroseconds(1400); // Initialize servo angle
  pinMode(signalPin, INPUT);     // Set signal pin as input
  Serial.begin(9600);            // Start serial communication
  attachInterrupt(digitalPinToInterrupt(signalPin), countPulse, RISING); // Use countPulse as ISR
  Serial.println("Setup finished");
}

void loop() {
  
  analogWrite(3, 22);// Call shooting function
  
  measureRPM();              // Calculate RPM
}

// Interrupt Service Routine (ISR) to count high pulses
void countPulse() {
  pulseCount++;
}
