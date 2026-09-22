/*
  KY-008 Laser Module Test

  Wiring:
  KY-008 S  -> Arduino Nano D7
  KY-008 +  -> 5V
  KY-008 -  -> GND

  Serial Monitor:
  115200 baud

  Commands:
  ON  -> Laser ON
  OFF -> Laser OFF
  TEST -> Laser ON for 2 seconds
*/

const int LASER_PIN = 7;

const unsigned long TEST_DURATION = 2000;

bool laserActive = false;
unsigned long laserStartTime = 0;

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
  Serial.println();
  Serial.println("Ready.");
}

void loop() {

  // Check for serial commands
  if (Serial.available() > 0) {

    String command = Serial.readStringUntil('\n');

    command.trim();
    command.toUpperCase();

    if (command == "ON") {

      digitalWrite(LASER_PIN, HIGH);
      laserActive = false;

      Serial.println("LASER: ON");
    }

    else if (command == "OFF") {

      digitalWrite(LASER_PIN, LOW);
      laserActive = false;

      Serial.println("LASER: OFF");
    }

    else if (command == "TEST") {

      digitalWrite(LASER_PIN, HIGH);

      laserActive = true;
      laserStartTime = millis();

      Serial.println("LASER: TEST STARTED");
    }

    else {

      Serial.println("Unknown command.");
      Serial.println("Use ON, OFF, or TEST.");
    }
  }

  // Automatically turn the laser off after the test duration
  if (laserActive) {

    if (millis() - laserStartTime >= TEST_DURATION) {

      digitalWrite(LASER_PIN, LOW);

      laserActive = false;

      Serial.println("LASER: TEST COMPLETE");
      Serial.println("LASER: OFF");
    }
  }
}