# TimeDisplayLibrary

An Arduino library for controlling the original Showduino-style four-digit,
28-pixel NeoPixel seven-segment time display.

The cleanup keeps every original display mask intact while replacing repeated
pixel-writing code with a small, bounds-safe renderer. Existing sketches using
the old method names continue to work.

## Features

- Supports the existing 28-pixel display wiring and masks.
- Configurable RGB or packed NeoPixel colour.
- Clears and updates the display in one call.
- Checks whether the attached strip has at least 28 pixels.
- Retains the original API for backward compatibility.
- Includes a complete Arduino example.

## Dependency

Install **Adafruit NeoPixel** using the Arduino Library Manager.

## Basic usage

```cpp
#include <Adafruit_NeoPixel.h>
#include <TimeDisplayLibrary.h>

constexpr uint8_t DISPLAY_PIN = 6;
constexpr uint16_t DISPLAY_PIXELS = 28;

Adafruit_NeoPixel pixels(DISPLAY_PIXELS, DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
TimeDisplayLibrary timeDisplay(pixels);

void setup() {
    Serial.begin(115200);

    pixels.begin();
    pixels.setBrightness(80);
    pixels.clear();
    pixels.show();

    timeDisplay.setColor(0, 120, 255);
    timeDisplay.displayTwentyFive();
}

void loop() {
}
```

## Public API

| Method | Purpose |
| --- | --- |
| `setColor(r, g, b)` | Sets the colour used for patterns. |
| `setColor(packedColor)` | Sets an Adafruit NeoPixel packed colour. |
| `getColor()` | Returns the current packed colour. |
| `displayTwentyFive()` | Displays the original `twentyfive` mask. |
| `displayFiveTen()` | Displays the original `five_ten` mask. |
| `displayZero()` | Displays the original `zero` mask. |
| `displayNineFourTwo()` | Displays the original `ninefourtwo` mask. |
| `displayOneEightFourTwo()` | Displays the original `oneeightfourtwo` mask. |
| `clear()` | Turns all pixels off and updates the strip. |
| `hasValidPixelCount()` | Confirms that at least 28 pixels are available. |

## Legacy API

The following original calls remain supported:

```cpp
timeDisplay.twentyfive();
timeDisplay.five_ten();
timeDisplay.zero();
timeDisplay.ninefourtwo();
timeDisplay.oneeightfourtwo();
```

New Showduino code should use the readable camel-case names instead.

## Display assumptions

This version deliberately preserves the project's original physical pixel
mapping. It does not yet generate arbitrary digits because the repository does
not document the segment-to-pixel wiring clearly enough to infer that mapping
safely. A future Showduino integration can add a configurable segment map
without breaking these established masks.
