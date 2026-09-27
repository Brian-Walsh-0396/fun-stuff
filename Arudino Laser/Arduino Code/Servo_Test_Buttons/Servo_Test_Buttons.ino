```cpp
#include <Servo.h>

// ============================================================
// SERVO OBJECTS
// ============================================================

Servo panServo;
Servo tiltServo;


// ============================================================
// SERVO SIGNAL PINS
// ============================================================

const int PAN_PIN = 9;
const int TILT_PIN = 10;


// ============================================================
// BUTTON PINS
// ============================================================

const int START_BUTTON = 2;
const int STOP_BUTTON = 3;
const int RESET_BUTTON = 4;


// ============================================================
// SERVO MOVEMENT LIMITS
// ============================================================

// Conservative initial movement limits
const int PAN_MIN_ANGLE = 20;
const int PAN_MAX_ANGLE = 160;

const int TILT_MIN_ANGLE = 15;
const int TILT_MAX_ANGLE = 165;


// ============================================================
// SERVO POSITIONS
// ============================================================

// Center position
const int PAN_CENTER = 90;
const int TILT_CENTER = 90;

// Home / Reset position
// NOTE:
// These values are 0,0 as requested.
// Make sure your hardware can safely move to 0 degrees.
const int PAN_HOME = 0;
const int TILT_HOME = 0;


// ============================================================
// SYSTEM STATE
// ============================================================

// System starts stopped
bool systemRunning = false;


// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(115200);


  // ----------------------------------------------------------
  // Configure buttons
  // ----------------------------------------------------------
  // INPUT_PULLUP means:
  //
  // Button NOT pressed = HIGH
  // Button PRESSED     = LOW
  //
  // Connect the other side of each button to GND.
  // ----------------------------------------------------------

  pinMode(START_BUTTON, INPUT_PULLUP);
  pinMode(STOP_BUTTON, INPUT_PULLUP);
  pinMode(RESET_BUTTON, INPUT_PULLUP);


  // ----------------------------------------------------------
  // Attach servos
  // ----------------------------------------------------------

  panServo.attach(PAN_PIN);
  tiltServo.attach(TILT_PIN);


  // ----------------------------------------------------------
  // Start at center
  // ----------------------------------------------------------

  panServo.write(PAN_CENTER);
  tiltServo.write(TILT_CENTER);


  delay(1000);


  // ----------------------------------------------------------
  // Serial startup message
  // ----------------------------------------------------------

  Serial.println("================================");
  Serial.println("Pan/Tilt Servo Test");
  Serial.println("================================");

  Serial.println();
  Serial.println("BUTTONS:");
  Serial.println("START  - D2");
  Serial.println("STOP   - D3");
  Serial.println("RESET  - D4");

  Serial.println();
  Serial.println("SERIAL COMMANDS:");
  Serial.println("CENTER  - Move to center");
  Serial.println("PANMIN  - Move pan to minimum");
  Serial.println("PANMAX  - Move pan to maximum");
  Serial.println("TILTMIN - Move tilt to minimum");
  Serial.println("TILTMAX - Move tilt to maximum");
  Serial.println("TEST    - Run complete servo test");

  Serial.println();
  Serial.println("System state: STOPPED");
  Serial.println("Ready.");
}


// ============================================================
// MAIN LOOP
// ============================================================

void loop() {


  // ==========================================================
  // CHECK START BUTTON
  // ==========================================================

  if (digitalRead(START_BUTTON) == LOW) {

    delay(50);  // Simple button debounce

    if (digitalRead(START_BUTTON) == LOW) {

      systemRunning = true;

      Serial.println("START button pressed.");
      Serial.println("System state: RUNNING");

      // Wait for button release
      while (digitalRead(START_BUTTON) == LOW) {
        delay(10);
      }
    }
  }


  // ==========================================================
  // CHECK STOP BUTTON
  // ==========================================================

  if (digitalRead(STOP_BUTTON) == LOW) {

    delay(50);  // Simple button debounce

    if (digitalRead(STOP_BUTTON) == LOW) {

      systemRunning = false;

      Serial.println("STOP button pressed.");
      Serial.println("System state: STOPPED");

      // Wait for button release
      while (digitalRead(STOP_BUTTON) == LOW) {
        delay(10);
      }
    }
  }


  // ==========================================================
  // CHECK RESET BUTTON
  // ==========================================================

  if (digitalRead(RESET_BUTTON) == LOW) {

    delay(50);  // Simple button debounce

    if (digitalRead(RESET_BUTTON) == LOW) {

      Serial.println("RESET button pressed.");

      moveToHome();

      // Reset does not automatically start the system
      systemRunning = false;

      Serial.println("System state: STOPPED");

      // Wait for button release
      while (digitalRead(RESET_BUTTON) == LOW) {
        delay(10);
      }
    }
  }


  // ==========================================================
  // SERIAL COMMANDS
  // ==========================================================

  if (Serial.available() > 0) {

    String command = Serial.readStringUntil('\n');

    command.trim();
    command.toUpperCase();


    // --------------------------------------------------------
    // CENTER
    // --------------------------------------------------------

    if (command == "CENTER") {

      moveToCenter();
    }


    // --------------------------------------------------------
    // PAN MINIMUM
    // --------------------------------------------------------

    else if (command == "PANMIN") {

      if (systemRunning) {

        Serial.println("Moving pan to minimum...");

        panServo.write(PAN_MIN_ANGLE);
      }

      else {

        Serial.println("System is stopped.");
        Serial.println("Press START first.");
      }
    }


    // --------------------------------------------------------
    // PAN MAXIMUM
    // --------------------------------------------------------

    else if (command == "PANMAX") {

      if (systemRunning) {

        Serial.println("Moving pan to maximum...");

        panServo.write(PAN_MAX_ANGLE);
      }

      else {

        Serial.println("System is stopped.");
        Serial.println("Press START first.");
      }
    }


    // --------------------------------------------------------
    // TILT MINIMUM
    // --------------------------------------------------------

    else if (command == "TILTMIN") {

      if (systemRunning) {

        Serial.println("Moving tilt to minimum...");

        tiltServo.write(TILT_MIN_ANGLE);
      }

      else {

        Serial.println("System is stopped.");
        Serial.println("Press START first.");
      }
    }


    // --------------------------------------------------------
    // TILT MAXIMUM
    // --------------------------------------------------------

    else if (command == "TILTMAX") {

      if (systemRunning) {

        Serial.println("Moving tilt to maximum...");

        tiltServo.write(TILT_MAX_ANGLE);
      }

      else {

        Serial.println("System is stopped.");
        Serial.println("Press START first.");
      }
    }


    // --------------------------------------------------------
    // SERVO TEST
    // --------------------------------------------------------

    else if (command == "TEST") {

      if (systemRunning) {

        runServoTest();
      }

      else {

        Serial.println("System is stopped.");
        Serial.println("Press START first.");
      }
    }


    // --------------------------------------------------------
    // UNKNOWN COMMAND
    // --------------------------------------------------------

    else {

      Serial.println("Unknown command.");
    }
  }
}


// ============================================================
// MOVE TO CENTER
// ============================================================

void moveToCenter() {

  Serial.println("Moving to center...");

  panServo.write(PAN_CENTER);
  tiltServo.write(TILT_CENTER);

  delay(500);

  Serial.println("Position: CENTER");
}


// ============================================================
// MOVE TO HOME / RESET
// ============================================================

void moveToHome() {

  Serial.println("Moving to HOME position...");
  Serial.println("Position: 0,0");

  panServo.write(PAN_HOME);
  tiltServo.write(TILT_HOME);

  delay(500);

  Serial.println("Position: HOME (0,0)");
}


// ============================================================
// COMPLETE SERVO TEST
// ============================================================

void runServoTest() {

  Serial.println("Starting servo test...");
  Serial.println("Watch for mechanical binding.");


  // ----------------------------------------------------------
  // Center
  // ----------------------------------------------------------

  Serial.println("Center");

  panServo.write(PAN_CENTER);
  tiltServo.write(TILT_CENTER);

  delay(1000);


  // ----------------------------------------------------------
  // Pan minimum
  // ----------------------------------------------------------

  Serial.println("Pan minimum");

  panServo.write(PAN_MIN_ANGLE);

  delay(1000);


  // ----------------------------------------------------------
  // Pan maximum
  // ----------------------------------------------------------

  Serial.println("Pan maximum");

  panServo.write(PAN_MAX_ANGLE);

  delay(1000);


  // ----------------------------------------------------------
  // Return pan to center
  // ----------------------------------------------------------

  Serial.println("Pan center");

  panServo.write(PAN_CENTER);

  delay(1000);


  // ----------------------------------------------------------
  // Tilt minimum
  // ----------------------------------------------------------

  Serial.println("Tilt minimum");

  tiltServo.write(TILT_MIN_ANGLE);

  delay(1000);


  // ----------------------------------------------------------
  // Tilt maximum
  // ----------------------------------------------------------

  Serial.println("Tilt maximum");

  tiltServo.write(TILT_MAX_ANGLE);

  delay(1000);


  // ----------------------------------------------------------
  // Return tilt to center
  // ----------------------------------------------------------

  Serial.println("Tilt center");

  tiltServo.write(TILT_CENTER);

  delay(1000);


  Serial.println("Servo test complete.");
  Serial.println("Position: CENTER");
}
```
