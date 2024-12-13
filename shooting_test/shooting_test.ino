const int pwmPin1 = 3;
const int pwmPin2 = 3;
float desiredVoltage = 0.8; // For example, outputting 3V
float maxVoltage = 5.0;

void shooting(int desiredVoltage) {
  int pwmValue =(desiredVoltage / 5.0) * 255;
  analogWrite(pwmPin1, pwmValue);
  analogWrite(pwmPin2, pwmValue);
}




void setup() {
  pinMode(pwmPin1, OUTPUT);
  pinMode(pwmPin2, OUTPUT);
}

void loop() {
  int pwmValue =(desiredVoltage / maxVoltage) * 255;
  analogWrite(pwmPin1, pwmValue);
  analogWrite(pwmPin2, pwmValue);
}

