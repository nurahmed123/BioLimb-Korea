// Pin definitions for X, Y, Z motors
#include <Servo.h>

// Define servo objects
Servo servoW;
Servo servoG;

const int servoW_Pin = 11;  // Digital Pin 11 (Spindle PWM)
const int servoG_Pin = A3;  // Analog Pin A3 (Coolant Enable)


const int stepX = 2;
const int dirX = 5;

const int stepY = 3;
const int dirY = 6;

const int stepZ = 4;
const int dirZ = 7;

const int enPin = 8;  // Enable pin (shared)


// --- Stepper RPM Configuration ---
// IMPORTANT: Adjust this value based on your motor's native steps and your driver's microstepping setting.
// For a common 1.8 degree motor (200 steps/revolution) with NO microstepping: STEPS_PER_REVOLUTION = 200
// For a 1.8 degree motor (200 steps/revolution) with 1/16 microstepping: STEPS_PER_REVOLUTION = 200 * 16 = 3200
const int STEPS_PER_REVOLUTION = 200;  // Set this to your effective steps per revolution

// Set the desired default RPM for the motors
const int DEFAULT_RPM = 30;  // For example, 60 RPM (1 revolution per second)

// Calculate the delay between step pulses required for the DEFAULT_RPM.
// Formula: delay_us = (60,000,000 microseconds/minute) / (STEPS_PER_REVOLUTION * DEFAULT_RPM * 2)
// The '2' is because there are two delayMicroseconds calls per step (one for HIGH, one for LOW).
// Using 'L' suffix for long literal to ensure calculation is done with long integers.
const long STEP_PULSE_DELAY_US = 30000000L / (STEPS_PER_REVOLUTION * DEFAULT_RPM);
// --- End Stepper RPM Configuration ---


const int WristDefaultAngle = 90;     // min 10 up and max 130 down
const int GrabberDefaultAngle = 179;  // close 180 open 160


void setup() {
  // Set all pins as output
  pinMode(stepX, OUTPUT);
  pinMode(dirX, OUTPUT);

  pinMode(stepY, OUTPUT);
  pinMode(dirY, OUTPUT);

  pinMode(stepZ, OUTPUT);
  pinMode(dirZ, OUTPUT);

  pinMode(enPin, OUTPUT);

  digitalWrite(enPin, LOW);  // Enable all motors (LOW enables most drivers)

  Serial.begin(9600);  // Initialize serial communication at 9600 baud
  Serial.println("Arduino Stepper Control Ready!");
  Serial.print("Configured for ");
  Serial.print(STEPS_PER_REVOLUTION);
  Serial.print(" steps/revolution and ");
  Serial.print(DEFAULT_RPM);
  Serial.println(" RPM.");
  Serial.print("Calculated step pulse delay: ");
  Serial.print(STEP_PULSE_DELAY_US);
  Serial.println(" microseconds.");



  // Attach servos to their pins
  servoW.attach(servoW_Pin);
  servoG.attach(servoG_Pin);

  // Set initial positions
  servoW.write(WristDefaultAngle);
  servoG.write(GrabberDefaultAngle);
}

void loop() {
  if (Serial.available() > 0) {
    String commandString = Serial.readStringUntil('\n');
    commandString.trim();

    Serial.print("DEBUG: Received command string: '");
    Serial.print(commandString);
    Serial.println("'");

    parseAndExecuteCommands(commandString);


    if (commandString == "TB") {  // take buds
      takeBuds();
    } else if (commandString == "IN") {  // index pos after take buds
      IndexPos();
    } else if (commandString == "TS") {  // take sample
      TakeSample();
    } else if (commandString == "PB") {  // put back buds
      PutBuds();
    } else if (commandString == "FULL") {  // put back buds
      FullWorkOut();
    }
  }
}


void FullWorkOut() {
  takeBuds();
  IndexPos();
  delay(500);
  TakeSample();
  delay(500);
  PutBuds();
}


void openGrabber() {
  parseAndExecuteCommands("G170");
  delay(10);
}

void closeGrabber() {
  parseAndExecuteCommands("G180");
  delay(10);
}


void takeBuds() {
  parseAndExecuteCommands("XR310");
  delay(500);

  openGrabber();
  delay(500);

  parseAndExecuteCommands("YB100");
  delay(10);


  parseAndExecuteCommands("ZB110");
  delay(500);

  parseAndExecuteCommands("YB50");
  delay(10);

  closeGrabber();
  delay(1000);

  // Y100 45 Z100

  // parseAndExecuteCommands("ZB5");
  // delay(10);


  // parseAndExecuteCommands("ZT20");
  // delay(10);

  // parseAndExecuteCommands("YT40");
  // delay(500);

  // parseAndExecuteCommands("ZT40");
  // delay(500);
}

void IndexPos() {
  // parseAndExecuteCommands("ZT5");
  // delay(10);

  parseAndExecuteCommands("YT50");
  delay(10);

  parseAndExecuteCommands("ZT110");
  delay(10);

  parseAndExecuteCommands("YT100");
  delay(10);

  parseAndExecuteCommands("XL310");
  delay(10);
}


void TakeSample() {
  delay(1500);
  parseAndExecuteCommands("ZT200");
  delay(500);

  parseAndExecuteCommands("W20");
  delay(300);

  parseAndExecuteCommands("YB70");
  delay(3000);

  parseAndExecuteCommands("YT120");
  delay(500);

  parseAndExecuteCommands("ZB150");
  parseAndExecuteCommands("YB30");
  parseAndExecuteCommands("ZB70");
  delay(300);

  parseAndExecuteCommands("W90");  // i may change that
  delay(10);
}


void PutBuds() {
  parseAndExecuteCommands("XR250");
  delay(10);

  parseAndExecuteCommands("YB120");
  delay(10);

  parseAndExecuteCommands("W85");
  delay(10);

  openGrabber();
  delay(1000);



  // parseAndExecuteCommands("ZT100");
  // delay(1000);
  PutBackToIndex();
}


void PutBackToIndex() {
  // parseAndExecuteCommands("ZB100");
  // delay(10);

  servoG.write(179);
  servoW.write(90);
  delay(1000);

  parseAndExecuteCommands("YT120");
  delay(10);

  parseAndExecuteCommands("XL250");
  delay(10);
}


void parseAndExecuteCommands(String inputString) {
  int startIndex = 0;
  while (startIndex < inputString.length()) {
    char motorID = inputString.charAt(startIndex);
    char direction;
    int steps;
    int endIndex;

    char upperMotorID = toupper(motorID);

    String angleString = inputString.substring(1);  // Get the rest of the string as the angle
    int angle = angleString.toInt();                // Convert the angle string to an integer

    if (upperMotorID == 'X' || upperMotorID == 'Y' || upperMotorID == 'Z' || upperMotorID == 'W' || upperMotorID == 'G') {
      if (startIndex + 1 < inputString.length()) {
        direction = inputString.charAt(startIndex + 1);

        endIndex = startIndex + 2;
        while (endIndex < inputString.length() && isDigit(inputString.charAt(endIndex))) {
          endIndex++;
        }

        if (endIndex > startIndex + 2) {
          String stepsString = inputString.substring(startIndex + 2, endIndex);
          steps = stepsString.toInt();

          Serial.print("DEBUG: Parsed - Motor: ");
          Serial.print(motorID);
          Serial.print(", Direction: ");
          Serial.print(direction);
          Serial.print(", Steps: ");
          Serial.println(steps);

          if (upperMotorID == 'X') {
            if (direction == 'L' || direction == 'l' || direction == 'R' || direction == 'r') {
              rotateMotor(stepX, dirX, direction, steps);
            } else {
              Serial.print("ERROR: Invalid direction for X motor (use L/R): ");
              Serial.println(direction);
            }
          } else if (upperMotorID == 'Y') {
            if (direction == 'B' || direction == 'b' || direction == 'T' || direction == 't') {
              rotateMotor(stepY, dirY, direction, steps);
            } else {
              Serial.print("ERROR: Invalid direction for Y motor (use B/T): ");
              Serial.println(direction);
            }
          } else if (upperMotorID == 'Z') {
            if (direction == 'B' || direction == 'b' || direction == 'T' || direction == 't') {
              rotateMotor(stepZ, dirZ, direction, steps);
            } else {
              Serial.print("ERROR: Invalid direction for Z motor (use B/T): ");
              Serial.println(direction);
            }
          } else if (upperMotorID == 'W') {
            Serial.println(motorID);
            Serial.println(angle);

            servoW.write(angle);
          } else if (upperMotorID == 'G') {
            Serial.println(motorID);
            Serial.println(angle);

            servoG.write(angle);
          }
        } else {
          Serial.print("DEBUG: Error parsing steps for motor: ");
          Serial.println(motorID);
        }
      } else {
        Serial.print("DEBUG: Error parsing direction for motor: ");
        Serial.println(motorID);
      }
    } else {
      Serial.print("DEBUG: Unknown motor ID encountered or end of valid commands: ");
      Serial.println(motorID);
      break;
    }
    startIndex = endIndex;
  }
}

void rotateMotor(int stepPin, int dirPin, char direction, int stepsToRotate) {
  Serial.print("Rotating motor on stepPin ");
  Serial.print(stepPin);
  Serial.print(", dirPin ");
  Serial.print(dirPin);
  Serial.print(" with direction ");
  Serial.print(direction);
  Serial.print(" for ");
  Serial.print(stepsToRotate);
  Serial.print(" steps at ");
  Serial.print(DEFAULT_RPM);
  Serial.println(" RPM.");


  char upperDirection = toupper(direction);

  // Logic to set direction for each motor type
  if (stepPin == stepX) {         // X-axis (L/R)
    if (upperDirection == 'R') {  // Right -> Clockwise
      digitalWrite(dirPin, HIGH);
    } else if (upperDirection == 'L') {  // Left -> Counter-clockwise
      digitalWrite(dirPin, LOW);
    } else {
      Serial.println("ERROR: Invalid direction for X-axis. Use 'L' or 'R'.");
      return;
    }
  } else if (stepPin == stepY) {  // Y-axis (B/T)
    if (upperDirection == 'T') {  // Top -> Counter-clockwise
      digitalWrite(dirPin, LOW);
    } else if (upperDirection == 'B') {  // Back -> Clockwise
      digitalWrite(dirPin, HIGH);
    } else {
      Serial.println("ERROR: Invalid direction for Y-axis. Use 'B' or 'T'.");
      return;
    }
  } else if (stepPin == stepZ) {  // Z-axis (B/T)
    if (upperDirection == 'T') {  // Top -> Clockwise
      digitalWrite(dirPin, HIGH);
    } else if (upperDirection == 'B') {  // Back -> Counter-clockwise
      digitalWrite(dirPin, LOW);
    } else {
      Serial.println("ERROR: Invalid direction for Z-axis. Use 'B' or 'T'.");
      return;
    }
  } else {
    Serial.println("ERROR: rotateMotor called with unknown stepPin.");
    return;
  }

  // Add a small delay after setting direction for some drivers to register
  delayMicroseconds(50);

  for (int i = 0; i < stepsToRotate; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(STEP_PULSE_DELAY_US);  // Use the calculated delay for desired RPM

    digitalWrite(stepPin, LOW);
    delayMicroseconds(STEP_PULSE_DELAY_US);  // Use the calculated delay for desired RPM
  }
  Serial.println("Motor rotation complete.");
}