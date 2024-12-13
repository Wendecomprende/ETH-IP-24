/*
 * IP24-Skript.ino
 *
 * Dieses Programm steuert einen linienfolgenden Roboter mit Hilfe eines PI-Reglers. 
 * Der Roboter verwendet QTR-Sensoren (Infrarot Sensor) zur Linienverfolgung und steuert zwei Gleichstrommotoren 
 * über ein Adafruit-Motor-Shield. Der PI-Regler berechnet die notwendigen Anpassungen 
 * der Motorgeschwindigkeit, um den Roboter auf der Linie zu halten. 
 * Es gibt verschiedene Linienstatus (onLine, stop, station, goal), die je nach 
 * Sensorwerten aktualisiert werden und spezifische Aktionen auslösen.
 * 
 * Enthaltene Funktionen:
 * 
 * 1. **setup():**
 *    - Diese Funktion wird einmalig beim Starten des Programms aufgerufen.
 *    - Initialisiert die wichtigsten Module und Systeme des Roboters:
 *      - **initMotorShield():** Initialisiert das Motorschield zur Steuerung der Motoren.
 *      - **initDriveLine():** Initialisiert das System zur Linienverfolgung des Roboters.
 *      - **sensorKalibrieren():** Führt eine Kalibrierung der Sensoren durch, um eine zuverlässige Linienerkennung zu gewährleisten.
 *    - Dieser Abschnitt bietet auch Platz für benutzerdefinierte Setup-Funktionen, die vom Benutzer hinzugefügt werden können.
 * 
 * 2. **loop():**
 *    - Diese Funktion wird kontinuierlich in einer Endlosschleife ausgeführt und bildet das Herzstück des Roboterbetriebs.
 *    - Der Roboter führt folgende Hauptaufgaben aus:
 *      - ***Linienverfolgung:** Liest die aktuelle Position der Linie anhand der Sensoren.
 *      - **PI-Regelung:** Aktualisiert den PI-Regler mit der aktuellen Linienposition und berechnet das Steuersignal.
 *      - **Motorkontrolle:** Passt die Motorgeschwindigkeiten basierend auf dem berechneten Steuersignal an.
 *      - *Linienstatus-Überwachung:** Überwacht kontinuierlich den Status der Linie und reagiert auf Veränderungen:
 *        - **stop:** Hält den Roboter an.
 *        - **station:** Führt eine benutzerdefinierte Aktion bei Erreichen einer Station aus.
 *        - **goal:** Schaltet eine LED ein, wenn das Ziel erreicht ist.
 *        - **default:** Schaltet die LED aus, wenn kein besonderes Ereignis vorliegt.
 *    - Ein Zähler überwacht, wie lange der Roboter im Zielzustand bleibt und stoppt die Motoren nach einer definierten Zeit.
 *    - Der `delay(25)`-Aufruf sorgt für eine kleine Verzögerung, um die Stabilität des Regelkreises zu gewährleisten.
 * 
 * Verwendung:
 * - Dieses Skript sollte auf den Mikrocontroller hochgeladen werden, um den Roboter zu steuern.
 * - Die `setup()`-Funktion dient der Initialisierung, während die `loop()`-Funktion kontinuierlich die Roboterfunktionen ausführt.
 * 
 * Hinweise:
 * - Die Kalibrierung der Sensoren ist entscheidend für eine zuverlässige Linienverfolgung.
 * - Der PI-Regler sollte optimal abgestimmt sein, um eine präzise Steuerung der Motoren zu gewährleisten.
 * - Benutzerdefinierte Aktionen können in den vorgesehenen Abschnitten des Skripts eingefügt werden.
 */

#include "03_Defines.h" // Das File 03_Defines.h muss in jedem .ino File eingebunden werden.



// Die Setup-Funktion wird einmalig beim Start des Programms aufgerufen. 
// Hier werden alle notwendigen Initialisierungen des Roboters durchgeführt.
void setup(){
  /// Mechatronik kit SETUP
  initMotorShield();   // Initialisiert das Motorschield zur Steuerung der Motoren.
  initDriveLine();     // Initialisiert die Linienverfolgungssensoren des Roboters.
  initRandom(); // Initialisiert den ganzen Rest
  delay(500);
  kaugummi.writeMicroseconds(1030);
  delay(800);
  kaugummi.writeMicroseconds(1111);
  delay(800);
  hebel.writeMicroseconds(500);
  delay(800);
  hebel.writeMicroseconds(1450);
  delay(800);
  analogWrite(3, 30);
  delay(800);
  analogWrite(3, 0);
  delay(800);
  sensorKalibrieren(); // Kalibriert die Sensoren durch mehrfache Messungen, um die Genauigkeit zu verbessern.
  delay(4000);
}

// Die Loop-Funktion wird kontinuierlich ausgeführt, solange der Roboter eingeschaltet ist. 
// Sobald das Programm das Ende der Funktion erreicht, beginnt es wieder von vorne.
void loop()
{
  double controlSignal;  // Variable zur Speicherung des Steuersignals für die Motoren.
  int position;          // Variable zur Speicherung der aktuellen Position der Linie.
  int goalCount;         // Zähler für die Verweildauer des Roboters im Zielzustand.
  int adress = 0;        // adress for location

  while (1) {
    // Liest die Position der Linie von den Sensoren:
    // 0   --> Linie befindet sich links vom Roboter.
    // max --> Linie befindet sich rechts vom Roboter, wobei max = (sensorCount - 1) * 1000.
    position = qtr.readLineBlack(sensorValues);  // Ermittelt die aktuelle Position der Linie.

    // Aktualisiert den PI-Regler basierend auf der gemessenen Position und berechnet das Steuersignal.
    updatePID(&pid, position);  // Berechnet das Steuersignal mit dem PI-Regler.
    setMotorSpeeds(pid.control);            // Passt die Motorgeschwindigkeiten basierend auf dem Steuersignal an.

    updateLineStatus();  // Aktualisiert den aktuellen Status der Linie.

    // Führt Aktionen basierend auf dem neuen Linienstatus aus.
    if (lineStatus != lineOldStatus) {  // Überprüft, ob sich der Linienstatus geändert hat.
      switch (lineStatus) {
        case stop:
          
          delay(210); // delay für zentrierten stopp
          motorR->run(BRAKE);
          motorL->run(BRAKE);
          motorR->setSpeed(0);  // Stoppt den rechten Motor.
          motorL->setSpeed(0);  // Stoppt den linken Motor.
          delay(1000);  // delay for smoothness if needed
          adress += 1; 
          
          switch (adress){
            
            case 1:
              hebel.writeMicroseconds(1400); // stellt hebel wieder auf 90 Grad
              turnleft();
              delay(2000);
              followingPIDBACK();
              followingPID();
              followingPIDBACK();
              shooting(24, 690); 
              Forward(55,450);
              turnright();
              motordefaulspeed = 80;
              break;
            case 2:
              turnleft();
              delay(2000);
              followingPIDBACK();
              followingPID();
              followingPIDBACK();
              shooting(24,685);
              Forward(55,450);
              turnright();
              motordefaulspeed = 70;
              break;
            case 3:
              turnright();
              delay(2000);
              followingPIDBACK();
              followingPID();
              followingPIDBACK();
              shooting(30, 1265);
              Forward(55,450);
              turnleft();

              motordefaulspeed = 50;
              break;
            case 4:
              turnright();
              followingPIDBACK();
              followingPID();
              followingPIDBACK();
              shooting(23.2,790);//23.5, 980);
              Forward(55,450);
              turnleft();

              motordefaulspeed = 75;
              break;
            case 5:
              hebel.writeMicroseconds(1400); // stellt hebel wieder auf 90 grad
              turnright();
              followingPIDBACK();
              followingPID();
              followingPIDBACK();
              shooting(21.5, 750);
              Forward(55,450);
              turnleft();
              motordefaulspeed = 80;
              break;
            case 6:
              turnleft();
              followingPIDBACK();
              followingPID();
              followingPIDBACK();
              shooting(23.5,890);//23.9, 950);
              Forward(55,450);
              turnright();
              motordefaulspeed = 70;
              break;
            case 7:
              turnright();
              followingPIDBACK();
              followingPID();
              followingPIDBACK();
              shooting(30, 1150);
              Forward(55,450);
              turnleft();
              motordefaulspeed = 70;
              break;
            case 8:
              turnleft();
              followingPIDBACK();
              followingPID();
              followingPIDBACK();
              shooting(24, 685);
              Forward(55,450);
              turnright();
              motordefaulspeed = 80;
            break; 
            
          }
          //delay(500);         // Wartezeit von 1 Sekunde.
          break;
          
        
        case station:
          /*
          delay(80);
          motorR->run(BRAKE);
          delay(250);
          motorL->run(BRAKE);
          motorL->run(30);
          motorL->run(FORWARD);
          delay(200);
          motorL->run(BRAKE);
          */
          delay(300);
          motorR->run(BRAKE);
          motorL->run(BRAKE);
          motorR->setSpeed(0);  // Stoppt den rechten Motor.
          motorL->setSpeed(0);  // Stoppt den linken Motor.

          
          motorL->run(30);
          motorL->run(FORWARD);
          delay(200);
          motorL->run(BRAKE);
          
          hebel.writeMicroseconds(500); // aktiviert hebel
          delay(3000);
          hebel.writeMicroseconds(1000);
          break;
        
        case goal:
          digitalWrite(LED_BUILTIN, HIGH);  // Schaltet die eingebaute LED ein, wenn das Ziel erreicht ist.
          break;
        default:
          digitalWrite(LED_BUILTIN, LOW);   // Schaltet die eingebaute LED aus, wenn kein besonderes Ereignis vorliegt.
          break;
      }
      lineOldStatus = lineStatus;  // Aktualisiert den alten Linienstatus für den nächsten Vergleich.
    }
                    
    if (lineStatus == goal) {      // Überprüft, ob der Roboter im Zielzustand ist.
      goalCount++;                 // Erhöht den Zielzähler.
      if (goalCount >= 30) {       // Stoppt die Motoren, wenn der Roboter 750ms im Zielzustand verweilt.
        motorR->setSpeed(0);       // Stoppt den rechten Motor.
        motorL->setSpeed(0);       // Stoppt den linken Motor.
      }
    } else {
      goalCount = 0;               // Setzt den Zielzähler zurück, wenn der Roboter nicht im Zielzustand ist.
    }

    delay(25);  // Wartezeit von 25ms für eine stabilere Regelungsschleife.
  }
}


