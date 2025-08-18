const int stepX = 2;
const int dirX  = 5;

const int stepY = 3;
const int dirY  = 6;

const int stepZ = 4;
const int dirZ  = 7;

const int enPin = 8;

void setup() {
  pinMode(stepX, OUTPUT);
  pinMode(dirX, OUTPUT);

  pinMode(stepY, OUTPUT);
  pinMode(dirY, OUTPUT);

  pinMode(stepZ, OUTPUT);
  pinMode(dirZ, OUTPUT);

  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW); // Enable motors

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    char motorID = Serial.read();
    delay(10);

    if (Serial.available() > 0) {
      char direction = Serial.read();
      delay(10);

      if (Serial.available() > 0) {
        int steps = Serial.parseInt();

        // Clear buffer
        while (Serial.available() > 0) Serial.read();

        Serial.print("Motor: "); Serial.println(motorID);
        Serial.print("Direction: "); Serial.println(direction);
        Serial.print("Steps: "); Serial.println(steps);

        if (motorID == 'X' || motorID == 'x') {
          rotateMotor(stepX, dirX, direction, steps);
        } else if (motorID == 'Y' || motorID == 'y') {
          rotateMotor(stepY, dirY, direction, steps);
        } else if (motorID == 'Z' || motorID == 'z') {
          rotateMotor(stepZ, dirZ, direction, steps);
        }
      }
    }
  }
}

void rotateMotor(int stepPin, int dirPin, char direction, int stepsToRotate) {
  digitalWrite(dirPin, (direction == 'R' || direction == 'r') ? HIGH : LOW);

  for (int i = 0; i < stepsToRotate; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
}