#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <TimeDisplayLibrary.h>

// -----------------------------------------------------------------------------
// Hardware configuration
// -----------------------------------------------------------------------------
constexpr uint8_t DISPLAY_PIN = 6;
constexpr uint16_t DISPLAY_PIXEL_COUNT = 28;
constexpr uint8_t DISPLAY_BRIGHTNESS = 80;
constexpr unsigned long TIME_CHANGE_INTERVAL_MS = 2000;

// Create the NeoPixel strip and pass it into the time display library.
Adafruit_NeoPixel displayPixels(
    DISPLAY_PIXEL_COUNT,
    DISPLAY_PIN,
    NEO_GRB + NEO_KHZ800
);

TimeDisplayLibrary neoClock(displayPixels);

// Example times shown in HHMM format.
const uint16_t EXAMPLE_TIMES[] = {1900, 1942, 510, 0, 2359};
constexpr size_t EXAMPLE_TIME_COUNT = sizeof(EXAMPLE_TIMES) / sizeof(EXAMPLE_TIMES[0]);

size_t currentTimeIndex = 0;
unsigned long lastTimeChangeMs = 0;

// -----------------------------------------------------------------------------
// Display helper
// -----------------------------------------------------------------------------
void showCurrentTime() {
    const uint16_t hhmm = EXAMPLE_TIMES[currentTimeIndex];

    Serial.print(F("Displaying HHMM value: "));
    if (hhmm < 1000) {
        Serial.print('0');
    }
    if (hhmm < 100) {
        Serial.print('0');
    }
    if (hhmm < 10) {
        Serial.print('0');
    }
    Serial.println(hhmm);

    if (!neoClock.showTime(hhmm)) {
        Serial.println(F("ERROR: Invalid time or incorrect pixel count."));
    }
}

// -----------------------------------------------------------------------------
// Arduino setup
// -----------------------------------------------------------------------------
void setup() {
    Serial.begin(115200);
    delay(250);

    Serial.println();
    Serial.println(F("TimeDisplayLibrary direct time example starting..."));

    displayPixels.begin();
    displayPixels.setBrightness(DISPLAY_BRIGHTNESS);
    displayPixels.clear();
    displayPixels.show();

    if (!neoClock.hasValidPixelCount()) {
        Serial.println(F("ERROR: The display requires at least 28 NeoPixels."));
        return;
    }

    // Change this to true if the physical digits appear in reverse order.
    neoClock.setReverseDigitOrder(false);

    // Set the display colour once. Every rendered time uses this colour.
    neoClock.setColor(0, 120, 255);

    // This is the simple call requested for a 19:00 display.
    neoClock.showTime(1900);

    lastTimeChangeMs = millis();
}

// -----------------------------------------------------------------------------
// Arduino main loop
// -----------------------------------------------------------------------------
void loop() {
    if (!neoClock.hasValidPixelCount()) {
        delay(1000);
        return;
    }

    const unsigned long nowMs = millis();
    if (nowMs - lastTimeChangeMs >= TIME_CHANGE_INTERVAL_MS) {
        lastTimeChangeMs = nowMs;
        currentTimeIndex = (currentTimeIndex + 1) % EXAMPLE_TIME_COUNT;
        showCurrentTime();
    }
}
