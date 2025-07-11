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

  Serial.begin(9600); // Initialize serial communication at 9600 baud
}

void loop() {
  if (Serial.available() > 0) {
    char motorID = Serial.read();   // Read the motor ID ('X', 'Y', 'Z')
    // Add a short delay to ensure the next character is in the buffer
    delay(10); // Give a moment for the next byte to arrive

    if (Serial.available() > 0) {
      char direction = Serial.read(); // Read the direction ('L' or 'R')
      // Add a short delay
      delay(10); // Give a moment for the next byte to arrive

      if (Serial.available() > 0) {
        int steps = Serial.parseInt(); // Read the integer steps value

        // Clear any remaining characters in the buffer, especially the newline
        while (Serial.available() > 0) {
          Serial.read();
        }

        // --- DEBUGGING PRINTS ---
        Serial.print("DEBUG: Raw input (MotorID): "); Serial.println(motorID);
        Serial.print("DEBUG: Raw input (Direction): "); Serial.println(direction);
        Serial.print("DEBUG: Raw input (Steps): "); Serial.println(steps);
        // --- END DEBUGGING PRINTS ---


        if (motorID == 'X' || motorID == 'x') {
          Serial.println("DEBUG: Executing for X motor.");
          rotateMotor(stepX, dirX, direction, steps);
        } else if (motorID == 'Y' || motorID == 'y') {
          Serial.println("DEBUG: Executing for Y motor.");
          rotateMotor(stepY, dirY, direction, steps);
        } else if (motorID == 'Z' || motorID == 'z') {
          Serial.println("DEBUG: Executing for Z motor.");
          rotateMotor(stepZ, dirZ, direction, steps);
        } else {
            Serial.println("DEBUG: Unknown Motor ID received!");
        }
      } else {
        Serial.println("DEBUG: Not enough data for steps.");
      }
    } else {
      Serial.println("DEBUG: Not enough data for direction.");
    }
  }
}

void rotateMotor(int stepPin, int dirPin, char direction, int stepsToRotate) {
  if (direction == 'R' || direction == 'r') { // Clockwise
    digitalWrite(dirPin, HIGH);
  } else if (direction == 'L' || direction == 'l') { // Counter-clockwise
    digitalWrite(dirPin, LOW);
  }

  for (int i = 0; i < stepsToRotate; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000); // Adjust for desired speed

    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000); // Adjust for desired speed
  }
}
