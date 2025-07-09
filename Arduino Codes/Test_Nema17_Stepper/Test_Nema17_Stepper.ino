// Pin definitions for X, Y, Z motors
const int stepX = 2;
const int dirX  = 5;

const int stepY = 3;
const int dirY  = 6;

const int stepZ = 4;
const int dirZ  = 7;

const int enPin = 8; // Enable pin (shared)

void setup() {
  // Set all pins as output
  pinMode(stepX, OUTPUT);
  pinMode(dirX, OUTPUT);

  pinMode(stepY, OUTPUT);
  pinMode(dirY, OUTPUT);

  pinMode(stepZ, OUTPUT);
  pinMode(dirZ, OUTPUT);

  pinMode(enPin, OUTPUT);

  digitalWrite(enPin, LOW); // Enable all motors
}

void loop() {
  // Clockwise Rotation
  digitalWrite(dirX, HIGH);
  digitalWrite(dirY, HIGH);
  digitalWrite(dirZ, HIGH);
  rotateAllMotors();

  delay(1000);

  // Counterclockwise Rotation
  digitalWrite(dirX, LOW);
  digitalWrite(dirY, LOW);
  digitalWrite(dirZ, LOW);
  rotateAllMotors();

  delay(1000);
}

void rotateAllMotors() {
  for (int i = 0; i < 800; i++) {
    digitalWrite(stepX, HIGH);
    digitalWrite(stepY, HIGH);
    digitalWrite(stepZ, HIGH);
    delayMicroseconds(1000);

    digitalWrite(stepX, LOW);
    digitalWrite(stepY, LOW);
    digitalWrite(stepZ, LOW);
    delayMicroseconds(1000);
  }
}
