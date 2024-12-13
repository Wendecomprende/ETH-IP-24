#include <Servo.h>

Servo winkel;    // Declare the servo
Servo hebel;     // Declare the second servo
Servo kaugummi;  // Declare the third servo

void setup() {
    winkel.attach(9);        // Attach winkel to pin 9
    kaugummi.attach(10);     // Attach kaugummi to pin 10
    hebel.attach(11);        // Attach hebel to pin 11
    Serial.begin(9600);      // Start serial communication

    // Initialize servos to specific positions
    kaugummi.writeMicroseconds(1111);
    //winkel.write(90);         // Move winkel to 0 degrees
    //hebel.write(90);          // Move hebel to 0 degrees

    // Set additional positions after setup
    winkel.writeMicroseconds(1400);        // Move winkel to 90 degrees
    hebel.writeMicroseconds(1400);         // Move hebel to 90 degrees
}

void loop() {
    
    hebel.writeMicroseconds(500);
    winkel.writeMicroseconds(500);
    delay(5000);
    hebel.writeMicroseconds(1400);
    winkel.writeMicroseconds(1400);
    
    /*
           // Wait for 10 seconds
    for (int i = 500; i < 2000; i += 100) {
        //hebel.writeMicroseconds(i);
        winkel.writeMicroseconds(i); // Write to kaugummi servo
        delay(500);                   // Wait for half a second
        Serial.println(i);            // Print the current pulse width to Serial Monitor
    }
  */
    

    
}
