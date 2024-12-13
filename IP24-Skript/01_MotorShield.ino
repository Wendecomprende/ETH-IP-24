/*
 * 01_MotorShield.ino
 * 
 * Diese Datei enthält die Implementierung der grundlegenden Funktionen zur Steuerung der Motoren und Servos des Roboters.
 * Sie umfasst die Initialisierung des Motorschields, das Setzen der Motorgeschwindigkeiten basierend auf einem Steuersignal
 * sowie die Konfiguration und Steuerung der Servomotoren.
 * 
 * Enthaltene Funktionen:
 * 
 * 1. **initMotorShield():**
 *    - Initialisiert das Motor Shield und stellt die anfänglichen Einstellungen für die Motoren und Servos ein.
 *    - Setzt die anfängliche Geschwindigkeit der Motoren auf Null und richtet die Motoren auf Vorwärtslauf aus.
 *    - Verbindet die Servomotoren mit den definierten Pins.
 * 
 * 2. **setMotorSpeeds(double signal):**
 *    - Berechnet und setzt die Geschwindigkeiten der Motoren basierend auf dem übergebenen Steuersignal.
 *    - Die Geschwindigkeiten werden begrenzt und die Richtung der Motoren wird entsprechend angepasst.
 *    - Rückwärtsbewegungen werden speziell behandelt, um eine reduzierte Geschwindigkeit und positive Werte zu gewährleisten.
 * 
 * 3. **Servo-Funktionen:**
 *    - **servo.attach(uint8_t pin):** Verbindet den Servomotor mit dem angegebenen Pin. Der Pin muss ein PWM-fähiger Pin auf dem Mikrocontroller sein.
 *    - **servo.write(int angle):** Setzt den Winkel des Servomotors auf den angegebenen Wert. Der Winkelbereich liegt typischerweise zwischen 0° und 180°.
 *    - **servo.writeMicroseconds(int us):** Setzt die Pulsbreite für den Servomotor auf den angegebenen Wert in Mikrosekunden. Die Pulsbreite beeinflusst die genaue Position des Servos.
 *    - **servo.read():** Gibt den aktuellen Winkel des Servomotors zurück. Dies ist hilfreich, um die aktuelle Position des Servos abzufragen.
 *    - **servo.detach():** Trennt den Servomotor vom Steuerpin, wodurch der Pin für andere Funktionen verwendet werden kann.
 *
 * Verwendung:
 * - Diese Datei sollte in das Hauptprogramm eingebunden werden, um die Motorsteuerung und die Servomotoren zu konfigurieren.
 * - Die Funktion `setMotorSpeeds()` wird verwendet, um die Motoren basierend auf dem aktuellen Steuersignal anzupassen.
 * - Die Servo-Funktionen (`attach()`, `write()`, `writeMicroseconds()`, `read()`, `detach()`) ermöglichen die Steuerung der Servomotoren (`servo1` und `servo2`).
 * 
 * Hinweise:
 * - Die Anfangsgeschwindigkeiten der Motoren sind auf Null gesetzt, um sicherzustellen, dass die Motoren beim Start nicht unerwartet laufen.
 * - Die Geschwindigkeiten werden begrenzt, um Schäden an den Motoren oder dem Robotersystem zu verhindern.
 * - Die Servomotoren müssen korrekt an die definierten Pins (`SERVOPIN1` und `SERVOPIN2`) angeschlossen werden.
 * - Die Servomotoren sollten im unterstützten Winkelbereich bewegt werden, um eine korrekte Funktion zu gewährleisten.
 */

#include "03_Defines.h" // Das File 03_Defines.h muss in jedem .ino File eingebunden werden.

/**
 * @brief Initialisiert das Motor Shield und die Servomotoren.
 *
 * Diese Funktion initialisiert das Motor Shield und setzt die anfänglichen Einstellungen für die Motoren und Servos.
 * Die Motoren werden auf eine Geschwindigkeit von Null gesetzt und auf Vorwärtslauf ausgerichtet. 
 * Die Servomotoren werden mit den definierten Pins verbunden.
 */
void initMotorShield() {
  AFMS.begin();          // Motor Shield initialisieren
  motorR->setSpeed(0);   // Anfangsgeschwindigkeit für den rechten Motor auf Null setzen
  motorL->setSpeed(0);   // Anfangsgeschwindigkeit für den linken Motor auf Null setzen
  motorR->run(FORWARD);// Anfangsrichtung für den rechten Motor auf Vorwärtslauf setzen
  motorL->run(FORWARD);  // Anfangsry yyyyyYYYYXXYYXYYXYichtung für den linken Motor auf Vorwärtslauf setzen

  // Servomotoren an die definierten Pins anschließen
  //servo1.attach(SERVOPIN1, 350, 2490);
  //servo2.attach(SERVOPIN2, 350, 2510);
}

/**
 * @brief Setzt die Geschwindigkeiten der Motoren basierend auf dem Steuersignal.
 *
 * Diese Funktion berechnet die Geschwindigkeit für den linken und rechten Motor basierend auf dem übergebenen
 * Steuersignal. Die Geschwindigkeiten werden begrenzt und die Richtung der Motoren wird angepasst.
 * Rückwärtsbewegungen werden speziell behandelt, um die Geschwindigkeit anzupassen und positive Werte zu gewährleisten.
 *
 * @param signal Das Steuersignal, das zur Berechnung der Motorengeschwindigkeiten verwendet wird.
 */
void setMotorSpeeds(double signal) {
  // Linke Motorgeschwindigkeit berechnen und begrenzen
  int leftSpeed = constrain((int)(defaultSpeed + signal), -255, 255);
  // Rechte Motorgeschwindigkeit berechnen und begrenzen
  int rightSpeed = constrain((int)(defaultSpeed - signal), -255, 255);
  
  // Motorrichtung und -geschwindigkeit für den linken Motor anpassen
  if (leftSpeed < backwardsLimit) {
    motorL->run(BACKWARD);       // Linken Motor auf Rückwärtslauf setzen
    leftSpeed *= 0.6;            // Geschwindigkeit für Rückwärtsbewegung anpassen
    leftSpeed = abs(leftSpeed);  // Positive Geschwindigkeitswerte sicherstellen
  } else {
    motorL->run(FORWARD);  // Linken Motor auf Vorwärtslauf setzen
  }

  // Motorrichtung und -geschwindigkeit für den rechten Motor anpassen
  if (rightSpeed < backwardsLimit) {
    motorR->run(BACKWARD);         // Rechten Motor auf Rückwärtslauf setzen
    rightSpeed *= 0.6;             // Geschwindigkeit für Rückwärtsbewegung anpassen
    rightSpeed = abs(rightSpeed);  // Positive Geschwindigkeitswerte sicherstellen
  } else {
    motorR->run(FORWARD);  // Rechten Motor auf Vorwärtslauf setzen
  }
  
  // Geschwindigkeiten der Motoren setzen
  
  motorR->setSpeed(rightSpeed);  // Rechte Motorgeschwindigkeit setzen
  motorL->setSpeed(leftSpeed);   // Linke Motorgeschwindigkeit setzen
  /*
  motorL->run(BACKWARD);
  motorR->run(BACKWARD);
  */
}

