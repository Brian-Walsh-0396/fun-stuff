/*
  KY-008 Laser Module Test

  Wiring:
  KY-008 S  -> Arduino Nano D7
  KY-008 +  -> 5V
  KY-008 -  -> GND

  Serial Monitor:
  115200 baud

  Commands:
  ON   -> Laser ON
  OFF  -> Laser OFF
  TEST -> Laser ON for 2 seconds
  FIRE -> Fire 3 one-second laser pulses
  E    -> Emergency Stop
*/

const int LASER_PIN = 7;

// Timing
const unsigned long TEST_DURATION = 2000;
const unsigned long FIRE_ON_DURATION = 1000;
const unsigned long FIRE_OFF_DURATION = 500;

// TEST state
bool laserActive = false;
unsigned long laserStartTime = 0;

// FIRE state
bool fireActive = false;
bool fireLaserOn = false;

int fireCount = 0;

const int FIRE_TOTAL = 3;

unsigned long fireTimer = 0;


void setup() {

  Serial.begin(115200);

  pinMode(LASER_PIN, OUTPUT);

  // Make sure the laser starts OFF
  digitalWrite(LASER_PIN, LOW);

  Serial.println("================================");
  Serial.println("KY-008 Laser Test");
  Serial.println("================================");

  Serial.println("Commands:");
  Serial.println("ON   - Turn laser ON");
  Serial.println("OFF  - Turn laser OFF");
  Serial.println("TEST - Turn laser ON for 2 seconds");
  Serial.println("FIRE - Fire 3 laser pulses");
  Serial.println("E    - Emergency Stop");

  Serial.println();
  Serial.println("Ready.");
}


void loop() {

  // =========================================================
  // SERIAL COMMANDS
  // =========================================================

  if (Serial.available() > 0) {

    String command = Serial.readStringUntil('\n');

    command.trim();
    command.toUpperCase();


    // ---------------------------------------------------------
    // ON
    // ---------------------------------------------------------

    if (command == "ON") {

      cancelTimedOperations();

      digitalWrite(LASER_PIN, HIGH);

      Serial.println("LASER: ON");
    }


    // ---------------------------------------------------------
    // OFF
    // ---------------------------------------------------------

    else if (command == "OFF") {

      cancelTimedOperations();

      digitalWrite(LASER_PIN, LOW);

      Serial.println("LASER: OFF");
    }


    // ---------------------------------------------------------
    // TEST
    // ---------------------------------------------------------

    else if (command == "TEST") {

      // Cancel FIRE if currently running
      fireActive = false;

      digitalWrite(LASER_PIN, HIGH);

      laserActive = true;
      laserStartTime = millis();

      Serial.println("LASER: TEST STARTED");
    }


    // ---------------------------------------------------------
    // FIRE
    // ---------------------------------------------------------

    else if (command == "FIRE") {

      // Cancel TEST if currently running
      laserActive = false;

      fireActive = true;
      fireLaserOn = true;

      fireCount = 1;

      digitalWrite(LASER_PIN, HIGH);

      fireTimer = millis();

      Serial.println("==============================");
      Serial.println("LASER: FIRE SEQUENCE STARTED");
      Serial.println("==============================");

      Serial.print("LASER: PULSE ");
      Serial.print(fireCount);
      Serial.print("/");
      Serial.println(FIRE_TOTAL);
    }


    // ---------------------------------------------------------
    // EMERGENCY STOP
    // ---------------------------------------------------------

    else if (command == "E") {

      cancelTimedOperations();

      digitalWrite(LASER_PIN, LOW);

      Serial.println();
      Serial.println("*** EMERGENCY STOP ***");
      Serial.println("LASER: OFF");
    }


    // ---------------------------------------------------------
    // UNKNOWN COMMAND
    // ---------------------------------------------------------

    else {

      Serial.println("Unknown command.");
      Serial.println("Use ON, OFF, TEST, FIRE, or E.");
    }
  }


  // =========================================================
  // TEST TIMER
  // =========================================================

  if (laserActive) {

    if (millis() - laserStartTime >= TEST_DURATION) {

      digitalWrite(LASER_PIN, LOW);

      laserActive = false;

      Serial.println("LASER: TEST COMPLETE");
      Serial.println("LASER: OFF");
    }
  }


  // =========================================================
  // FIRE SEQUENCE
  // =========================================================

  if (fireActive) {

    // Laser currently ON
    if (fireLaserOn) {

      if (millis() - fireTimer >= FIRE_ON_DURATION) {

        digitalWrite(LASER_PIN, LOW);

        fireLaserOn = false;

        fireTimer = millis();

        Serial.println("LASER: OFF");
      }
    }

    // Laser currently OFF
    else {

      if (millis() - fireTimer >= FIRE_OFF_DURATION) {

        // Check whether all pulses are complete
        if (fireCount >= FIRE_TOTAL) {

          fireActive = false;

          digitalWrite(LASER_PIN, LOW);

          Serial.println("==============================");
          Serial.println("LASER: FIRE SEQUENCE COMPLETE");
          Serial.println("LASER: OFF");
          Serial.println("==============================");
        }

        else {

          fireCount++;

          digitalWrite(LASER_PIN, HIGH);

          fireLaserOn = true;

          fireTimer = millis();

          Serial.print("LASER: PULSE ");
          Serial.print(fireCount);
          Serial.print("/");
          Serial.println(FIRE_TOTAL);
        }
      }
    }
  }
}


// =============================================================
// CANCEL TIMED OPERATIONS
// =============================================================

void cancelTimedOperations() {

  laserActive = false;

  fireActive = false;
  fireLaserOn = false;

  fireCount = 0;
}