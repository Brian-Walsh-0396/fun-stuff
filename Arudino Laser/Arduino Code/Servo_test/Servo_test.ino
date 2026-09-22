#include <Servo.h>

// Servo objects
Servo panServo;
Servo tiltServo;

// Servo signal pins
const int PAN_PIN = 9;
const int TILT_PIN = 10;

// Conservative initial movement limits
const int PAN_MIN_ANGLE = 20;
const int PAN_MAX_ANGLE = 160;

const int TILT_MIN_ANGLE = 15;
const int TILT_MAX_ANGLE = 165;

// Center position
const int PAN_CENTER = 90;
const int TILT_CENTER = 90;

void setup() {
  Serial.begin(115200);

  // Attach servos
  panServo.attach(PAN_PIN);
  tiltServo.attach(TILT_PIN);

  // Start at center
  panServo.write(PAN_CENTER);
  tiltServo.write(TILT_CENTER);

  delay(1000);

  Serial.println("================================");
  Serial.println("Pan/Tilt Servo Test");
  Serial.println("================================");
  Serial.println("Commands:");
  Serial.println("CENTER - Move to center");
  Serial.println("PANMIN - Move pan to minimum");
  Serial.println("PANMAX - Move pan to maximum");
  Serial.println("TILTMIN - Move tilt to minimum");
  Serial.println("TILTMAX - Move tilt to maximum");
  Serial.println("TEST - Run complete servo test");
  Serial.println();
  Serial.println("Ready.");
}

void loop() {

  if (Serial.available() > 0) {

    String command = Serial.readStringUntil('\n');

    command.trim();
    command.toUpperCase();

    if (command == "CENTER") {

      moveToCenter();
    }

    else if (command == "PANMIN") {

      Serial.println("Moving pan to minimum...");
      panServo.write(PAN_MIN_ANGLE);
    }

    else if (command == "PANMAX") {

      Serial.println("Moving pan to maximum...");
      panServo.write(PAN_MAX_ANGLE);
    }

    else if (command == "TILTMIN") {

      Serial.println("Moving tilt to minimum...");
      tiltServo.write(TILT_MIN_ANGLE);
    }

    else if (command == "TILTMAX") {

      Serial.println("Moving tilt to maximum...");
      tiltServo.write(TILT_MAX_ANGLE);
    }

    else if (command == "TEST") {

      runServoTest();
    }

    else {

      Serial.println("Unknown command.");
    }
  }
}

void moveToCenter() {

  Serial.println("Moving to center...");

  panServo.write(PAN_CENTER);
  tiltServo.write(TILT_CENTER);

  delay(500);

  Serial.println("Position: CENTER");
}

void runServoTest() {

  Serial.println("Starting servo test...");
  Serial.println("Watch for mechanical binding.");

  // Center
  Serial.println("Center");
  panServo.write(PAN_CENTER);
  tiltServo.write(TILT_CENTER);
  delay(1000);

  // Pan minimum
  Serial.println("Pan minimum");
  panServo.write(PAN_MIN_ANGLE);
  delay(1000);

  // Pan maximum
  Serial.println("Pan maximum");
  panServo.write(PAN_MAX_ANGLE);
  delay(1000);

  // Return pan to center
  Serial.println("Pan center");
  panServo.write(PAN_CENTER);
  delay(1000);

  // Tilt minimum
  Serial.println("Tilt minimum");
  tiltServo.write(TILT_MIN_ANGLE);
  delay(1000);

  // Tilt maximum
  Serial.println("Tilt maximum");
  tiltServo.write(TILT_MAX_ANGLE);
  delay(1000);

  // Return tilt to center
  Serial.println("Tilt center");
  tiltServo.write(TILT_CENTER);
  delay(1000);

  Serial.println("Servo test complete.");
  Serial.println("Position: CENTER");
}