
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
// ANIMATION SETTINGS
// =====================================

#define FRAME_DELAY 20
#define RAINBOW_SPEED 128

#define DREAMCOAT_DELAY 1000
#define DREAMCOAT_BLUE_DELAY 3000

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
  MODE_RAINBOW,
  MODE_DREAMCOAT
};

DisplayMode currentMode = MODE_OFF;

uint16_t rainbowOffset = 0;
unsigned long lastFrame = 0;

String inputBuffer = "";

// =====================================
// DREAMCOAT COLOR DEFINITIONS
// =====================================

struct DreamcoatColor {
  const char* name;
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  unsigned long duration;
};

const DreamcoatColor dreamcoatColors[] = {

  { "Red", 255, 0, 0, 1000 },
  { "Yellow", 255, 255, 0, 1000 },
  { "Green", 0, 255, 0, 1000 },
  { "Brown", 139, 69, 19, 1000 },

  { "Scarlet", 255, 36, 0, 1000 },
  { "Black", 0, 0, 0, 1000 },
  { "Ochre", 204, 119, 34, 1000 },
  { "Peach", 255, 218, 185, 1000 },

  { "Ruby", 224, 17, 95, 1000 },
  { "Olive", 128, 128, 0, 1000 },
  { "Violet", 143, 0, 255, 1000 },
  { "Fawn", 229, 170, 112, 1000 },

  { "Lilac", 200, 162, 200, 1000 },
  { "Gold", 255, 215, 0, 1000 },
  { "Chocolate", 210, 105, 30, 1000 },
  { "Mauve", 224, 176, 255, 1000 },

  { "Cream", 255, 253, 208, 1000 },
  { "Crimson", 220, 20, 60, 1000 },
  { "Silver", 192, 192, 192, 1000 },
  { "Rose", 255, 0, 127, 1000 },

  { "Azure", 0, 127, 255, 1000 },
  { "Lemon", 255, 247, 0, 1000 },
  { "Russet", 128, 70, 27, 1000 },
  { "Grey", 128, 128, 128, 1000 },

  { "Purple", 128, 0, 255, 1000 },
  { "White", 255, 255, 255, 1000 },
  { "Pink", 255, 105, 180, 1000 },
  { "Orange", 255, 165, 0, 1000 },

  // Final Blue stays on for 3 seconds
  { "Blue", 0, 0, 255, 3000 }
};

const uint8_t DREAMCOAT_COUNT =
  sizeof(dreamcoatColors) / sizeof(dreamcoatColors[0]);

uint8_t dreamcoatIndex = 0;
unsigned long dreamcoatLastChange = 0;

// =====================================
// SETUP
// =====================================

void setup() {

  Serial.begin(115200);

  pixels.begin();

  // Start with all LEDs OFF
  currentMode = MODE_OFF;
  pixels.setBrightness(ON_BRIGHTNESS);
  pixels.clear();
  pixels.show();

  Serial.println();
  Serial.println("===========================");
  Serial.println("  QT Py NeoPixel Controller");
  Serial.println("===========================");
  Serial.println();

  Serial.println("Startup complete.");
  Serial.println("All LEDs are OFF.");
  Serial.println("Waiting for command.");

  printHelp();
}

// =====================================
// MAIN LOOP
// =====================================

void loop() {

  // Read serial commands
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

  // Rainbow animation
  if (currentMode == MODE_RAINBOW) {

    if (millis() - lastFrame >= FRAME_DELAY) {

      lastFrame = millis();
      updateRainbow();
    }
  }

  // Dreamcoat animation
  if (currentMode == MODE_DREAMCOAT) {

    updateDreamcoat();
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

  } else if (command == "ORANGE") {

    setSolidColor(255, 165, 0, COLOR_BRIGHTNESS);

  } else if (command == "YELLOW") {

    setSolidColor(255, 255, 0, COLOR_BRIGHTNESS);

  } else if (command == "GREEN") {

    setSolidColor(0, 255, 0, COLOR_BRIGHTNESS);

  } else if (command == "BLUE") {

    setSolidColor(0, 0, 255, COLOR_BRIGHTNESS);

  } else if (command == "WHITE") {

    setSolidColor(255, 255, 255, COLOR_BRIGHTNESS);

  } else if (command == "PURPLE") {

    setSolidColor(128, 0, 255, COLOR_BRIGHTNESS);

  } else if (command == "RAINBOW") {

    pixels.setBrightness(COLOR_BRIGHTNESS);

    currentMode = MODE_RAINBOW;
    rainbowOffset = 0;
    lastFrame = millis();

    updateRainbow();

    Serial.println("Mode: RAINBOW");

  } else if (command == "DREAMCOAT") {

    startDreamcoat();

  } else if (command == "LOCATOR") {

    showLocator();

  } else if (command == "B") {

    showB();

  } else if (command == "J") {

    showJ();

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
// SOLID COLOR CONTROL
// =====================================

void setSolidColor(
  uint8_t red,
  uint8_t green,
  uint8_t blue,
  uint8_t brightness) {

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
// LOCATOR MODE
// =====================================

void showLocator() {

  // Stop active animations
  currentMode = MODE_SOLID;

  // Set brightness to 80%
  pixels.setBrightness(COLOR_BRIGHTNESS);

  // Turn off all LEDs
  pixels.clear();

  // First LED (index 0) = Green
  pixels.setPixelColor(
    0,
    pixels.Color(0, 255, 0)
  );

  // Last LED (index 24) = Red
  pixels.setPixelColor(
    LED_COUNT - 1,
    pixels.Color(255, 0, 0)
  );

  // Update the LED matrix
  pixels.show();

  // Display location information
  Serial.println();
  Serial.println("===========================");
  Serial.println("       LOCATOR MODE");
  Serial.println("===========================");
  Serial.println();

  Serial.println("GREEN = First LED (1,1)");
  Serial.println("RED   = Last LED  (5,5)");

  Serial.println();
  Serial.println("All other LEDs are OFF.");
  Serial.println("---------------------------");
  Serial.println();
}

// =====================================
// B MODE
// =====================================

void showB() {

  // Stop active animations
  currentMode = MODE_SOLID;

  // Set brightness to 80%
  pixels.setBrightness(COLOR_BRIGHTNESS);

  // Turn off all LEDs
  pixels.clear();

  // Letter B pattern (5x5 matrix)
  // 1 = ON, 0 = OFF
  const bool letterB[5][5] = {
    {1, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 0}
  };

  // Draw the letter B
  for (int row = 0; row < 5; row++) {
    for (int col = 0; col < 5; col++) {

      if (letterB[row][col]) {

        // Serpentine NeoPixel layout
        int index;

        if (row % 2 == 0) {
          index = row * 5 + col;
        } else {
          index = row * 5 + (4 - col);
        }

        pixels.setPixelColor(
          index,
          pixels.Color(0, 128, 255)
        );
      }
    }
  }

  // Update the LED matrix
  pixels.show();

  // Display information
  Serial.println();
  Serial.println("===========================");
  Serial.println("          B MODE");
  Serial.println("===========================");
  Serial.println();
  Serial.println("Displaying letter B");
  Serial.println("Color: Blue (0, 128, 255)");
  Serial.println("Brightness: 80%");
  Serial.println("All other LEDs are OFF.");
  Serial.println("---------------------------");
  Serial.println();
}

// =====================================
// J MODE
// =====================================

void showJ() {

  // Stop active animations
  currentMode = MODE_SOLID;

  // Set brightness to 80%
  pixels.setBrightness(COLOR_BRIGHTNESS);

  // Turn off all LEDs
  pixels.clear();

  // Letter J pattern (5x5 matrix)
  // 1 = ON, 0 = OFF
  const bool letterJ[5][5] = {
    {0, 1, 1, 1, 1},
    {0, 0, 0, 1, 0},
    {0, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {0, 1, 1, 1, 0}
  };

  // Draw the letter J
  for (int row = 0; row < 5; row++) {
    for (int col = 0; col < 5; col++) {

      if (letterJ[row][col]) {

        // Serpentine NeoPixel layout
        int index;

        if (row % 2 == 0) {
          index = row * 5 + col;
        } else {
          index = row * 5 + (4 - col);
        }

        pixels.setPixelColor(
          index,
          pixels.Color(255, 218, 3)
        );
      }
    }
  }

  // Update the LED matrix
  pixels.show();

  // Display information
  Serial.println();
  Serial.println("===========================");
  Serial.println("          J MODE");
  Serial.println("===========================");
  Serial.println();
  Serial.println("Displaying letter J");
  Serial.println("Color: Gold (255, 218, 3)");
  Serial.println("Brightness: 80%");
  Serial.println("All other LEDs are OFF.");
  Serial.println("---------------------------");
  Serial.println();
}

// =====================================
// SMOOTH RAINBOW
// =====================================

void updateRainbow() {

  for (uint8_t i = 0; i < LED_COUNT; i++) {

    uint16_t hue =
      rainbowOffset +
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
// DREAMCOAT CONTROLLER
// =====================================

void startDreamcoat() {

  currentMode = MODE_DREAMCOAT;
  dreamcoatIndex = 0;

  Serial.println();
  Serial.println("DREAMCOAT MODE STARTED");
  Serial.println("---------------------------");
  Serial.println();

  Serial.println("I look handsome, I look smart");
  Serial.println("I am a walking work of art");
  Serial.println("Such a dazzling coat of many colors");
  Serial.println("How I love my coat of many colors");

  Serial.println();
  Serial.println("---------------------------");
  Serial.println();

  showDreamcoatColor();

  dreamcoatLastChange = millis();
}

// =====================================
// DISPLAY CURRENT DREAMCOAT COLOR
// =====================================

void showDreamcoatColor() {

  const DreamcoatColor& color =
    dreamcoatColors[dreamcoatIndex];

  pixels.setBrightness(COLOR_BRIGHTNESS);

  // Black means all LEDs OFF
  if (
    color.red == 0 &&
    color.green == 0 &&
    color.blue == 0
  ) {

    pixels.clear();

  } else {

    uint32_t pixelColor = pixels.Color(
      color.red,
      color.green,
      color.blue
    );

    for (uint8_t i = 0; i < LED_COUNT; i++) {
      pixels.setPixelColor(i, pixelColor);
    }
  }

  pixels.show();

  Serial.print("Dreamcoat: ");
  Serial.print(color.name);

  Serial.print(" | Duration: ");
  Serial.print(color.duration / 1000);
  Serial.println(" second(s)");
}

// =====================================
// ADVANCE DREAMCOAT SEQUENCE
// =====================================

void updateDreamcoat() {

  unsigned long currentTime = millis();

  unsigned long duration =
    dreamcoatColors[dreamcoatIndex].duration;

  if (
    currentTime - dreamcoatLastChange >= duration
  ) {

    dreamcoatIndex++;

    // Sequence completed after final Blue
    if (dreamcoatIndex >= DREAMCOAT_COUNT) {

      // Stop Dreamcoat mode
      currentMode = MODE_OFF;

      // Turn off all 25 LEDs
      pixels.clear();
      pixels.show();

      // Display completion message
      Serial.println();
      Serial.println("===========================");
      Serial.println("    DREAMCOAT COMPLETE");
      Serial.println("===========================");
      Serial.println();
      Serial.println("All LEDs turned OFF.");
      Serial.println();

      return;
    }

    // Display the next color
    showDreamcoatColor();

    dreamcoatLastChange = currentTime;
  }
}

// =====================================
// SERIAL HELP MENU
// =====================================

void printHelp() {

  Serial.println();
  Serial.println("AVAILABLE COMMANDS");
  Serial.println("---------------------------");

  Serial.println("RED       - Solid Red");
  Serial.println("ORANGE    - Solid Orange");
  Serial.println("YELLOW    - Solid Yellow");
  Serial.println("GREEN     - Solid Green");
  Serial.println("BLUE      - Solid Blue");
  Serial.println("WHITE     - Solid White");
  Serial.println("PURPLE    - Solid Purple");

  Serial.println();
  Serial.println("RAINBOW   - Smooth Rainbow");
  Serial.println("DREAMCOAT - Dreamcoat Color Cycle");
  Serial.println("LOCATOR   - Show First/Last LED");

  Serial.println();
  Serial.println("B         - Blue Letter B");
  Serial.println("J         - Gold Letter J");

  Serial.println();
  Serial.println("ON        - White at 50%");
  Serial.println("OFF       - Turn LEDs Off");
  Serial.println("HELP      - Show Commands");

  Serial.println();
  Serial.println("Color Brightness: 80%");
  Serial.println("Serial Baud Rate: 115200");
  Serial.println("---------------------------");
  Serial.println();
}
