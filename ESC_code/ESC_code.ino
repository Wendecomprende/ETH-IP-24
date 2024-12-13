#include <Servo.h>

Servo esc;  // Erstellen eines Servo-Objekts

void setup() {
  esc.attach(10);  // ESC an Pin 10 anschließen

  // Serielle Kommunikation startenpor
  Serial.begin(9600);
  Serial.println("Initialisierung des ESCs...");

  // Initialisieren: Minimalwert (1000 µs) für 2 Sekunden
  esc.writeMicroseconds(1000);
  delay(2000);

  Serial.println("ESC bereit. Geben Sie einen Wert zwischen 0 und 100 ein:");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    int value = input.toInt();

    if (value >= 0 && value <= 100) {
      // Wert von 0-100 in den Bereich 1000-2000 µs umwandeln
      int pwmValue = map(value, 0, 100, 1000, 2000);
      esc.writeMicroseconds(pwmValue);

      Serial.print("PWM Signal: ");
      Serial.println(pwmValue);
    } else {
      Serial.println("Bitte geben Sie einen Wert zwischen 0 und 100 ein.");
    }
  }
}