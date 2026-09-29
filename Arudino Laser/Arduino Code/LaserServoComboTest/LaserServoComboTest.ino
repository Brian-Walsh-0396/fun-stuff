#include <Servo.h>
#include <math.h>

// ============================================================
// PIN CONFIGURATION
// ============================================================

const byte PAN_PIN   = 10;
const byte TILT_PIN  = 9;
const byte LASER_PIN = 7;


// ============================================================
// SERVO LIMITS
// ============================================================

const byte PAN_MIN   = 20;
const byte PAN_MAX   = 160;

const byte TILT_MIN  = 15;
const byte TILT_MAX  = 165;

const byte PAN_CENTER  = 90;
const byte TILT_CENTER = 90;


// ============================================================
// MOVEMENT SETTINGS
// ============================================================

const byte SMOOTH_DELAY = 12;


// ============================================================
// DANCE SETTINGS
// ============================================================

const byte DANCE_PAN_RADIUS  = 35;
const byte DANCE_TILT_RADIUS = 35;

const byte DANCE_STEPS   = 120;
const byte DANCE_DELAY   = 15;
const byte DANCE_CIRCLES = 3;


// ============================================================
// LASER SETTINGS
// ============================================================

const unsigned int LASER_TEST_DURATION = 2000;
const unsigned int FIRE_ON_DURATION    = 1000;
const unsigned int FIRE_OFF_DURATION   = 500;

const byte FIRE_TOTAL = 3;


// ============================================================
// SERVO OBJECTS
// ============================================================

Servo panServo;
Servo tiltServo;


// ============================================================
// CURRENT POSITION
// ============================================================

byte currentPan  = PAN_CENTER;
byte currentTilt = TILT_CENTER;


// ============================================================
// LASER STATE
// ============================================================

bool laserTestActive = false;
bool fireActive = false;
bool fireLaserOn = false;

byte fireCount = 0;

unsigned long laserTimer = 0;
unsigned long fireTimer = 0;


// ============================================================
// SERIAL COMMAND BUFFER
//
// Replaces Arduino String to reduce SRAM use and avoid
// dynamic-memory fragmentation.
// ============================================================

char command[16];
byte commandIndex = 0;


// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(115200);

  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW);

  panServo.attach(PAN_PIN);
  tiltServo.attach(TILT_PIN);

  panServo.write(PAN_CENTER);
  tiltServo.write(TILT_CENTER);

  delay(500);

  Serial.println(F(""));
  Serial.println(F("=============================="));
  Serial.println(F("PAN / TILT + LASER CONTROL"));
  Serial.println(F("=============================="));

  Serial.println(F("CENTER"));
  Serial.println(F("PANMIN / PANMAX"));
  Serial.println(F("TILTMIN / TILTMAX"));
  Serial.println(F("SERVOTEST"));
  Serial.println(F("DANCE"));
  Serial.println(F("ON / OFF"));
  Serial.println(F("LASERTEST"));
  Serial.println(F("FIRE"));
  Serial.println(F("LSWEEP"));
  Serial.println(F("LNOD"));
  Serial.println(F("E - Emergency Laser Stop"));

  Serial.println(F("=============================="));
}


// ============================================================
// MAIN LOOP
// ============================================================

void loop() {

  readSerial();

  updateLaserTest();

  updateFire();
}


// ============================================================
// SERIAL READER
// ============================================================

void readSerial() {

  while (Serial.available()) {

    char c = Serial.read();

    // Ignore carriage return
    if (c == '\r') {
      continue;
    }

    // Command complete
    if (c == '\n') {

      command[commandIndex] = '\0';

      processCommand();

      commandIndex = 0;

      return;
    }

    // Convert lowercase to uppercase
    if (c >= 'a' && c <= 'z') {
      c -= 32;
    }

    // Store character if buffer has room
    if (commandIndex < sizeof(command) - 1) {

      command[commandIndex++] = c;
    }
  }
}


// ============================================================
// COMMAND PROCESSOR
// ============================================================

void processCommand() {

  if (strcmp(command, "CENTER") == 0) {

    centerServos();
  }

  else if (strcmp(command, "PANMIN") == 0) {

    smoothMove(
      PAN_MIN,
      currentTilt
    );
  }

  else if (strcmp(command, "PANMAX") == 0) {

    smoothMove(
      PAN_MAX,
      currentTilt
    );
  }

  else if (strcmp(command, "TILTMIN") == 0) {

    smoothMove(
      currentPan,
      TILT_MIN
    );
  }

  else if (strcmp(command, "TILTMAX") == 0) {

    smoothMove(
      currentPan,
      TILT_MAX
    );
  }

  else if (strcmp(command, "SERVOTEST") == 0) {

    servoTest();
  }

  else if (strcmp(command, "DANCE") == 0) {

    dance();
  }

  else if (strcmp(command, "ON") == 0) {

    cancelLaserTimers();

    digitalWrite(LASER_PIN, HIGH);

    Serial.println(F("LASER ON"));
  }

  else if (strcmp(command, "OFF") == 0) {

    laserOff();

    Serial.println(F("LASER OFF"));
  }

  else if (strcmp(command, "LASERTEST") == 0) {

    startLaserTest();
  }

  else if (strcmp(command, "FIRE") == 0) {

    startFire();
  }

  else if (strcmp(command, "LSWEEP") == 0) {

    laserSweep();
  }

  else if (strcmp(command, "LNOD") == 0) {

    laserNod();
  }

  else if (strcmp(command, "E") == 0) {

    laserOff();

    Serial.println(F("*** LASER STOPPED ***"));
  }

  else {

    Serial.println(F("Unknown command."));
  }
}


// ============================================================
// SMOOTH SERVO MOVEMENT
// ============================================================

void smoothMove(byte targetPan, byte targetTilt) {

  targetPan = constrain(
    targetPan,
    PAN_MIN,
    PAN_MAX
  );

  targetTilt = constrain(
    targetTilt,
    TILT_MIN,
    TILT_MAX
  );

  while (
    currentPan != targetPan ||
    currentTilt != targetTilt
  ) {

    if (currentPan < targetPan) {
      currentPan++;
    }

    else if (currentPan > targetPan) {
      currentPan--;
    }


    if (currentTilt < targetTilt) {
      currentTilt++;
    }

    else if (currentTilt > targetTilt) {
      currentTilt--;
    }


    panServo.write(currentPan);
    tiltServo.write(currentTilt);

    delay(SMOOTH_DELAY);
  }
}


// ============================================================
// CENTER SERVOS
// ============================================================

void centerServos() {

  smoothMove(
    PAN_CENTER,
    TILT_CENTER
  );

  Serial.println(F("CENTERED"));
}


// ============================================================
// SERVO TEST
// ============================================================

void servoTest() {

  laserOff();

  Serial.println(F("SERVO TEST START"));

  centerServos();

  delay(300);


  // Pan minimum
  smoothMove(
    PAN_MIN,
    TILT_CENTER
  );

  delay(300);


  // Pan maximum
  smoothMove(
    PAN_MAX,
    TILT_CENTER
  );

  delay(300);


  // Return pan center
  smoothMove(
    PAN_CENTER,
    TILT_CENTER
  );

  delay(300);


  // Tilt minimum
  smoothMove(
    PAN_CENTER,
    TILT_MIN
  );

  delay(300);


  // Tilt maximum
  smoothMove(
    PAN_CENTER,
    TILT_MAX
  );

  delay(300);


  // Center
  centerServos();

  Serial.println(F("SERVO TEST COMPLETE"));
}


// ============================================================
// DANCE MODE
// ============================================================

void dance() {

  laserOff();

  Serial.println(F("DANCE START"));

  centerServos();

  delay(200);


  // Move to starting point
  smoothMove(
    PAN_CENTER + DANCE_PAN_RADIUS,
    TILT_CENTER
  );


  for (
    byte circle = 0;
    circle < DANCE_CIRCLES;
    circle++
  ) {

    for (
      byte step = 0;
      step < DANCE_STEPS;
      step++
    ) {

      float angle =
        (2.0 * PI * step) / DANCE_STEPS;


      int panPosition =
        PAN_CENTER +
        DANCE_PAN_RADIUS * cos(angle);


      int tiltPosition =
        TILT_CENTER +
        DANCE_TILT_RADIUS * sin(angle);


      panPosition = constrain(
        panPosition,
        PAN_MIN,
        PAN_MAX
      );


      tiltPosition = constrain(
        tiltPosition,
        TILT_MIN,
        TILT_MAX
      );


      panServo.write(panPosition);
      tiltServo.write(tiltPosition);


      currentPan = panPosition;
      currentTilt = tiltPosition;


      delay(DANCE_DELAY);
    }
  }


  centerServos();

  Serial.println(F("DANCE COMPLETE"));
}


// ============================================================
// LASER SWEEP
//
// Pan Min
// Laser ON
// Pan Min -> Max
// Laser OFF
// Center
// ============================================================

void laserSweep() {

  laserOff();

  Serial.println(F("LASER SWEEP START"));


  // Move to starting position
  smoothMove(
    PAN_MIN,
    TILT_CENTER
  );

  delay(250);


  // Laser ON
  digitalWrite(
    LASER_PIN,
    HIGH
  );

  Serial.println(F("LASER ON"));


  // Sweep across
  smoothMove(
    PAN_MAX,
    TILT_CENTER
  );


  // Laser OFF
  digitalWrite(
    LASER_PIN,
    LOW
  );

  Serial.println(F("LASER OFF"));


  // Return to center
  centerServos();


  Serial.println(F("LASER SWEEP COMPLETE"));
}


// ============================================================
// LASER NOD
//
// Tilt Min
// Laser ON
// Tilt Min -> Max
// Laser OFF
// Center
// ============================================================

void laserNod() {

  laserOff();

  Serial.println(F("LASER NOD START"));


  // Move to starting position
  smoothMove(
    PAN_CENTER,
    TILT_MIN
  );

  delay(250);


  // Laser ON
  digitalWrite(
    LASER_PIN,
    HIGH
  );

  Serial.println(F("LASER ON"));


  // Nod
  smoothMove(
    PAN_CENTER,
    TILT_MAX
  );


  // Laser OFF
  digitalWrite(
    LASER_PIN,
    LOW
  );

  Serial.println(F("LASER OFF"));


  // Return to center
  centerServos();


  Serial.println(F("LASER NOD COMPLETE"));
}


// ============================================================
// LASER TEST
// ============================================================

void startLaserTest() {

  cancelLaserTimers();

  digitalWrite(
    LASER_PIN,
    HIGH
  );

  laserTestActive = true;

  laserTimer = millis();

  Serial.println(F("LASER TEST START"));
}


void updateLaserTest() {

  if (!laserTestActive) {
    return;
  }


  if (
    millis() - laserTimer
    >= LASER_TEST_DURATION
  ) {

    digitalWrite(
      LASER_PIN,
      LOW
    );

    laserTestActive = false;

    Serial.println(F("LASER TEST COMPLETE"));
  }
}


// ============================================================
// FIRE MODE
// ============================================================

void startFire() {

  cancelLaserTimers();

  fireActive = true;
  fireLaserOn = true;

  fireCount = 1;

  digitalWrite(
    LASER_PIN,
    HIGH
  );

  fireTimer = millis();

  Serial.println(F("FIRE START"));
}


void updateFire() {

  if (!fireActive) {
    return;
  }


  // ----------------------------------------------------------
  // Laser currently ON
  // ----------------------------------------------------------

  if (fireLaserOn) {

    if (
      millis() - fireTimer
      >= FIRE_ON_DURATION
    ) {

      digitalWrite(
        LASER_PIN,
        LOW
      );

      fireLaserOn = false;

      fireTimer = millis();
    }
  }


  // ----------------------------------------------------------
  // Laser currently OFF
  // ----------------------------------------------------------

  else {

    if (
      millis() - fireTimer
      >= FIRE_OFF_DURATION
    ) {

      // Finished
      if (fireCount >= FIRE_TOTAL) {

        fireActive = false;

        digitalWrite(
          LASER_PIN,
          LOW
        );

        Serial.println(F("FIRE COMPLETE"));
      }

      // Next pulse
      else {

        fireCount++;

        digitalWrite(
          LASER_PIN,
          HIGH
        );

        fireLaserOn = true;

        fireTimer = millis();
      }
    }
  }
}


// ============================================================
// LASER OFF
// ============================================================

void laserOff() {

  cancelLaserTimers();

  digitalWrite(
    LASER_PIN,
    LOW
  );
}


// ============================================================
// CANCEL LASER TIMERS
// ============================================================

void cancelLaserTimers() {

  laserTestActive = false;

  fireActive = false;

  fireLaserOn = false;

  fireCount = 0;
}