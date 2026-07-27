# NeoClock / TimeDisplayLibrary

Reusable Arduino library for a four-digit, 28-pixel NeoPixel seven-segment clock, prepared for Showduino integration.

## Preferred Showduino API

```cpp
#include <Adafruit_NeoPixel.h>
#include <NeoClock.h>

Adafruit_NeoPixel pixels(28, 6, NEO_GRB + NEO_KHZ800);
NeoClock neoClock;

void setup() {
    neoClock.begin(pixels);
    neoClock.setBrightness(80);
    neoClock.setColor(0, 120, 255);
    neoClock.showTime(1900); // displays 19:00
}

void loop() {}
```

You can also use:

```cpp
neoClock.showTime(19, 0);
neoClock.showNumber(1900);
neoClock.showDigits(1, 9, 0, 0);
neoClock.clear();
```

## Features

- Displays valid `HHMM` times from `0000` to `2359`.
- Displays numbers from `0000` to `9999`.
- Supports explicit four-digit rendering.
- Configurable RGB colour and brightness.
- Optional reversed physical digit order.
- Validates that at least 28 pixels are available.
- Supports both default construction plus `begin()` and constructor injection.
- Preserves the original class and method names for older sketches.

## Main API

| Method | Purpose |
| --- | --- |
| `begin(pixels)` | Attaches and initialises the NeoPixel display. |
| `isReady()` | Confirms that a valid display is attached. |
| `showTime(1900)` | Displays a validated HHMM value. |
| `showTime(19, 0)` | Displays validated hours and minutes. |
| `showNumber(1900)` | Displays any value from 0000 to 9999. |
| `showNumber(25, false)` | Displays 25 without leading zeroes. |
| `showDigits(1, 9, 0, 0)` | Displays four explicit digits. |
| `setColor(r, g, b)` | Sets the digit colour. |
| `setBrightness(80)` | Sets NeoPixel brightness from 0 to 255. |
| `setReverseDigitOrder(true)` | Reverses the physical digit chain. |
| `clear()` | Turns all pixels off. |

## Alternative construction

The display can still be supplied in the constructor:

```cpp
NeoClock neoClock(pixels);

void setup() {
    pixels.begin();
    neoClock.setBrightness(80);
    neoClock.showTime(1900);
}
```

## Wiring assumption

The generic renderer assumes four consecutive groups of seven pixels. Within each digit, pixels must be ordered:

```text
0 = A, 1 = B, 2 = C, 3 = D, 4 = E, 5 = F, 6 = G
```

If the digits appear right-to-left, call:

```cpp
neoClock.setReverseDigitOrder(true);
```

The exact physical segment order still needs validating against the original clock hardware.

## Backward compatibility

Existing projects can continue using:

```cpp
#include <TimeDisplayLibrary.h>

TimeDisplayLibrary neoClock(pixels);
neoClock.twentyfive();
neoClock.five_ten();
neoClock.zero();
neoClock.ninefourtwo();
neoClock.oneeightfourtwo();
```

`TimeDisplayLibrary` now wraps the newer `NeoClock` implementation, so old sketches remain valid while new Showduino code can use the cleaner API.

## Dependency

Install **Adafruit NeoPixel** through the Arduino Library Manager.
