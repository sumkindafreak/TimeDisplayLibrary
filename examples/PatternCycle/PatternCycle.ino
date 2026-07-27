#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <NeoClock.h>

constexpr uint8_t DISPLAY_PIN = 6;
constexpr uint16_t DISPLAY_PIXEL_COUNT = 28;
constexpr uint8_t DISPLAY_BRIGHTNESS = 80;
constexpr unsigned long TIME_CHANGE_INTERVAL_MS = 2000;

Adafruit_NeoPixel displayPixels(
    DISPLAY_PIXEL_COUNT,
    DISPLAY_PIN,
    NEO_GRB + NEO_KHZ800
);

NeoClock neoClock;

const uint16_t EXAMPLE_TIMES[] = {1900, 1942, 510, 0, 2359};
constexpr size_t EXAMPLE_TIME_COUNT = sizeof(EXAMPLE_TIMES) / sizeof(EXAMPLE_TIMES[0]);

size_t currentTimeIndex = 0;
unsigned long lastTimeChangeMs = 0;

void showCurrentTime() {
    const uint16_t hhmm = EXAMPLE_TIMES[currentTimeIndex];

    Serial.print(F("Displaying HHMM value: "));
    if (hhmm < 1000) Serial.print('0');
    if (hhmm < 100) Serial.print('0');
    if (hhmm < 10) Serial.print('0');
    Serial.println(hhmm);

    if (!neoClock.showTime(hhmm)) {
        Serial.println(F("ERROR: Invalid time or display not ready."));
    }
}

void setup() {
    Serial.begin(115200);
    delay(250);

    if (!neoClock.begin(displayPixels)) {
        Serial.println(F("ERROR: NeoClock requires at least 28 NeoPixels."));
        return;
    }

    neoClock.setBrightness(DISPLAY_BRIGHTNESS);
    neoClock.setColor(0, 120, 255);
    neoClock.setReverseDigitOrder(false);

    // Display 19:00 using the simple Showduino-friendly call.
    neoClock.showTime(1900);
    lastTimeChangeMs = millis();
}

void loop() {
    if (!neoClock.isReady()) {
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
