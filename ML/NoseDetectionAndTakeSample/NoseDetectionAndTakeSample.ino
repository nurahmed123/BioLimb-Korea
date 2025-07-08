#include <AccelStepper.h>

AccelStepper stepper1(AccelStepper::DRIVER, 2, 5); // X-axis
AccelStepper stepper2(AccelStepper::DRIVER, 3, 6); // Y-axis
AccelStepper stepper3(AccelStepper::DRIVER, 4, 7); // Z-axis
AccelStepper pusher (AccelStepper::DRIVER, 8, 9); // push cotton bud

void setup() {
  Serial.begin(115200);

  stepper1.setMaxSpeed(500);
  stepper2.setMaxSpeed(500);
  stepper3.setMaxSpeed(500);
  pusher.setMaxSpeed(300);

  stepper1.setAcceleration(200);
  stepper2.setAcceleration(200);
  stepper3.setAcceleration(200);
  pusher.setAcceleration(100);
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    int x = input.substring(0, input.indexOf(',')).toInt();
    int y = input.substring(input.indexOf(',') + 1, input.lastIndexOf(',')).toInt();
    int z = input.substring(input.lastIndexOf(',') + 1).toInt();

    Serial.print("Moving to: "); Serial.print(x); Serial.print(","); Serial.print(y); Serial.print(","); Serial.println(z);

    stepper1.moveTo(x);
    stepper2.moveTo(y);
    stepper3.moveTo(z);

    while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0 || stepper3.distanceToGo() != 0) {
      stepper1.run();
      stepper2.run();
      stepper3.run();
    }

    delay(1000); // short pause

    // Activate pusher motor
    pusher.moveTo(200); // push cotton bud 200 steps
    while (pusher.distanceToGo() != 0) {
      pusher.run();
    }

    delay(1000);
    pusher.moveTo(0); // retract
    while (pusher.distanceToGo() != 0) {
      pusher.run();
    }
  }
}
