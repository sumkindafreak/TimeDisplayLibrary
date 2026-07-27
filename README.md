# TimeDisplayLibrary

Arduino library for a four-digit, 28-pixel NeoPixel seven-segment display, prepared for future Showduino integration.

## The simple call

To display **19:00**:

```cpp
neoClock.showTime(1900);
```

You can also pass hours and minutes separately:

```cpp
neoClock.showTime(19, 0);
```

## Features

- Displays any valid `HHMM` time from `0000` to `2359`.
- Displays any four-digit number from `0000` to `9999`.
- Supports explicit four-digit calls such as `showDigits(1, 9, 0, 0)`.
- Configurable RGB or packed NeoPixel colour.
- Optional reversed physical digit order.
- Validates the required 28-pixel display size.
- Preserves every original hard-coded pattern and legacy method name.
- Includes a complete Arduino example.

## Dependency

Install **Adafruit NeoPixel** using the Arduino Library Manager.

## Complete basic example

```cpp
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <TimeDisplayLibrary.h>

constexpr uint8_t DISPLAY_PIN = 6;
constexpr uint16_t DISPLAY_PIXELS = 28;
constexpr uint8_t DISPLAY_BRIGHTNESS = 80;

Adafruit_NeoPixel pixels(
    DISPLAY_PIXELS,
    DISPLAY_PIN,
    NEO_GRB + NEO_KHZ800
);

TimeDisplayLibrary neoClock(pixels);

void setup() {
    Serial.begin(115200);

    pixels.begin();
    pixels.setBrightness(DISPLAY_BRIGHTNESS);
    pixels.clear();
    pixels.show();

    neoClock.setColor(0, 120, 255);
    neoClock.setReverseDigitOrder(false);

    if (!neoClock.showTime(1900)) {
        Serial.println(F("Unable to display 19:00"));
    }
}

void loop() {
}
```

## Main API

| Method | Purpose |
| --- | --- |
| `showTime(1900)` | Displays a validated HHMM clock value. |
| `showTime(19, 0)` | Displays validated hours and minutes. |
| `showNumber(1900)` | Displays any value from 0000 to 9999. |
| `showNumber(25, false)` | Displays 25 without leading zeroes. |
| `showDigits(1, 9, 0, 0)` | Displays four explicit digits. |
| `setColor(r, g, b)` | Sets the digit colour. |
| `setReverseDigitOrder(true)` | Reverses the physical digit chain. |
| `clear()` | Turns all pixels off. |
| `hasValidPixelCount()` | Confirms that at least 28 pixels exist. |

## Wiring assumption

The generic number renderer assumes four consecutive groups of seven pixels.
Within each digit, pixels must be ordered as standard seven-segment sections:

```text
0 = A, 1 = B, 2 = C, 3 = D, 4 = E, 5 = F, 6 = G
```

If the digits appear in the opposite left-to-right order, call:

```cpp
neoClock.setReverseDigitOrder(true);
```

## Legacy API

The original project methods remain available and still use their exact original pixel masks:

```cpp
neoClock.twentyfive();
neoClock.five_ten();
neoClock.zero();
neoClock.ninefourtwo();
neoClock.oneeightfourtwo();
```

This means older sketches continue to work while new Showduino code can use the much cleaner `showTime()` API.
