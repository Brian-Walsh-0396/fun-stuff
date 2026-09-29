#include <Servo.h>
#include <math.h>

// ============================================================
// PAN / TILT SERVO TEST
// ============================================================
//
// Serial Monitor:
// Baud Rate: 115200
// Line Ending: Newline
//
// Commands:
//
// CENTER  - Move both servos smoothly to center
// PANMIN   - Move pan servo to minimum
// PANMAX   - Move pan servo to maximum
// TILTMIN  - Move tilt servo to minimum
// TILTMAX  - Move tilt servo to maximum
// TEST     - Run complete servo test
// DANCE    - Run smooth circular dance mode
//
// ============================================================


// ============================================================
// Servo Objects
// ============================================================

Servo panServo;
Servo tiltServo;


// ============================================================
// Servo Signal Pins
// ============================================================

const int PAN_PIN = 10;
const int TILT_PIN = 9;


// ============================================================
// Servo Movement Limits
// ============================================================
//
// These conservative limits help prevent the pan/tilt
// mechanism from reaching the physical limits of the servos.
//
// Adjust these values if mechanical binding occurs.
//

const int PAN_MIN_ANGLE = 20;
const int PAN_MAX_ANGLE = 160;

const int TILT_MIN_ANGLE = 15;
const int TILT_MAX_ANGLE = 165;


// ============================================================
// Center Position
// ============================================================

const int PAN_CENTER = 90;
const int TILT_CENTER = 90;


// ============================================================
// Current Servo Positions
// ============================================================
//
// These variables track the last position commanded by the
// Arduino.
//
// They allow the smoothMove() function to gradually transition
// from the current position to a new position.
//

int currentPan = PAN_CENTER;
int currentTilt = TILT_CENTER;


// ============================================================
// Smooth Movement Settings
// ============================================================
//
// Delay between each 1-degree movement.
//
// Higher value = slower movement
// Lower value  = faster movement
//

const int SMOOTH_DELAY = 12;


// ============================================================
// Dance Mode Settings
// ============================================================
//
// Starting configuration:
//
// Pan Radius   = 35 degrees
// Tilt Radius  = 35 degrees
// Steps        = 120
// Delay        = 15 milliseconds
//
// Pan and tilt move together using cosine and sine to
// approximate a circular movement.
//

const int DANCE_PAN_RADIUS = 35;
const int DANCE_TILT_RADIUS = 35;

const int DANCE_STEPS = 120;
const int DANCE_DELAY = 15;

const int DANCE_CIRCLES = 3;


// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(115200);

  // Attach the servos to their signal pins.
  panServo.attach(PAN_PIN);
  tiltServo.attach(TILT_PIN);

  // Start both servos at center.
  panServo.write(PAN_CENTER);
  tiltServo.write(TILT_CENTER);

  currentPan = PAN_CENTER;
  currentTilt = TILT_CENTER;

  // Give the servos time to reach center.
  delay(1000);

  // Display command menu.
  Serial.println("================================");
  Serial.println("Pan/Tilt Servo Test");
  Serial.println("================================");
  Serial.println();
  Serial.println("Commands:");
  Serial.println();
  Serial.println("CENTER  - Move smoothly to center");
  Serial.println("PANMIN   - Move pan to minimum");
  Serial.println("PANMAX   - Move pan to maximum");
  Serial.println("TILTMIN  - Move tilt to minimum");
  Serial.println("TILTMAX  - Move tilt to maximum");
  Serial.println("TEST     - Run complete servo test");
  Serial.println("DANCE    - Run smooth circular dance");
  Serial.println();
  Serial.println("Ready.");
}


// ============================================================
// MAIN LOOP
// ============================================================

void loop() {

  // Check whether a command has been entered
  // into the Serial Monitor.

  if (Serial.available() > 0) {

    String command = Serial.readStringUntil('\n');

    // Remove extra spaces/newline characters.
    command.trim();

    // Convert command to uppercase.
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

      Serial.println("Moving pan to minimum...");

      smoothMove(
        PAN_MIN_ANGLE,
        currentTilt
      );

      Serial.println("Pan minimum reached.");
    }


    // --------------------------------------------------------
    // PAN MAXIMUM
    // --------------------------------------------------------

    else if (command == "PANMAX") {

      Serial.println("Moving pan to maximum...");

      smoothMove(
        PAN_MAX_ANGLE,
        currentTilt
      );

      Serial.println("Pan maximum reached.");
    }


    // --------------------------------------------------------
    // TILT MINIMUM
    // --------------------------------------------------------

    else if (command == "TILTMIN") {

      Serial.println("Moving tilt to minimum...");

      smoothMove(
        currentPan,
        TILT_MIN_ANGLE
      );

      Serial.println("Tilt minimum reached.");
    }


    // --------------------------------------------------------
    // TILT MAXIMUM
    // --------------------------------------------------------

    else if (command == "TILTMAX") {

      Serial.println("Moving tilt to maximum...");

      smoothMove(
        currentPan,
        TILT_MAX_ANGLE
      );

      Serial.println("Tilt maximum reached.");
    }


    // --------------------------------------------------------
    // COMPLETE SERVO TEST
    // --------------------------------------------------------

    else if (command == "TEST") {

      runServoTest();
    }


    // --------------------------------------------------------
    // DANCE MODE
    // --------------------------------------------------------

    else if (command == "DANCE") {

      runDanceMode();
    }


    // --------------------------------------------------------
    // UNKNOWN COMMAND
    // --------------------------------------------------------

    else {

      Serial.print("Unknown command: ");
      Serial.println(command);
    }
  }
}


// ============================================================
// SMOOTH MOVEMENT FUNCTION
// ============================================================
//
// Moves both servos gradually toward their target positions.
//
// Instead of immediately commanding:
//
//     90 -> 20
//
// the servo receives:
//
//     90
//     89
//     88
//     87
//     ...
//     21
//     20
//
// This produces a more controlled movement.
//

void smoothMove(int targetPan, int targetTilt) {

  // Make sure the requested positions stay inside
  // the configured servo limits.

  targetPan = constrain(
    targetPan,
    PAN_MIN_ANGLE,
    PAN_MAX_ANGLE
  );

  targetTilt = constrain(
    targetTilt,
    TILT_MIN_ANGLE,
    TILT_MAX_ANGLE
  );


  // Continue until BOTH servos reach their targets.

  while (
    currentPan != targetPan ||
    currentTilt != targetTilt
  ) {

    // --------------------------------------------------------
    // Move Pan
    // --------------------------------------------------------

    if (currentPan < targetPan) {

      currentPan++;
    }

    else if (currentPan > targetPan) {

      currentPan--;
    }


    // --------------------------------------------------------
    // Move Tilt
    // --------------------------------------------------------

    if (currentTilt < targetTilt) {

      currentTilt++;
    }

    else if (currentTilt > targetTilt) {

      currentTilt--;
    }


    // Send the updated positions to the servos.

    panServo.write(currentPan);
    tiltServo.write(currentTilt);


    // Small delay creates controlled movement.

    delay(SMOOTH_DELAY);
  }
}


// ============================================================
// MOVE TO CENTER
// ============================================================

void moveToCenter() {

  Serial.println("Moving smoothly to center...");

  smoothMove(
    PAN_CENTER,
    TILT_CENTER
  );

  Serial.println("Position: CENTER");
}


// ============================================================
// COMPLETE SERVO TEST
// ============================================================

void runServoTest() {

  Serial.println();
  Serial.println("================================");
  Serial.println("Starting Servo Test");
  Serial.println("================================");
  Serial.println();

  Serial.println("Watch for mechanical binding.");
  Serial.println();


  // ----------------------------------------------------------
  // Center
  // ----------------------------------------------------------

  Serial.println("1. Moving to center...");

  smoothMove(
    PAN_CENTER,
    TILT_CENTER
  );

  delay(500);


  // ----------------------------------------------------------
  // Pan Minimum
  // ----------------------------------------------------------

  Serial.println("2. Moving pan to minimum...");

  smoothMove(
    PAN_MIN_ANGLE,
    TILT_CENTER
  );

  delay(500);


  // ----------------------------------------------------------
  // Pan Maximum
  // ----------------------------------------------------------

  Serial.println("3. Moving pan to maximum...");

  smoothMove(
    PAN_MAX_ANGLE,
    TILT_CENTER
  );

  delay(500);


  // ----------------------------------------------------------
  // Pan Center
  // ----------------------------------------------------------

  Serial.println("4. Returning pan to center...");

  smoothMove(
    PAN_CENTER,
    TILT_CENTER
  );

  delay(500);


  // ----------------------------------------------------------
  // Tilt Minimum
  // ----------------------------------------------------------

  Serial.println("5. Moving tilt to minimum...");

  smoothMove(
    PAN_CENTER,
    TILT_MIN_ANGLE
  );

  delay(500);


  // ----------------------------------------------------------
  // Tilt Maximum
  // ----------------------------------------------------------

  Serial.println("6. Moving tilt to maximum...");

  smoothMove(
    PAN_CENTER,
    TILT_MAX_ANGLE
  );

  delay(500);


  // ----------------------------------------------------------
  // Return to Center
  // ----------------------------------------------------------

  Serial.println("7. Returning to center...");

  smoothMove(
    PAN_CENTER,
    TILT_CENTER
  );

  delay(500);


  Serial.println();
  Serial.println("Servo test complete.");
  Serial.println("Position: CENTER");
  Serial.println();
}


// ============================================================
// DANCE MODE
// ============================================================
//
// Both servos move simultaneously.
//
// Pan movement uses:
//
//     cos(angle)
//
// Tilt movement uses:
//
//     sin(angle)
//
// Combining the two movements creates an approximate
// circular pattern.
//
// The movement is:
//
//              TILT
//                ^
//
//                *
//           *         *
//
//       *        +        *
//
//           *         *
//                *
//
//                ----------> PAN
//
//             + = Center
//
// ============================================================

void runDanceMode() {

  Serial.println();
  Serial.println("================================");
  Serial.println("Starting DANCE Mode");
  Serial.println("================================");
  Serial.println();

  Serial.println("Pan Radius: 35 degrees");
  Serial.println("Tilt Radius: 35 degrees");
  Serial.println("Steps: 120");
  Serial.println("Delay: 15 ms");
  Serial.println("Circles: 3");
  Serial.println();


  // ----------------------------------------------------------
  // STEP 1
  // Move both servos to center.
  // ----------------------------------------------------------

  Serial.println("Moving to center...");

  smoothMove(
    PAN_CENTER,
    TILT_CENTER
  );

  delay(300);


  // ----------------------------------------------------------
  // STEP 2
  // Calculate the starting point of the circle.
  //
  // At angle 0:
  //
  // cos(0) = 1
  // sin(0) = 0
  //
  // Therefore:
  //
  // Pan  = 90 + 35 = 125
  // Tilt = 90
  //
  // ----------------------------------------------------------

  int startingPan =
    PAN_CENTER + DANCE_PAN_RADIUS;

  int startingTilt =
    TILT_CENTER;


  Serial.println("Moving to dance starting position...");

  smoothMove(
    startingPan,
    startingTilt
  );

  delay(200);


  // ----------------------------------------------------------
  // STEP 3
  // Perform the circular dance.
  // ----------------------------------------------------------

  for (
    int circle = 0;
    circle < DANCE_CIRCLES;
    circle++
  ) {

    Serial.print("Dance circle ");
    Serial.print(circle + 1);
    Serial.print(" of ");
    Serial.println(DANCE_CIRCLES);


    // --------------------------------------------------------
    // Calculate 120 individual positions around the circle.
    // --------------------------------------------------------

    for (
      int step = 0;
      step < DANCE_STEPS;
      step++
    ) {

      // Convert the current step into an angle
      // between 0 and 2*PI.

      float angle =
        (2.0 * PI * step) / DANCE_STEPS;


      // ------------------------------------------------------
      // Calculate Pan Position
      // ------------------------------------------------------
      //
      // Cosine controls the horizontal movement.
      //

      int panPosition =
        PAN_CENTER +
        (DANCE_PAN_RADIUS * cos(angle));


      // ------------------------------------------------------
      // Calculate Tilt Position
      // ------------------------------------------------------
      //
      // Sine controls the vertical movement.
      //

      int tiltPosition =
        TILT_CENTER +
        (DANCE_TILT_RADIUS * sin(angle));


      // ------------------------------------------------------
      // Safety Limits
      // ------------------------------------------------------
      //
      // Prevent either servo from exceeding its configured
      // movement limits.
      //

      panPosition = constrain(
        panPosition,
        PAN_MIN_ANGLE,
        PAN_MAX_ANGLE
      );

      tiltPosition = constrain(
        tiltPosition,
        TILT_MIN_ANGLE,
        TILT_MAX_ANGLE
      );


      // ------------------------------------------------------
      // Move BOTH servos simultaneously.
      // ------------------------------------------------------

      panServo.write(panPosition);
      tiltServo.write(tiltPosition);


      // Record the new commanded positions.

      currentPan = panPosition;
      currentTilt = tiltPosition;


      // ------------------------------------------------------
      // Short delay between each calculated position.
      //
      // 120 small movements combined with a 15 ms delay
      // should produce a relatively smooth circular motion.
      // ------------------------------------------------------

      delay(DANCE_DELAY);
    }
  }


  // ----------------------------------------------------------
  // STEP 4
  // Return smoothly to center.
  // ----------------------------------------------------------

  Serial.println();
  Serial.println("Dance complete.");
  Serial.println("Returning to center...");

  smoothMove(
    PAN_CENTER,
    TILT_CENTER
  );


  Serial.println();
  Serial.println("================================");
  Serial.println("DANCE Mode Complete");
  Serial.println("================================");
  Serial.println("Position: CENTER");
  Serial.println();
  Serial.println("Ready.");
}