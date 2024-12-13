void sendPositionAndHeading(float posX, float posY, float heading) {
    Serial.print("X: ");
    Serial.print(posX);
    Serial.print(", Y: ");
    Serial.print(posY);
    Serial.print(", Heading: ");
    Serial.println(heading);
}

void loop() {
    // Update position and heading (use your actual values here)
    float posX = ...;  // Calculated X position
    float posY = ...;  // Calculated Y position
    float heading = ...;  // Calculated heading

    // Send data every second (adjust as needed)
    if (millis() % 1000 == 0) {
        sendPositionAndHeading(posX, posY, heading);
    }

    // Rest of your loop code
}
