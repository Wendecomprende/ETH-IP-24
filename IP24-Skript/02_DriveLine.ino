/*
 * 02_DriveLine.ino
 * 
 * Diese Datei enthält die Implementierung der grundlegenden Funktionen zur Steuerung der Linienverfolgung des Roboters. 
 * Sie umfasst die Initialisierung und Kalibrierung der QTR-Sensoren, die Konfiguration des PI-Reglers sowie die Aktualisierung 
 * des Linienstatus basierend auf den Sensordaten.
 * 
 * Enthaltene Funktionen:
 * 
 * 1. **initDriveLine():**
 *    - Initialisiert den PI-Regler und konfiguriert die QTR-Sensoren zur Linienverfolgung.
 *    - Setzt den Modus der QTR-Sensoren auf RC und definiert die verwendeten Pins.
 *    - Verzögerung von 500 ms zur Stabilisierung nach dem Setup.
 * 
 * 2. **sensorKalibrieren():**
 *    - Kalibriert die QTR-Sensoren durch wiederholtes Auslesen ihrer Werte.
 *    - Eine eingebaute LED signalisiert den Kalibrierungsmodus.
 * 
 * 3. **initPI(PIController *pi, double kp, double ki, double target):**
 *    - Initialisiert den PI-Regler mit den übergebenen Parametern für Proportionalität, Integral und Zielwert.
 *    - Setzt den Integralterm auf Null, um den Regler für den Betrieb vorzubereiten.
 * 
 * 4. **updatePI(PIController *pi, double measurement):**
 *    - Berechnet das Steuersignal basierend auf dem Fehler zwischen Soll- und Istwert.
 *    - Aktualisiert den Integralterm und begrenzt ihn, um ein Überschwingen (Windup) zu verhindern.
 *    - Gibt das berechnete Steuersignal zurück.
 * 
 * 5. **updateLineStatus():**
 *    - Aktualisiert den aktuellen Linienstatus des Roboters, indem die Sensorwerte analysiert werden.
 *    - Setzt den Linienstatus abhängig davon, ob und wie viele der Sensoren die Linie erkennen (onLine, stop, station, goal).
 * 
 * Verwendung:
 * - Diese Datei sollte in das Hauptprogramm eingebunden werden, um die Funktionen zur Linienverfolgung zu verwenden.
 * - Die PI-Regelung und Linienverfolgung basieren auf den QTR-Sensorwerten und den definierten Reglerparametern.
 * 
 * Hinweise:
 * - Die Kalibrierung der Sensoren ist entscheidend für die Genauigkeit der Linienverfolgung.
 * - Anpassungen der PI-Parameter (kp, ki) können notwendig sein, um die Regelung an die spezifischen Anforderungen des Roboters anzupassen.
 */

#include "03_Defines.h" // Das File 03_Defines.h muss in jedem .ino File eingebunden werden.

/**
 * @brief Initialisiert die Linienverfolgung des Roboters.
 *
 * Diese Funktion initialisiert den PI-Regler und konfiguriert die QTR-Sensoren für die Linienverfolgung.
 * Der PI-Regler wird mit den vorgegebenen Verstärkungsfaktoren und einem Zielwert basierend auf der
 * mittleren Position der Sensoren initialisiert. Anschließend werden die Sensorpins festgelegt und der
 * Emitterpin für die Infrarotbeleuchtung konfiguriert.
 */
void initDriveLine() {
    initPID(&pid, REGLER_KP, REGLER_KI, REGLER_KD, (double)(maxposition / 2));  // PI-Regler initialisieren
    initPIDSLOW(&pidSLOW, REGLER_KPSLOW, REGLER_KISLOW, REGLER_KDSLOW, (double)(maxposition / 2));
    initPDBACK(&pdBACK, REGLER_KPBACK, REGLER_KDBACK, (double)(maxposition / 2));
    initPD(&pd, REGLER_KP, REGLER_KD, (double)(maxposition / 2));
    qtr.setTypeRC();  // QTR-Sensoren auf RC-Modus setzen
    qtr.setSensorPins((const uint8_t[]){ QTR_1, QTR_2, QTR_3, QTR_4, QTR_5 }, SensorCount);  // Sensorpins definieren
    qtr.setEmitterPin(QTR_EMITTER);  // Emitterpin definieren
    delay(500); // Kurze Verzögerung für Setup-Stabilisierung
}

/**
 * @brief Kalibriert die QTR-Sensoren für die Linienverfolgung.
 *
 * Diese Funktion kalibriert die QTR-Sensoren, indem sie mehrfach ausgelesen werden,
 * um maximale und minimale Werte zu erfassen. Während der Kalibrierung leuchtet die
 * eingebaute LED des Arduino, um den Kalibrierungsmodus anzuzeigen.
 * 
 * Die Kalibrierung sollte folgendermassen Ablaufen:
 * 1. Den Roboter nahe zum Boden bringen und einschalten.
 * 2. Den Rober mehrere (3-4) mal über die Linie hin und her bewegen.
 * 3. Sobald die Orange LED vom Arduino Board erlischt, ist der Vorgang abgeschlossen.
 */
void sensorKalibrieren() {
    pinMode(LED_BUILTIN, OUTPUT);     // Eingebautes LED als Ausgang setzen
    digitalWrite(LED_BUILTIN, HIGH);  // LED einschalten, um den Kalibrierungsmodus anzuzeigen

    for (uint16_t i = 0; i < 200; i++) {
        qtr.calibrate();  // Sensoren kalibrieren
    }
    digitalWrite(LED_BUILTIN, LOW);  // LED ausschalten, um das Ende der Kalibrierung anzuzeigen

}

/**
 * @brief Initialisiert den PI-Regler mit den vorgegebenen Parametern.
 *
 * Diese Funktion setzt die Verstärkungsfaktoren (kp, ki) sowie den Zielwert (target)
 * für den PI-Regler. Zusätzlich wird der Integralterm auf Null gesetzt, um den Regler
 * auf den Betrieb vorzubereiten.
 *
 * @param pid Zeiger auf die PIController-Struktur, die initialisiert werden soll.
 * @param kp Proportionaler Verstärkungsfaktor.
 * @param ki Integralverstärkungsfaktor.
 * @param target Zielwert (Sollwert) des Reglers.
 */
void initPID(PIDController *pid, double kp, double ki, double kd, double target) {
    pid->kp = kp;          // Proportionalen Verstärkungsfaktor setzen
    pid->ki = ki;          // Integralverstärkungsfaktor setzen
    pid->kd = kd;
    pid->target = target;  // Gewünschten Zielwert setzen
    pid->integral = 0;     // Integralterm auf Null setzen
    pid->derivative = 0;
    pid->prevError = 0;
    pid->control = 0;
    pid->error = 0;
}

void initPIDSLOW(PIDControllerSLOW *pidSLOW, double kp, double ki, double kd, double target) {
    pidSLOW->kp = kp;          // Proportionalen Verstärkungsfaktor setzen
    pidSLOW->ki = ki;          // Integralverstärkungsfaktor setzen
    pidSLOW->kd = kd;
    pidSLOW->target = target;  // Gewünschten Zielwert setzen
    pidSLOW->integral = 0;     // Integralterm auf Null setzen
    pidSLOW->derivative = 0;
    pidSLOW->prevError = 0;
    pidSLOW->control = 0;
    pidSLOW->error = 0;
}

void initPDBACK(PDControllerBACK *pdBACK, double kp, double kd, double target) {
    pdBACK->kp = kp;          // Proportionalen Verstärkungsfaktor setzen
    pdBACK->kd = kd;
    pdBACK->target = target;  // Gewünschten Zielwert setzen
    pdBACK->derivative = 0;
    pdBACK->prevError = 0;
    pdBACK->control = 0;
    pdBACK->error = 0;
}

void initPDSHOOT(PDControllerSHOOT *pdSHOOT, double kp, double kd, double target) {
    pdSHOOT->kp = kp;          // Proportionalen Verstärkungsfaktor setzen
    pdSHOOT->kd = kd;
    pdSHOOT->target = target;  // Gewünschten Zielwert setzen
    pdSHOOT->derivative = 0;
    pdSHOOT->prevError = 0;
    pdSHOOT->control = 0;
    pdSHOOT->error = 0;
}

void initPD(PDController *pd, double kp, double kd, double target) {
    pd->kp = kp;          // Proportionalen Verstärkungsfaktor setzen
    pd->kd = kd;
    pd->target = target;  // Gewünschten Zielwert setzen
    pd->derivative = 0;
    pd->prevError = 0;
    pd->control = 0;
    pd->error = 0;
}

/**
 * @brief Aktualisiert den PI-Regler und berechnet das Steuersignal.
 *
 * Diese Funktion berechnet das Steuersignal basierend auf dem Fehler zwischen dem
 * aktuellen Messwert (measurement) und dem Zielwert (target). Der Fehler wird
 * proportional (durch kp) und über die Zeit (durch ki) aufintegriert, um eine
 * möglichst genaue Regelung zu gewährleisten.
 *
 * @param pid Zeiger auf die PIController-Struktur, die den Regler enthält.
 * @param measurement Der aktuelle Messwert, der mit dem Zielwert verglichen wird.
 * @return Das berechnete Steuersignal, das an die Motoren weitergegeben wird.
 */
void updatePID(PIDController *pid, double measurement) {
    double error = pid->target - measurement;  // Fehler berechnen als Differenz zwischen Sollwert und aktuellem Wert
    
    // Fehler skalieren und Integralterm aufsummieren
    pid->integral += map(error, -1 * (maxposition / 2), maxposition / 2, -255, 255) / 4;
    
    // Integral begrenzen, um Windup zu verhindern
    pid->integral = constrain(pid->integral, -1 * highestIntegral, highestIntegral);

    pid->derivative = error - pid->prevError;

    pid->prevError = error;

    // Berechnung des Steuersignals als Summe von Proportional- und Integralterm
    pid->control = pid->kp * error + pid->ki * pid->integral + pid->kd * pid->derivative;
}

void updatePIDSLOW(PIDControllerSLOW *pidSLOW, double measurement) {
    double error = pidSLOW->target - measurement;  // Fehler berechnen als Differenz zwischen Sollwert und aktuellem Wert
    
    // Fehler skalieren und Integralterm aufsummieren
    pidSLOW->integral += map(error, -1 * (maxposition / 2), maxposition / 2, -255, 255) / 4;
    
    // Integral begrenzen, um Windup zu verhindern
    pidSLOW->integral = constrain(pidSLOW->integral, -1 * highestIntegral, highestIntegral);

    pidSLOW->derivative = error - pidSLOW->prevError;

    pidSLOW->prevError = error;

    // Berechnung des Steuersignals als Summe von Proportional- und Integralterm
    pidSLOW->control = pidSLOW->kp * error + pidSLOW->ki * pidSLOW->integral + pidSLOW->kd * pidSLOW->derivative;
}

void updatePDBACK(PDControllerBACK *pdBACK, double measurement) {
    double error = pdBACK->target - measurement;  // Fehler berechnen als Differenz zwischen Sollwert und aktuellem Wert
    
    pdBACK->derivative = error - pdBACK->prevError;

    pdBACK->prevError = error;

    // Berechnung des Steuersignals als Summe von Proportional- und Integralterm
    pdBACK->control = pdBACK->kp * error + pdBACK->kd * pdBACK->derivative;
}

void updatePDSHOOT(PDControllerSHOOT *pdSHOOT, double measurement) {
    double error = pdSHOOT->target - measurement;  // Fehler berechnen als Differenz zwischen Sollwert und aktuellem Wert
    
    pdSHOOT->derivative = error - pdSHOOT->prevError;

    pdSHOOT->prevError = error;

    // Berechnung des Steuersignals als Summe von Proportional- und Integralterm
    pdSHOOT->control = pdSHOOT->kp * error + pdSHOOT->kd * pdSHOOT->derivative;
}

void updatePD(PDController *pd, double measurement) {
    double error = pd->target - measurement;  // Fehler berechnen als Differenz zwischen Sollwert und aktuellem Wert

    pd->derivative = error - pd->prevError;

    pd->prevError = error;

    // Berechnung des Steuersignals als Summe von Proportional- und Integralterm
    pd->control = pd->kp * error + pd->kd * pd->derivative;
}



/**
 * @brief Aktualisiert den Linienstatus basierend auf den Sensorwerten.
 *
 * Diese Funktion liest die aktuellen Werte der QTR-Sensoren aus und bestimmt basierend
 * auf diesen Werten den aktuellen Linienstatus des Roboters (onLine, stop, station, goal).
 * Der Status wird durch die Erkennung von bestimmten Mustern auf der Linie (oder deren Fehlen)
 * durch die Sensoren bestimmt.
 */
void updateLineStatus() {
    for (uint8_t i = 0; i < SensorCount; i++) {
        if (sensorValues[i] > 555) {  // Schwellenwert für den QTR-Sensor, um zu bestimmen, ob die Linie erkannt wird
            sensorStatus[i] = 1;  // Sensor erkennt die Linie
        } else {
            sensorStatus[i] = 0;  // Sensor erkennt die Linie nicht
        }
    }
    
    // Bestimmen des aktuellen Linienstatus basierend auf den Sensorwerten
    if (sensorStatus[0] == 1 && sensorStatus[4] == 1 && (sensorStatus[1] == 0 || sensorStatus[2] == 0 || sensorStatus[3] == 0)) {
        lineStatus = station;  // Muster: Schwarz-Weiss-Weiss-Weiss-Schwarz (Station)
    } else if (sensorStatus[0] == 1 && sensorStatus[1] == 1 && sensorStatus[2] == 1 && sensorStatus[3] == 1 && sensorStatus[4] == 1) {
        lineStatus = stop;  // Muster: Schwarz-Schwarz-Schwarz-Schwarz-Schwarz (Stop)
    } else if (sensorStatus[0] == 0 && sensorStatus[1] == 0 && sensorStatus[2] == 0 && sensorStatus[3] == 0 && sensorStatus[4] == 0) {
        lineStatus = goal;  // Muster: Weiss-Weiss-Weiss-Weiss-Weiss (Ziel)
    }  else if (sensorStatus[0] == 0 && sensorStatus[1] == 1 && sensorStatus[2] == 1 && sensorStatus[3] == 1 && sensorStatus[4] == 0){
        lineStatus = shoot; // Muster: Weiss-Schwarz-Schwarz-Schwarz-Weiss
    } else {
        lineStatus = onLine;  // Muster: nahezu Weiss-Weiss-Schwarz-Weiss-Weiss (Linie-folgen)
    }
}



void Forward(int x, int y){
    motorR->setSpeed(x);
    motorL->setSpeed(x);
    motorR->run(FORWARD);
    motorL->run(FORWARD);
    delay(y); 
    motorR->setSpeed(0);
    motorL->setSpeed(0);
    motorR->run(RELEASE);
    motorL->run(RELEASE);
    
}
void Backward(int x, int y){
    motorR->setSpeed(x);
    motorL->setSpeed(x);
    motorR->run(BACKWARD);
    motorL->run(BACKWARD);
    delay(y); 
    motorR->setSpeed(0);
    motorL->setSpeed(0);
    motorR->run(RELEASE);
    motorL->run(RELEASE);
}


