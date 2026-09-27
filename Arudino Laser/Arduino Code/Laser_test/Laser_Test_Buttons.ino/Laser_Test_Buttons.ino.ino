```cpp
  /*
  ============================================================
  KY-008 Laser Module Test
  Arduino Nano 3.0
  ============================================================

  LASER WIRING:
  KY-008 S  -> Arduino Nano D7
  KY-008 +  -> 5V
  KY-008 -  -> GND


  BUTTON WIRING:

  ON BUTTON:
  One side -> Arduino Nano D2
  Other side -> GND

  OFF BUTTON:
  One side -> Arduino Nano D3
  Other side -> GND


  BUTTON OPERATION:

  ON button  -> Laser ON
  OFF button -> Laser OFF


  SERIAL MONITOR:

  Baud Rate: 115200

  Commands:

  ON  -> Laser ON
  OFF -> Laser OFF


  IMPORTANT:

  The laser does NOT pulse.

  The laser does NOT automatically turn off.

  The laser will remain ON until:
  - The OFF button is pressed
  - The OFF serial command is sent
  - The Arduino is reset/powered off
*/

  // ============================================================
  // PIN DEFINITIONS
  // ============================================================

  // KY-008 laser signal
  const int LASER_PIN = 7;

// Physical buttons
const int LASER_ON_BUTTON = 2;
const int LASER_OFF_BUTTON = 3;


// ============================================================
// LASER STATE
// ============================================================

bool laserOn = false;


// ============================================================
// SETUP
// ============================================================

void setup() {

  // Start serial communication
  Serial.begin(115200);


  // ----------------------------------------------------------
  // Laser output
  // ----------------------------------------------------------

  pinMode(LASER_PIN, OUTPUT);


  // Make sure the laser starts OFF
  digitalWrite(LASER_PIN, LOW);


  // ----------------------------------------------------------
  // Button inputs
  // ----------------------------------------------------------
  //
  // INPUT_PULLUP uses the Nano's internal pull-up resistor.
  //
  // Button NOT pressed = HIGH
  // Button PRESSED     = LOW
  //
  // Therefore, each button connects between the Nano pin
  // and GND.
  // ----------------------------------------------------------

  pinMode(LASER_ON_BUTTON, INPUT_PULLUP);
  pinMode(LASER_OFF_BUTTON, INPUT_PULLUP);


  // ----------------------------------------------------------
  // Startup messages
  // ----------------------------------------------------------

  Serial.println("================================");
  Serial.println("KY-008 Laser Test");
  Serial.println("================================");

  Serial.println();
  Serial.println("BUTTONS:");
  Serial.println("ON  -> D2");
  Serial.println("OFF -> D3");

  Serial.println();
  Serial.println("SERIAL COMMANDS:");
  Serial.println("ON  - Turn laser ON");
  Serial.println("OFF - Turn laser OFF");

  Serial.println();
  Serial.println("Laser state: OFF");
  Serial.println("Ready.");
}


// ============================================================
// MAIN LOOP
// ============================================================

void loop() {


  // ==========================================================
  // CHECK ON BUTTON
  // ==========================================================

  if (digitalRead(LASER_ON_BUTTON) == LOW) {

    // Small debounce delay
    delay(50);

    // Confirm button is still pressed
    if (digitalRead(LASER_ON_BUTTON) == LOW) {

      turnLaserOn();

      // Wait for button release
      while (digitalRead(LASER_ON_BUTTON) == LOW) {
        delay(10);
      }
    }
  }


  // ==========================================================
  // CHECK OFF BUTTON
  // ==========================================================

  if (digitalRead(LASER_OFF_BUTTON) == LOW) {

    // Small debounce delay
    delay(50);

    // Confirm button is still pressed
    if (digitalRead(LASER_OFF_BUTTON) == LOW) {

      turnLaserOff();

      // Wait for button release
      while (digitalRead(LASER_OFF_BUTTON) == LOW) {
        delay(10);
      }
    }
  }


  // ==========================================================
  // CHECK SERIAL COMMANDS
  // ==========================================================

  if (Serial.available() > 0) {

    String command = Serial.readStringUntil('\n');

    command.trim();
    command.toUpperCase();


    // --------------------------------------------------------
    // ON COMMAND
    // --------------------------------------------------------

    if (command == "ON") {

      turnLaserOn();
    }


    // --------------------------------------------------------
    // OFF COMMAND
    // --------------------------------------------------------

    else if (command == "OFF") {

      turnLaserOff();
    }


    // --------------------------------------------------------
    // UNKNOWN COMMAND
    // --------------------------------------------------------

    else {

      Serial.println("Unknown command.");
      Serial.println("Use ON or OFF.");
    }
  }
}


// ============================================================
// TURN LASER ON
// ============================================================

void turnLaserOn() {

  digitalWrite(LASER_PIN, HIGH);

  laserOn = true;

  Serial.println("LASER: ON");
}


// ============================================================
// TURN LASER OFF
// ============================================================

void turnLaserOff() {

  digitalWrite(LASER_PIN, LOW);

  laserOn = false;

  Serial.println("LASER: OFF");
}
```
