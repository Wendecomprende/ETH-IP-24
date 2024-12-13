/*
 * 03_Defines.h
 * 
 * Diese Datei dient als zentrale Konfigurationsdatei für das Arduino-Projekt und enthält die Definitionen der Pins, Konstanten sowie die Erstellung von Instanzen für die verwendeten Hardware-Komponenten.
 * 
 * Enthaltene Komponenten:
 * - **I2C-Kommunikation:** Einbindung der Wire-Bibliothek für die I2C-Kommunikation.
 * - **Adafruit Motor Shield:** Einbindung der Adafruit_MotorShield-Bibliothek zur Ansteuerung von DC-Motoren.
 * - **QTR-Sensoren:** Einbindung der QTRSensors-Bibliothek zur Nutzung von Linienverfolgungssensoren.
 * - **Servo:** Einbindung der Servo-Bibliothek zur Steuerung von Servomotoren.
 * 
 * Pin-Definitionen:
 * - Hier werden alle Pins für die Motoren, Servos und QTR-Sensoren festgelegt.
 * - Beispiel:
 *   - `MOTOR_RIGHT`, `MOTOR_LEFT`: Motoren-Pins für das Adafruit Motor Shield.
 *   - `QTR_1` bis `QTR_5`: Pins für die QTR-Liniensensoren.
 *   - `SERVOPIN1`, `SERVOPIN2`: Pins für die Servomotoren.
 * 
 * Konstanten:
 * - Definition der Konstanten für den PI-Regler und die Motorsteuerung.
 * - Beispiel:
 *   - `REGLER_KP`, `REGLER_KI`: Parameter für den PI-Regler.
 *   - `MOTORDEFAULTSPEED`: Standardgeschwindigkeit der Motoren.
 * 
 * Instanz-Erstellungen:
 * - Erstellung von Instanzen für Hardware-Komponenten wie Motoren, Servos und Sensoren.
 * - Beispiel:
 *   - `Adafruit_MotorShield AFMS`: Instanz des Motor Shields.
 *   - `QTRSensors qtr`: Instanz für das QTR-Sensorarray.
 * 
 * Datenstrukturen:
 * - Definition von benutzerdefinierten Datentypen und Strukturen, z.B. für den PI-Regler.
 * - Beispiel:
 *   - `PIController`: Struktur zur Speicherung der PI-Regler-Parameter und -Variablen.
 * 
 * Hinweise:
 * - Änderungen an dieser Datei können Auswirkungen auf den gesamten Code haben und sollten mit Vorsicht vorgenommen werden.
 * - Diese Datei sollte in den Haupt-Sketch eingebunden werden, um die definierten Konstanten und Instanzen nutzen zu können.
 */

#ifndef DEFINES_H
#define DEFINES_H

#include <Wire.h>  // Bibliothek zur I2C-Kommunikation
// Verwendete Version: 2.1 (Die Wire-Bibliothek ist Teil des Arduino Cores und wird in der Regel mit der IDE aktualisiert)
#include <Adafruit_MotorShield.h>  // Bibliothek für das Adafruit Motor Shield
// Verwendete Version: 1.1.3 von Adafruit (Prüfen Sie regelmäßig den Arduino Library Manager für Updates)
#include <QTRSensors.h>  // Bibliothek für die QTR-Sensoren
// Verwendete Version: 4.0.0 von Pololu (Verfügbar im Arduino Library Manager)
#include <Servo.h>  // Bibliothek zur Servo-Ansteuerung
// Verwendete Version: 1.2.2 von Michael Margolis, Arduino (Die Servo-Bibliothek ist weit verbreitet und regelmäßig aktualisiert, siehe Arduino Library Manager)
#include <Adafruit_VL53L0X.h>




// Definition der Konstanten für die Reglerparameter und die Motorgeschwindigkeit
double REGLER_KP = 0.031;//0.025;     // Proportionaler Verstärkungsfaktor für den PI-Regler
double REGLER_KI = 0.020;//0.05;     // Integralverstärkungsfaktor für den PI-Regler
double REGLER_KD = 0.031;//0.03;

double REGLER_KPSLOW = 0.025; //0.035;
double REGLER_KISLOW = 0.0035; //0.0835;
double REGLER_KDSLOW = 0.003;//0.037;

double REGLER_KPBACK = -0.0018;
double REGLER_KDBACK = 0.011;

double REGLER_KPSHOOT = 0.00005;
double REGLER_KDSHOOT = 0.00005;




double motordefaulspeed = 80;    // Standardgeschwindigkeit für die Motoren

// Definition der Pinbelegung für die Motoren auf dem Adafruit Motor Shield
#define MOTOR_RIGHT 1            // Pin für den rechten Motor
#define MOTOR_LEFT 3             // Pin für den linken Motor
#define MOTOR_1 2                // Pin für einen zusätzlichen Motor
#define MOTOR_2 4                // Pin für einen zusätzlichen Motor

// Definition der Pinbelegung für die Servomotoren und Motoren
//#define SERVOPIN1 9              // Pin für den ersten Servomotor
//#define SERVOPIN2 10             // Pin für den zweiten Servomotor
Servo winkel;    
Servo hebel;     
Servo kaugummi;
const int shootPin1 = 3;
const int shootPin2 = 3;
Adafruit_VL53L0X laser1 = Adafruit_VL53L0X(); // drivers left laser
Adafruit_VL53L0X laser2 = Adafruit_VL53L0X(); // drivers right laser
bool initializationFailed = false; 
#define XSHUT_PIN1 1
#define XSHUT_PIN2 2


#define QTR_SENSOR_PIN A0
#define THRESHOLD 500 // Adjust based on your testing


// Definition der Pinbelegung für die QTR-Sensoren zur Linienverfolgung
#define QTR_EMITTER 12           // Pin zur Steuerung des IR-Emitters der QTR-Sensoren
#define QTR_1 4                 // Pin für den ersten QTR-Sensor //// SENSOR PIN 4
#define QTR_2 8                  // Pin für den zweiten QTR-Sensor
#define QTR_3 7                  // Pin für den dritten QTR-Sensor
#define QTR_4 6                  // Pin für den vierten QTR-Sensor
#define QTR_5 5                  // Pin für den fünften QTR-Sensor


// Erstellen einer Instanz des Adafruit Motor Shields zur Ansteuerung der Motoren
Adafruit_MotorShield AFMS = Adafruit_MotorShield();  // Initialisierung des Motor Shields

// Erstellen von Motorobjekten für die Ansteuerung der Motoren über das Motor Shield
Adafruit_DCMotor *motorR = AFMS.getMotor(MOTOR_RIGHT);  // Rechter Motor
Adafruit_DCMotor *motorL = AFMS.getMotor(MOTOR_LEFT);   // Linker Motor

Adafruit_DCMotor *motorU = AFMS.getMotor(MOTOR_1);  // Zusätzlicher Motor 1
Adafruit_DCMotor *motorD = AFMS.getMotor(MOTOR_2);  // Zusätzlicher Motor 2

// Erstellen von Servo-Objekten zur Steuerung von Servomotoren
Servo servo1;  // Erstes Servo-Objekt
Servo servo2;  // Zweites Servo-Objekt

// Erstellen einer Instanz des QTR-Sensorarrays zur Linienverfolgung
QTRSensors qtr;  // Initialisierung des QTR-Sensorarrays
QTRSensors qtrcounter;

// Definieren der Anzahl der QTR-Sensoren und eines Arrays zur Speicherung der Sensordaten
const uint8_t SensorCount = 5;                     // Anzahl der verwendeten QTR-Sensoren
const int maxposition = (SensorCount - 1) * 1000;  // Maximale Positionswert für die Linienerkennung 
                                                   // Wird für fehlerberechnung von PI-Regler verwendet (Wert 1000 von Hersteller vorgegeben)
uint16_t sensorValues[SensorCount];                // Array zur Speicherung der aktuellen Sensordaten
bool sensorStatus[SensorCount];                    // Array zur Speicherung, ob die Sensoren die Linie erkennen

// Definition eines Enums zur Darstellung der verschiedenen Linienstatus des Roboters
enum lineStatus {
  onLine,   // Roboter ist auf der Linie
  stop,     // Roboter soll anhalten (alle Sensoren erkennen Schwarz)
  station,  // Roboter hat eine Station erreicht (Muster: Schwarz-Weiß-Schwarz)
  goal,     // Roboter hat das Ziel erreicht (alle Sensoren erkennen Weiß)
  shoot     // Roboter hat 90 Grad gedreht, soll schiessen
};
enum lineStatus lineStatus = onLine;         // Aktueller Linienstatus des Roboters
enum lineStatus lineOldStatus = lineStatus;  // Vorheriger Linienstatus zum Erkennen von Zustandsänderungen

// Definition der PI-Regler-Parameter und Variablen zur Motorsteuerung
int defaultSpeed = motordefaulspeed;            // Standardgeschwindigkeit der Motoren
int backwardsLimit = defaultSpeed / 2;         // Geschwindigkeitslimit für Rückwärtsbewegung
int highestIntegral = defaultSpeed * 3;          // Maximale Begrenzung der Integralaufsummierung

// Definieren einer Struktur für den PI-Regler, um Reglerparameter und -variablen zu speichern
typedef struct {
  double kp;        // Proportionaler Verstärkungsfaktor
  double ki;        // Integralverstärkungsfaktor
  double kd;
  double target;    // Zielwert (Sollwert)
  double integral;  // Akkumulierter Integralterm
  double derivative;
  double prevError;
  double control;
  double error;
} PIDController;
PIDController pid;  // Instanz des PI-Reglers

typedef struct {
  double kp;        // Proportionaler Verstärkungsfaktor
  double ki;        // Integralverstärkungsfaktor
  double kd;
  double target;    // Zielwert (Sollwert)
  double integral;  // Akkumulierter Integralterm
  double derivative;
  double prevError;
  double control;
  double error;
} PIDControllerSLOW;
PIDControllerSLOW pidSLOW;


typedef struct {
  double kp;        // Proportionaler Verstärkungsfaktor
  double ki;        // Integralverstärkungsfaktor
  double kd;
  double target;    // Zielwert (Sollwert)
  double derivative;
  double prevError;
  double control;
  double error;
} PDControllerBACK;
PDControllerBACK pdBACK;

typedef struct {
  double kp;        // Proportionaler Verstärkungsfaktor
  double ki;        // Integralverstärkungsfaktor
  double kd;
  double target;    // Zielwert (Sollwert)
  double derivative;
  double prevError;
  double control;
  double error;
} PDControllerSHOOT;
PDControllerSHOOT pdSHOOT;

typedef struct {
  double kp;        // Proportionaler Verstärkungsfaktor
  double ki;        // Integralverstärkungsfaktor
  double kd;
  double target;    // Zielwert (Sollwert)
  double derivative;
  double prevError;
  double control;
  double error;
} PDController;
PDController pd;
#endif  // Ende der DEFINES_H-Deklaration