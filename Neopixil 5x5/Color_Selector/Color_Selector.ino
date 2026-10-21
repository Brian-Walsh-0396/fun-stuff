
#include <Adafruit_NeoPixel.h>

// =====================================
// HARDWARE CONFIGURATION
// =====================================

#define LED_PIN A3
#define LED_COUNT 25

// =====================================
// BRIGHTNESS SETTINGS
// =====================================

#define COLOR_BRIGHTNESS 204  // 80%
#define ON_BRIGHTNESS 128     // 50%

// =====================================
// RAINBOW SETTINGS
// =====================================

#define FRAME_DELAY 20
#define RAINBOW_SPEED 128

Adafruit_NeoPixel pixels(
  LED_COUNT,
  LED_PIN,
  NEO_GRB + NEO_KHZ800
);

// =====================================
// OPERATING MODES
// =====================================

enum DisplayMode {
  MODE_OFF,
  MODE_SOLID,
  MODE_RAINBOW
};

DisplayMode currentMode = MODE_OFF;

uint16_t rainbowOffset = 0;
unsigned long lastFrame = 0;

String inputBuffer = "";

// =====================================
// SETUP
// =====================================

void setup() {

  Serial.begin(115200);

  pixels.begin();
  pixels.setBrightness(ON_BRIGHTNESS);
  pixels.clear();
  pixels.show();

  // Default startup: White at 50%
  setSolidColor(255, 255, 255, ON_BRIGHTNESS);

  delay(1000);

  Serial.println();
  Serial.println("===========================");
  Serial.println("  QT Py NeoPixel Controller");
  Serial.println("===========================");
  Serial.println();

  printHelp();
}

// =====================================
// MAIN LOOP
// =====================================

void loop() {

  // Process incoming serial commands
  while (Serial.available() > 0) {

    char incoming = Serial.read();

    if (incoming == '\n' || incoming == '\r') {

      if (inputBuffer.length() > 0) {
        processCommand(inputBuffer);
        inputBuffer = "";
      }

    } else {

      if (inputBuffer.length() < 32) {
        inputBuffer += incoming;
      }
    }
  }

  // Update rainbow without blocking
  if (currentMode == MODE_RAINBOW) {

    if (millis() - lastFrame >= FRAME_DELAY) {

      lastFrame = millis();
      updateRainbow();
    }
  }
}

// =====================================
// COMMAND PROCESSOR
// =====================================

void processCommand(String command) {

  command.trim();
  command.toUpperCase();

  Serial.print("Command received: ");
  Serial.println(command);

  if (command == "RED") {

    setSolidColor(255, 0, 0, COLOR_BRIGHTNESS);
    Serial.println("Color: RED");

  } else if (command == "ORANGE") {

    setSolidColor(255, 165, 0, COLOR_BRIGHTNESS);
    Serial.println("Color: ORANGE");

  } else if (command == "YELLOW") {

    setSolidColor(255, 255, 0, COLOR_BRIGHTNESS);
    Serial.println("Color: YELLOW");

  } else if (command == "GREEN") {

    setSolidColor(0, 255, 0, COLOR_BRIGHTNESS);
    Serial.println("Color: GREEN");

  } else if (command == "BLUE") {

    setSolidColor(0, 0, 255, COLOR_BRIGHTNESS);
    Serial.println("Color: BLUE");

  } else if (command == "WHITE") {

    setSolidColor(255, 255, 255, COLOR_BRIGHTNESS);
    Serial.println("Color: WHITE");

  } else if (command == "PURPLE") {

    setSolidColor(128, 0, 255, COLOR_BRIGHTNESS);
    Serial.println("Color: PURPLE");

  } else if (command == "RAINBOW") {

    pixels.setBrightness(COLOR_BRIGHTNESS);

    currentMode = MODE_RAINBOW;
    rainbowOffset = 0;
    lastFrame = millis();

    updateRainbow();

    Serial.println("Mode: RAINBOW");

  } else if (command == "ON") {

    setSolidColor(255, 255, 255, ON_BRIGHTNESS);
    Serial.println("Power: ON - White at 50%");

  } else if (command == "OFF") {

    currentMode = MODE_OFF;

    pixels.clear();
    pixels.show();

    Serial.println("Power: OFF");

  } else if (command == "HELP") {

    printHelp();

  } else {

    Serial.println("Unknown command.");
    Serial.println("Type HELP for available commands.");
  }
}

// =====================================
// SET SOLID COLOR
// =====================================

void setSolidColor(
  uint8_t red,
  uint8_t green,
  uint8_t blue,
  uint8_t brightness
) {

  currentMode = MODE_SOLID;

  pixels.setBrightness(brightness);

  uint32_t color = pixels.Color(
    red,
    green,
    blue
  );

  for (uint8_t i = 0; i < LED_COUNT; i++) {
    pixels.setPixelColor(i, color);
  }

  pixels.show();
}

// =====================================
// SMOOTH RAINBOW ANIMATION
// =====================================

void updateRainbow() {

  for (uint8_t i = 0; i < LED_COUNT; i++) {

    uint16_t hue = rainbowOffset +
      (uint32_t)i * 65536UL / LED_COUNT;

    uint32_t color = pixels.gamma32(
      pixels.ColorHSV(hue, 255, 255)
    );

    pixels.setPixelColor(i, color);
  }

  pixels.show();

  rainbowOffset += RAINBOW_SPEED;
}

// =====================================
// SERIAL HELP MENU
// =====================================

void printHelp() {

  Serial.println();
  Serial.println("AVAILABLE COMMANDS");
  Serial.println("---------------------------");

  Serial.println("RED     - Solid Red");
  Serial.println("ORANGE  - Solid Orange");
  Serial.println("YELLOW  - Solid Yellow");
  Serial.println("GREEN   - Solid Green");
  Serial.println("BLUE    - Solid Blue");
  Serial.println("WHITE   - Solid White");
  Serial.println("PURPLE  - Solid Purple");

  Serial.println();
  Serial.println("RAINBOW - Smooth Rainbow");
  Serial.println("ON      - White at 50%");
  Serial.println("OFF     - Turn LEDs Off");
  Serial.println("HELP    - Show Commands");

  Serial.println();
  Serial.println("Color brightness: 80%");
  Serial.println("Serial baud rate: 115200");
  Serial.println("---------------------------");
  Serial.println();
}
