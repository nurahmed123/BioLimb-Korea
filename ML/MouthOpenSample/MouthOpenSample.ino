#include <AccelStepper.h>

// === Stepper Motor Pins ===
// IN1-IN4 for each motor driver
#define STEP1_PIN 2
#define DIR1_PIN 5

#define STEP2_PIN 3
#define DIR2_PIN 6

#define STEP3_PIN 4
#define DIR3_PIN 7

#define STEP_GRIP_PIN 8
#define DIR_GRIP_PIN 9

// === Create Stepper Objects ===
AccelStepper joint1(AccelStepper::DRIVER, STEP1_PIN, DIR1_PIN);
AccelStepper joint2(AccelStepper::DRIVER, STEP2_PIN, DIR2_PIN);
AccelStepper joint3(AccelStepper::DRIVER, STEP3_PIN, DIR3_PIN);
AccelStepper gripper(AccelStepper::DRIVER, STEP_GRIP_PIN, DIR_GRIP_PIN);

// === Motion Parameters ===
const float stepsPerDegree = 1.8; // Change this based on your motor+driver config
const float speed = 200.0;
const float accel = 100.0;

String inputString = "";
bool stringComplete = false;

void setup() {
  Serial.begin(9600);

  // Set acceleration/speed
  joint1.setMaxSpeed(speed);
  joint1.setAcceleration(accel);

  joint2.setMaxSpeed(speed);
  joint2.setAcceleration(accel);

  joint3.setMaxSpeed(speed);
  joint3.setAcceleration(accel);

  gripper.setMaxSpeed(400);
  gripper.setAcceleration(300);

  inputString.reserve(100);
}

void loop() {
  if (stringComplete) {
    inputString.trim();

    if (inputString == "PUSH") {
      moveGripper(true);
    } else if (inputString == "PULL") {
      moveGripper(false);
    } else {
      int t1, t2, t3;
      if (parseAngles(inputString, t1, t2, t3)) {
        moveJoints(t1, t2, t3);
      }
    }

    inputString = "";
    stringComplete = false;
  }

  joint1.run();
  joint2.run();
  joint3.run();
  gripper.run();
}

// === Move Joints ===
void moveJoints(int deg1, int deg2, int deg3) {
  joint1.moveTo(deg1 * stepsPerDegree);
  joint2.moveTo(deg2 * stepsPerDegree);
  joint3.moveTo(deg3 * stepsPerDegree);

  while (joint1.distanceToGo() != 0 ||
         joint2.distanceToGo() != 0 ||
         joint3.distanceToGo() != 0) {
    joint1.run();
    joint2.run();
    joint3.run();
  }
}

// === Gripper Push/Pull ===
void moveGripper(bool push) {
  int target = push ? 100 : 0; // 100 steps forward, adjust based on your gripper
  gripper.moveTo(target);

  while (gripper.distanceToGo() != 0) {
    gripper.run();
  }
}

// === Parse "45,30,10" Format ===
bool parseAngles(String data, int &a1, int &a2, int &a3) {
  int c1 = data.indexOf(',');
  int c2 = data.indexOf(',', c1 + 1);
  if (c1 == -1 || c2 == -1) return false;

  a1 = data.substring(0, c1).toInt();
  a2 = data.substring(c1 + 1, c2).toInt();
  a3 = data.substring(c2 + 1).toInt();
  return true;
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}
