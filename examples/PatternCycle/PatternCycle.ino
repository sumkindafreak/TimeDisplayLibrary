#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <TimeDisplayLibrary.h>

// -----------------------------------------------------------------------------
// Hardware configuration
// -----------------------------------------------------------------------------
constexpr uint8_t DISPLAY_PIN = 6;
constexpr uint16_t DISPLAY_PIXEL_COUNT = 28;
constexpr uint8_t DISPLAY_BRIGHTNESS = 80;
constexpr unsigned long PATTERN_DURATION_MS = 1500;

// Create the NeoPixel strip and pass it into the time display library.
Adafruit_NeoPixel displayPixels(
    DISPLAY_PIXEL_COUNT,
    DISPLAY_PIN,
    NEO_GRB + NEO_KHZ800
);

TimeDisplayLibrary timeDisplay(displayPixels);

// Tracks which pattern is currently being shown.
uint8_t currentPattern = 0;
unsigned long lastPatternChangeMs = 0;

// -----------------------------------------------------------------------------
// Display helpers
// -----------------------------------------------------------------------------
void showCurrentPattern() {
    switch (currentPattern) {
        case 0:
            Serial.println(F("Displaying: Twenty Five"));
            timeDisplay.displayTwentyFive();
            break;

        case 1:
            Serial.println(F("Displaying: Five Ten"));
            timeDisplay.displayFiveTen();
            break;

        case 2:
            Serial.println(F("Displaying: Zero"));
            timeDisplay.displayZero();
            break;

        case 3:
            Serial.println(F("Displaying: Nine Four Two"));
            timeDisplay.displayNineFourTwo();
            break;

        case 4:
            Serial.println(F("Displaying: One Eight Four Two"));
            timeDisplay.displayOneEightFourTwo();
            break;

        default:
            currentPattern = 0;
            timeDisplay.displayTwentyFive();
            break;
    }
}

// -----------------------------------------------------------------------------
// Arduino setup
// -----------------------------------------------------------------------------
void setup() {
    Serial.begin(115200);
    delay(250);

    Serial.println();
    Serial.println(F("TimeDisplayLibrary pattern cycle starting..."));

    displayPixels.begin();
    displayPixels.setBrightness(DISPLAY_BRIGHTNESS);
    displayPixels.clear();
    displayPixels.show();

    if (!timeDisplay.hasValidPixelCount()) {
        Serial.println(F("ERROR: The display requires at least 28 NeoPixels."));
        return;
    }

    // Showduino-style blue can be changed at runtime without altering masks.
    timeDisplay.setColor(0, 120, 255);
    showCurrentPattern();
    lastPatternChangeMs = millis();
}

// -----------------------------------------------------------------------------
// Arduino main loop
// -----------------------------------------------------------------------------
void loop() {
    if (!timeDisplay.hasValidPixelCount()) {
        delay(1000);
        return;
    }

    const unsigned long nowMs = millis();
    if (nowMs - lastPatternChangeMs >= PATTERN_DURATION_MS) {
        lastPatternChangeMs = nowMs;
        currentPattern = (currentPattern + 1) % 5;
        showCurrentPattern();
    }
}
